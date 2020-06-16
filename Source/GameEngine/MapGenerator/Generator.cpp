#include <include/glm.h>
#include <time.h>
#include <list>
#include <GameEngine/IncludeList.h>
#include "Generator.h"
#include <GameEngine/Utils/ComponentFactory.h>
#include <GameEngine/Utils/DebugDraw.h>
#include <GameEngine/MapGenerator/GeneratorManager.h>
#include <GameEngine/Components/NPC/NPCSpawner.h>

Generator::Generator(int roomCount, int locationMaxX, int locationMaxY, int sizeMaxX, int sizeMaxY, int navigationMatrixSizeMultiplier) {

	this->roomCount = roomCount;
	this->locationMaxX = locationMaxX;
	this->locationMaxY = locationMaxY;
	this->sizeMaxX = sizeMaxX;
	this->sizeMaxY = sizeMaxY;
	navSizeMult = navigationMatrixSizeMultiplier;

	GeneratorManager& gm = GeneratorManager::GetInstance();
	gm.SetGenerator(this);

	texture = new Texture2D();
	texture->Load2D("Source/GameEngine/Textures/floor.jpg", GL_REPEAT);

	mapMatrix = new int* [(2 * locationMaxX + 10) * navSizeMult];
	auxMapMatrix = new int* [(2 * locationMaxX + 10)];
	for (int i = 0; i < (2 * locationMaxX + 10) * navSizeMult; i++) {
		if (i < 2 * locationMaxX + 10) {
			auxMapMatrix[i] = new int[(2 * locationMaxX + 10)];
		}
		mapMatrix[i] = new int[(2 * locationMaxX + 10) * navSizeMult];
	}

	for (int i = 0; i < (2 * locationMaxX + 10) * navSizeMult; i++) {
		for (int j = 0; j < (2 * locationMaxX + 10) * navSizeMult; j++) {
			if (i < 2 * locationMaxX + 10 && j < 2 * locationMaxX + 10) {
				auxMapMatrix[i][j] = 1;
			}
			mapMatrix[i][j] = 1;
		}
	}

	astar = new AStarAlgorithm((2 * locationMaxX + 10) * navSizeMult, (2 * locationMaxX + 10) * navSizeMult);
}

Generator::~Generator() {
	for (int i = 0; i < (2 * locationMaxX + 10) * navSizeMult; ++i) {
		if (i < 2 * locationMaxX + 10) {
			delete[] auxMapMatrix[i];
		}
		delete[] mapMatrix[i];
	}
	delete[] mapMatrix;
	delete[] auxMapMatrix;
	delete texture;
	delete astar;
}

void Generator::Init() {
	NPCSpawner::SpawnNPC(player, rooms.back().location);
}

glm::vec3 Generator::GetSpawnPoint() {
	return rooms.front().location;
}


void Generator::PlaceRooms() {

	int index = 0;

	while (index < roomCount) {

		float randomx = rand() % (2 * locationMaxX) + 5;
		float randomy = rand() % (2 * locationMaxY) + 5;

		randomx -= fmod(randomx, 2.0f);
		randomy -= fmod(randomy, 2.0f);

		printf("%lf %lf\n", randomx, randomy);

		glm::vec3 location = glm::vec3(randomx, 0, randomy);

		randomx = 2 * (rand() % sizeMaxX) + 4;
		randomy = 2 * (rand() % sizeMaxY) + 4;

		glm::vec3 size = glm::vec3(randomx, 0.01f, randomy);

		bool add = true;
		Room* newRoom = new Room(location, size);
		for (Room room : rooms) {
			if (newRoom->Intersection(room, 2.f)) {
				add = false;
				break;
			}
		}
		if (add) {
			rooms.push_back(*newRoom);
			PlaceRoom(newRoom->location, newRoom->size);
			index++;
			for (int i = newRoom->location.x - newRoom->size.x/2; i < newRoom->location.x + newRoom->size.x/2; i++) {
				for (int j = newRoom->location.z - newRoom->size.z/2; j < newRoom->location.z + newRoom->size.z/2; j++) {
					auxMapMatrix[j][i] = 0;
				}
			}
		}
		else {
			continue;
		}
		

	}

	std::vector<Delaunay::VertexType> roomLocations;

	for (Room room : rooms) {
		roomLocations.push_back(Delaunay::VertexType(room.location.x, room.location.z));
	}

	Delaunay* del = new Delaunay();
	std::vector<Delaunay::TriangleType> triangles = del->triangulate(roomLocations);

	std::map<std::pair<float, float>, std::pair<float, float>> MST = MSTAlgorithm::Algorithm(triangles, roomLocations);

	for (int i = 0; i < 3; i++) {
		
		auto it = MST.begin();
		std::advance(it, rand() % MST.size());
		std::pair<float, float> random_key_x = it->first;

		it = MST.begin();
		std::advance(it, rand() % MST.size());
		std::pair<float, float> random_key_y = it->first;

		if (random_key_x == random_key_y) {
			i--;
			continue;
		}

		MST.insert({ random_key_x, random_key_y });

	}

	for (auto const& edge : MST) {
		glm::vec3 size = glm::vec3(1);
		Room *from, *to;
		for (auto room : rooms) {
			if (room.location.x == edge.first.first && room.location.z == edge.first.second) {
				from = new Room(room.location, room.size);
				
			}
			if (room.location.x == edge.second.first && room.location.z == edge.second.second) {
				to = new Room(room.location, room.size);
			}
		}
		if (from->location != to->location) {
			PlaceCorridors(GetCorridorBoundary(from, to), to->location, size);
		}
		delete to;
		delete from;
	}

	for (auto room : rooms) {
		PlaceWall(room);
	}

	for (int i = 0; i < 2 * locationMaxX + 10; i++) {
		for (int j = 0; j < 2 * locationMaxY + 10; j++) {
			for (int k = 0; k < navSizeMult; k++) {
				for (int l = 0; l < navSizeMult; l++) {
					mapMatrix[i * navSizeMult + k][j * navSizeMult + l] = auxMapMatrix[i][j];
				}
			}
			printf("%d", auxMapMatrix[i][j]);
		}
		printf("\n");
	}
}

int Generator::CalcLinearInterp(float nr, int mode) {
	if (mode == 1) {
		float t = nr - floor(nr);
		float mult = float(navSizeMult);
		return int(round((1 - t) * floor(nr) * mult + t * ceil(nr) * (mult)));
	}
	else {
		return int(round(nr * float(navSizeMult)));
	}
}

int Generator::GetNavMatrixValue(glm::vec3 pos, int mode) {
	return mapMatrix[CalcLinearInterp(pos.z, mode)][CalcLinearInterp(pos.x, mode)];
}

std::stack<std::pair<float, float>> Generator::GetPath(glm::vec3 from, glm::vec3 to) {
	printf("from %d %d\n", GetNavMatrixValue(from), GetNavMatrixValue(from, 0));

	std::stack<std::pair<int, int>> aStarRes = astar->aStarSearch(&mapMatrix, std::make_pair(CalcLinearInterp(from.z),
		CalcLinearInterp(from.x)),
		std::make_pair(CalcLinearInterp(to.z),
			CalcLinearInterp(to.x)));

	std::stack<std::pair<float, float>> res, tmp;

	while (!aStarRes.empty()) {
		std::pair<int, int> p = aStarRes.top();
		aStarRes.pop();
		tmp.push(std::pair<float, float>((p.first / (float)navSizeMult), (p.second / (float)navSizeMult)));
	}

	while (!tmp.empty()) {
		std::pair<float, float> p = tmp.top();
		res.push(p);
		tmp.pop();
	}

	return res;
}

glm::vec3 Generator::GetCorridorBoundary(Room* r1, Room* r2) {
	glm::vec3 boundary;

	glm::vec3 finalPos = r2->location;
	glm::vec3 crtPos = r1->location;
	glm::vec3 size = r1->size;

	float leftDist = glm::l1Norm(finalPos - (crtPos - glm::vec3(round(size.x / 2.0f + 1), 0, 0)));
	float rightDist = glm::l1Norm(finalPos - (crtPos + glm::vec3(round(size.x / 2.0f + 1), 0, 0)));
	float bkdDist = glm::l1Norm(finalPos - (crtPos - glm::vec3(0, 0, round(size.z / 2.0f + 1))));
	float fwdDist = glm::l1Norm(finalPos - (crtPos + glm::vec3(0, 0, round(size.z / 2.0f + 1))));

	float min = leftDist;
	glm::vec3 next = crtPos - glm::vec3(round(size.x / 2.0f + 1), 0, 0);

	if (rightDist < min) {
		min = rightDist;
		next = crtPos + glm::vec3(round(size.x / 2.0f + 1), 0, 0);
	}

	if (bkdDist < min) {
		min = bkdDist;
		next = crtPos - glm::vec3(0, 0, round(size.z / 2.0f + 1));
	}

	if (fwdDist < min) {
		min = fwdDist;
		next = crtPos + glm::vec3(0, 0, round(size.z / 2.0f + 1));
	}

	printf("qwe %lf %lf %lf\n", next.x, next.y, next.z);
		
	PlaceCorridor(next, 2);
	return next;
}
	
void Generator::PlaceRoom(glm::vec3 location, glm::vec3 size) {

	ComponentFactory& factory = ComponentFactory::GetInstance();
	auto object = factory.createObject(3);

	static_cast<PointShadowRenderer*>(object->GetComponent("PointShadowRenderer"))->SetMesh("floor01");
	static_cast<RigidBodyComponent*>(object->GetComponent("RigidBodyComponent"))->SetWalkable(true);

	object->GetTransform()->SetPos(location);
	object->GetTransform()->SetScale(size);
	object->InitComponents();

	EngineManager& em = EngineManager::GetInstance();

	em.GetGameEngine()->AddObject(object);
}

void Generator::PlaceCorridor(glm::vec3 location, float size) {

	ComponentFactory& factory = ComponentFactory::GetInstance();
	auto object = factory.createObject(3);

	static_cast<PointShadowRenderer*>(object->GetComponent("PointShadowRenderer"))->SetMesh("floor01");
	static_cast<RigidBodyComponent*>(object->GetComponent("RigidBodyComponent"))->SetWalkable(true);

	object->GetTransform()->SetPos(location);
	object->GetTransform()->SetScale(glm::vec3(size, 0, size));
	object->InitComponents();

	EngineManager& em = EngineManager::GetInstance();

	em.GetGameEngine()->AddObject(object);

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (auxMapMatrix[(int)location.z - 1 + j][(int)location.x - 1 + i] == 1) {
				auxMapMatrix[(int)location.z - 1 + j][(int)location.x - 1 + i] = 4;
			}
		}
	}
	
}

void Generator::PlaceCorridors(glm::vec3 crtPos, glm::vec3 finalPos, glm::vec3 size) {

	if (auxMapMatrix[(int)crtPos.z][(int)crtPos.x] == 0) {
		return;
	}

	float multiplier = 2.0f;

	/*if (glm::l1Norm(crtPos, finalPos) <= 2.f) {
		printf("%lf\n", glm::l1Norm(crtPos, finalPos));
		multiplier = 1.0f;
	}*/

	float leftDist = glm::l1Norm(finalPos - (crtPos - glm::vec3(multiplier, 0, 0)));
	float rightDist = glm::l1Norm(finalPos - (crtPos + glm::vec3(multiplier, 0, 0)));
	float bkdDist = glm::l1Norm(finalPos - (crtPos - glm::vec3(0, 0, multiplier)));
	float fwdDist = glm::l1Norm(finalPos - (crtPos + glm::vec3(0, 0, multiplier)));

	float min = leftDist;
	glm::vec3 next = crtPos - glm::vec3(multiplier, 0, 0);

	if (rightDist < min) {
		min = rightDist;
		next = crtPos + glm::vec3(multiplier, 0, 0);
	}

	if (bkdDist < min) {
		min = bkdDist;
		next = crtPos - glm::vec3(0, 0, multiplier);
	}

	if (fwdDist < min) {
		min = fwdDist;
		next = crtPos + glm::vec3(0, 0, multiplier);
	}

	printf("next %d %d\n", (int)next.x, (int)next.z);

	if (auxMapMatrix[(int)next.z][(int)next.x] == 1) {
		PlaceCorridor(next, (int)multiplier);
	}
	else {
		glm::vec3 dir = next - crtPos;
		next -= dir / 2.0f;

		if (auxMapMatrix[(int)next.z][(int)next.x] == 1) {
			PlaceCorridor(next, (int)multiplier);
		}
	}

	PlaceCorridors(next, finalPos, size);
}

void Generator::PlaceWalls(Room room) {

}

void Generator::PlaceWall(Room room) {
	ComponentFactory& factory = ComponentFactory::GetInstance();
	EngineManager& em = EngineManager::GetInstance();

	auto object1 = factory.createObject(3);
	auto object2 = factory.createObject(3);
	auto object3 = factory.createObject(3);
	auto object4 = factory.createObject(3);


	if (IsDoor((int)room.location.z, (int)room.location.x, -1, 0)) {
		static_cast<PointShadowRenderer*>(object1->GetComponent("PointShadowRenderer"))->SetMesh("door01");
		object1->GetTransform()->SetRot(glm::vec3(0, M_PI / 2.0f, 0));
		object1->GetTransform()->SetScale(glm::vec3(0.01f, 3, room.size.x));
	}
	else {
		static_cast<PointShadowRenderer*>(object1->GetComponent("PointShadowRenderer"))->SetMesh("wall01");
		object1->GetTransform()->SetScale(glm::vec3(room.size.x, 3, 0.01f));
	}

	
	object1->GetTransform()->SetPos(glm::vec3(room.location.x, 1, room.location.z - room.size.z / 2));
	object1->InitComponents();
	em.GetGameEngine()->AddObject(object1);

	if (IsDoor((int)room.location.z, (int)room.location.x, 0, -1)) {
		static_cast<PointShadowRenderer*>(object2->GetComponent("PointShadowRenderer"))->SetMesh("door01");
		object2->GetTransform()->SetScale(glm::vec3(0.01f, 3, room.size.z));
	}
	else {
		static_cast<PointShadowRenderer*>(object2->GetComponent("PointShadowRenderer"))->SetMesh("wall01");
		object2->GetTransform()->SetRot(glm::vec3(0, M_PI / 2.0f, 0));
		object2->GetTransform()->SetScale(glm::vec3(room.size.z, 3, 0.01f));
	}

	object2->GetTransform()->SetPos(glm::vec3(room.location.x - room.size.x / 2, 1, room.location.z));
	object2->InitComponents();
	em.GetGameEngine()->AddObject(object2);

	if (IsDoor((int)room.location.z, (int)room.location.x, 1, 0)) {
		static_cast<PointShadowRenderer*>(object3->GetComponent("PointShadowRenderer"))->SetMesh("door01");
		object3->GetTransform()->SetRot(glm::vec3(0, M_PI / 2.0f, 0));
		object3->GetTransform()->SetScale(glm::vec3(0.01f, 3, room.size.x));
	}
	else {
		static_cast<PointShadowRenderer*>(object3->GetComponent("PointShadowRenderer"))->SetMesh("wall01");
		object3->GetTransform()->SetScale(glm::vec3(room.size.x, 3, 0.01f));
	}

	
	object3->GetTransform()->SetPos(glm::vec3(room.location.x, 1, room.location.z + room.size.z / 2));
	object3->InitComponents();
	em.GetGameEngine()->AddObject(object3);

	if (IsDoor((int)room.location.z, (int)room.location.x, 0, 1)) {
		static_cast<PointShadowRenderer*>(object4->GetComponent("PointShadowRenderer"))->SetMesh("door01");
		object4->GetTransform()->SetScale(glm::vec3(0.01f, 3, room.size.z));
	}
	else {
		static_cast<PointShadowRenderer*>(object4->GetComponent("PointShadowRenderer"))->SetMesh("wall01");
		object4->GetTransform()->SetRot(glm::vec3(0, M_PI / 2.0f, 0));
		object4->GetTransform()->SetScale(glm::vec3(room.size.z, 3, 0.01f));
	}

	object4->GetTransform()->SetPos(glm::vec3(room.location.x + room.size.x / 2, 1, room.location.z));
	object4->InitComponents();
	em.GetGameEngine()->AddObject(object4);
}
