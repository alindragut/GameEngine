#include <include/glm.h>
#include <time.h>
#include <list>
#include <GameEngine/IncludeList.h>
#include "Generator.h"
#include <GameEngine/Utils/ComponentFactory.h>
#include <GameEngine/Utils/DebugDraw.h>
#include <GameEngine/Utils/Delaunay/delaunay.h>
#include <GameEngine/Utils/MSTAlgorithm.h>
#include <GameEngine/Utils/AStarAlgorithm.h>
#include <GameEngine/MapGenerator/GeneratorManager.h>
#include <GameEngine/Components/Renderers/GameRenderers/TextureRenderer.h>

Generator::Generator(int roomCount, int locationMaxX, int locationMaxY, int sizeMaxX, int sizeMaxY) {

	this->roomCount = roomCount;
	this->locationMaxX = locationMaxX;
	this->locationMaxY = locationMaxY;
	this->sizeMaxX = sizeMaxX;
	this->sizeMaxY = sizeMaxY;

	GeneratorManager& gm = GeneratorManager::GetInstance();
	gm.SetGenerator(this);

	texture = new Texture2D();
	texture->Load2D("Source/GameEngine/Textures/floor.jpg", GL_REPEAT);

	mapMatrix = new int*[2 * locationMaxX + 10];
	for (int i = 0; i < 2 * locationMaxX + 10; i++) {
		mapMatrix[i] = new int [2 * locationMaxY + 10];
	}

	for (int i = 0; i < 2 * locationMaxX + 10; i++) {
		for (int j = 0; j < 2 * locationMaxY + 10; j++) {
			mapMatrix[i][j] = 1;
		}
	}
}

Generator::~Generator() {
	for (int i = 0; i < 2 * locationMaxX + 10; ++i)
		delete[] mapMatrix[i];
	delete[] mapMatrix;
	delete texture;
}

glm::vec3 Generator::GetSpawnPoint() {
	return rooms.front().location;
}


void Generator::PlaceRooms() {

	int index = 0;

	while (index < roomCount) {

		

		float randomx = rand() % (2 * locationMaxX) + 5;
		float randomy = rand() % (2 * locationMaxY) + 5;

		glm::vec3 location = glm::vec3(randomx, 0, randomy);

		randomx = 2 * (rand() % sizeMaxX) + 4;
		randomy = 2 * (rand() % sizeMaxY) + 4;

		glm::vec3 size = glm::vec3(randomx, 0.01f, randomy);

		bool add = true;
		Room* newRoom = new Room(location, size);
		for (Room room : rooms) {
			if (newRoom->Intersection(room, 1.25f)) {
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
					mapMatrix[j][i] = 0;
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
		for (auto room : rooms) {
			if (room.location.x == edge.first.first && room.location.z == edge.first.second) {
				size = room.size;
			}
		}
		PlaceCorridors(glm::vec3(edge.first.first, 0, edge.first.second), glm::vec3(edge.second.first, 0, edge.second.second), size);
	}

	for (auto room : rooms) {
		PlaceWall(room);
	}

	AStarAlgorithm* astar = new AStarAlgorithm(2 * locationMaxY + 10, 2 * locationMaxX + 10);
	/*astar->aStarSearch(&mapMatrix, std::make_pair((int)rooms.front().location.z,
		(int)rooms.front().location.x),
		std::make_pair((int)rooms.back().location.z,
		(int)rooms.back().location.x));*/

	for (int i = 0; i < 2 * locationMaxX + 10; i++) {
		for (int j = 0; j < 2 * locationMaxY + 10; j++) {
			printf("%d ", mapMatrix[i][j]);
		}
		printf("\n");
	}
}
	
void Generator::PlaceRoom(glm::vec3 location, glm::vec3 size) {

	ComponentFactory& factory = ComponentFactory::GetInstance();
	auto object = factory.createObject(8);

	static_cast<TextureRenderer*>(object->GetComponent("TextureRenderer"))->SetTexture(texture);
	static_cast<RigidBodyComponent*>(object->GetComponent("RigidBodyComponent"))->SetWalkable(true);

	object->GetTransform()->SetPos(location);
	object->GetTransform()->SetScale(size);
	object->InitComponents();

	EngineManager& em = EngineManager::GetInstance();

	em.GetGameEngine()->AddObject(object);
}

void Generator::PlaceCorridor(glm::vec3 location) {

	ComponentFactory& factory = ComponentFactory::GetInstance();
	auto object = factory.createObject(8);

	static_cast<TextureRenderer*>(object->GetComponent("TextureRenderer"))->SetTexture(texture);
	static_cast<RigidBodyComponent*>(object->GetComponent("RigidBodyComponent"))->SetWalkable(true);

	object->GetTransform()->SetPos(location);
	object->GetTransform()->SetScale(glm::vec3(1, 0, 1));
	object->InitComponents();

	EngineManager& em = EngineManager::GetInstance();

	em.GetGameEngine()->AddObject(object);
}

void Generator::PlaceCorridors(glm::vec3 crtPos, glm::vec3 finalPos, glm::vec3 size) {

	if (crtPos == finalPos) {
		return;
	}

	float leftDist = glm::l1Norm(finalPos - (crtPos - glm::vec3(1, 0, 0)));
	float rightDist = glm::l1Norm(finalPos - (crtPos + glm::vec3(1, 0, 0)));
	float bkdDist = glm::l1Norm(finalPos - (crtPos - glm::vec3(0, 0, 1)));
	float fwdDist = glm::l1Norm(finalPos - (crtPos + glm::vec3(0, 0, 1)));

	float upDist = glm::l2Norm(finalPos - (crtPos + glm::vec3(0, 1, 0)));
	float downDist = glm::l2Norm(finalPos - (crtPos - glm::vec3(0, 1, 0)));

	float min = leftDist;
	glm::vec3 next = crtPos - glm::vec3(1, 0, 0);

	if (rightDist < min) {
		min = rightDist;
		next = crtPos + glm::vec3(1, 0, 0);
	}

	if (bkdDist < min) {
		min = bkdDist;
		next = crtPos - glm::vec3(0, 0, 1);
	}

	if (fwdDist < min) {
		min = fwdDist;
		next = crtPos + glm::vec3(0, 0, 1);
	}
	Room* nextRoom = new Room(next, glm::vec3(1, 0, 1));
	bool toAdd = true;
	/*for (Room r : rooms) {
		if (r.Intersection(*nextRoom, 0.25f)) {
			toAdd = false;
			break;
		}
	}*/

	if (toAdd) {
		if (mapMatrix[(int)next.z][(int)next.x] == 1) {
			PlaceCorridor(next);
			mapMatrix[(int)next.z][(int)next.x] = 4;
		}
	}

	PlaceCorridors(next, finalPos, size);
}

void Generator::PlaceWalls(Room room) {

}

void Generator::PlaceWall(Room room) {
	ComponentFactory& factory = ComponentFactory::GetInstance();
	EngineManager& em = EngineManager::GetInstance();

	auto object1 = factory.createObject(8);
	auto object2 = factory.createObject(8);
	auto object3 = factory.createObject(8);
	auto object4 = factory.createObject(8);

	dynamic_cast<TextureRenderer*>(object1->GetComponent("TextureRenderer"))->SetTexture(texture);
	dynamic_cast<TextureRenderer*>(object2->GetComponent("TextureRenderer"))->SetTexture(texture);
	dynamic_cast<TextureRenderer*>(object3->GetComponent("TextureRenderer"))->SetTexture(texture);
	dynamic_cast<TextureRenderer*>(object4->GetComponent("TextureRenderer"))->SetTexture(texture);

	glm::vec2 l = glm::vec2(room.location.x - room.size.x / 2.f, room.location.z + room.size.z / 2.f);
	glm::vec2 r = glm::vec2(room.location.x + room.size.x / 2.f, room.location.z - room.size.z / 2.f);
	glm::vec2 t = glm::vec2(room.location.x + room.size.x / 2.f, room.location.z + room.size.z / 2.f);
	glm::vec2 b = glm::vec2(room.location.x - room.size.x / 2.f, room.location.z - room.size.z / 2.f);

	object1->GetTransform()->SetScale(glm::vec3(room.size.x, 3, 0.01f));
	object1->GetTransform()->SetPos(glm::vec3(room.location.x, 1, room.location.z - room.size.z / 2));
	object1->InitComponents();
	em.GetGameEngine()->AddObject(object1);

	object2->GetTransform()->SetScale(glm::vec3(0.01f, 3, room.size.z));
	object2->GetTransform()->SetPos(glm::vec3(room.location.x - room.size.x / 2, 1, room.location.z));
	object2->InitComponents();
	em.GetGameEngine()->AddObject(object2);

	object3->GetTransform()->SetScale(glm::vec3(room.size.x, 3, 0.01f));
	object3->GetTransform()->SetPos(glm::vec3(room.location.x, 1, room.location.z + room.size.z / 2));
	object3->InitComponents();
	em.GetGameEngine()->AddObject(object3);

	object4->GetTransform()->SetScale(glm::vec3(0.01f, 3, room.size.z));
	object4->GetTransform()->SetPos(glm::vec3(room.location.x + room.size.x / 2, 1, room.location.z));
	object4->InitComponents();
	em.GetGameEngine()->AddObject(object4);
}
