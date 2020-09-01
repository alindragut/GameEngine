#include <include/glm.h>
#include <time.h>
#include <list>
#include <GameEngine/IncludeList.h>
#include "Generator.h"
#include <GameEngine/Utils/ComponentFactory.h>
#include <GameEngine/Utils/DebugDraw.h>
#include <GameEngine/MapGenerator/GeneratorManager.h>
#include <GameEngine/Components/NPC/NPCSpawner.h>
#include <Core/GPU/MeshInstanced.h>
#include <GameEngine/Utils/MeshManager.h>

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
	
	for (Room room : rooms) {
		if (room.location != rooms.front().location) {
			int type = rand() % (3 - 2 + 1) + 2;
			
			float randX = rand() % (int)(room.size.x / 2);
			NPCSpawner::SpawnNPC(player, room.location + glm::vec3(0, 0.5f, 0), type);
			
		}
		
	}
}

glm::vec3 Generator::GetSpawnPoint() {
	return rooms.front().location;
}


void Generator::PlaceRooms() {

	int index = 0;

	while (index < roomCount) {

		float randomx = 2 * rand() % (locationMaxX) + 6;
		float randomy = 2 * rand() % (locationMaxY) + 6;


		glm::vec3 location = glm::vec3(randomx, -0.5f, randomy);

		randomx = 2 * (rand() % sizeMaxX) + 6;
		randomy = 2 * (rand() % sizeMaxY) + 6;

		glm::vec3 size = glm::vec3(randomx, 1, randomy);

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
			//PlaceRoom(newRoom->location, newRoom->size);
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
		PlaceColumns(room);
		PlaceDecorations(room);
	}

	for (int i = 0; i < 2 * locationMaxX + 10; i++) {
		for (int j = 0; j < 2 * locationMaxY + 10; j++) {
			for (int k = 0; k < navSizeMult; k++) {
				for (int l = 0; l < navSizeMult; l++) {
					mapMatrix[i * navSizeMult + k][j * navSizeMult + l] = auxMapMatrix[i][j];
				}
			}
		}
	}
	int startI = -1, startJ = -1;
	for (int i = 0; i < (2 * locationMaxX + 10); i++) {
		for (int j = 0; j < (2 * locationMaxY + 10); j++) {
			if (auxMapMatrix[i][j] != 1) {
				int val = GetWallDirection(auxMapMatrix, i, j);
				if (val != 5) {
					PlaceWall(glm::vec3(j, 0, i), val, auxMapMatrix[i][j], GetCorridorType(auxMapMatrix, i, j), GetWallOffset(auxMapMatrix, i, j));
				}
				PlaceFloor(glm::vec3(j, 0, i));
			}
		}
	}

	//glm::mat4 model = glm::translate(glm::mat4(1),glm::vec3(0.5, -0.5, 0.5)) * glm::rotate(glm::mat4(1), float(-M_PI_2), glm::vec3(1, 0, 0));

	//static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("wall01"))->AddInstance(model);

	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("fence01"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("fence03"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("wall01"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("floor01"))->Reconstruct();

	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("column"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("column_broken"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("banner"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("banner_wall"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("barrel"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("barrel_2"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("bones"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("bones_2"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("book"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("book_2"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("book_3"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("bucket"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("candelabrum_tall"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("candle"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("carpet"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("chair"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("chest"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("cobweb"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("cobweb_2"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("crate"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("pedestal"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("pedestal_2"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("potion"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("potion_2"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("potion_3"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("potion_4"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("potion_5"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("potion_6"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("skull"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("table_big"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("table_small"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("torch_wall"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("vase"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("window"))->Reconstruct();
	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("woodfire"))->Reconstruct();

	startI = -1, startJ = -1;

	for (int i = 0; i < (2 * locationMaxX + 10) * navSizeMult; i++) {
		for (int j = 0; j < (2 * locationMaxY + 10) * navSizeMult; j++) {
			if (mapMatrix[i][j] != 1) {
				startI = i;
				startJ = j;
				break;
			}
		}
		if (startI != -1) {
			break;
		}
	}

	if (startI != -1) {
		while (mapMatrix[startI][startJ] != 5) {
			mapMatrix[startI][startJ] = 5;

			int max = -1;
			int index = 0;
			int up = -1; 
			int down = -1;
			int left = -1; 
			int right = -1;

			if (mapMatrix[startI - 1][startJ] != 1 && mapMatrix[startI - 1][startJ] != 5) {
				up = GetNumberOfInvalidNeighbours(mapMatrix, startI - 1, startJ);
			}
			if (mapMatrix[startI + 1][startJ] != 1 && mapMatrix[startI + 1][startJ] != 5) {
				down = GetNumberOfInvalidNeighbours(mapMatrix, startI + 1, startJ);
			}
			if (mapMatrix[startI][startJ - 1] != 1 && mapMatrix[startI][startJ - 1] != 5) {
				left = GetNumberOfInvalidNeighbours(mapMatrix, startI, startJ - 1);;
			}
			if (mapMatrix[startI][startJ + 1] != 1 && mapMatrix[startI][startJ + 1] != 5) {
				right = GetNumberOfInvalidNeighbours(mapMatrix, startI, startJ + 1);;
			}

			if (down > max) {
				max = down;
				index = 1;
			}
			if (left > max) {
				max = left;
				index = 2;
			}
			if (right > max) {
				max = right;
				index = 3;
			}
			if (up > max) {
				max = up;
				index = 4;
			}

			if (max == 0) {
				break;
			}

			if (index == 1) {
				startI += 1;
			}
			else if (index == 2) {
				startJ -= 1;
			}
			else if (index == 3) {
				startJ += 1;
			}
			else if (index == 4) {
				startI -= 1;
			}
		}
	}
	

}

int Generator::GetCorridorType(int **mat, int i, int j) {
	if (mat[i][j] != 4) {
		return 0;
	}

	if ((mat[i + 1][j] == 1 && mat[i][j - 1] == 1) || (mat[i + 1][j] == 1 && mat[i][j + 1] == 1) || (mat[i - 1][j] == 1 && mat[i][j - 1] == 1) || (mat[i - 1][j] == 1 && mat[i][j + 1] == 1)) {
		return 4;
	}

	if (mat[i + 1][j] == 0 || mat[i - 1][j] == 0 || mat[i][j - 1] == 0 || mat[i][j + 1] == 0) {
		return 2;
	}

	if (mat[i][j] == 4 && mat[i - 1][j] == 4 && mat[i + 1][j] == 4 && mat[i][j - 1] == 4 && mat[i][j + 1] == 4) {
		return 3;
	}

	return 1;
}

int Generator::GetWallDirection(int **mat, int i, int j) {
	/* numpad */
	
	if (mat[i][j] == 4 && mat[i - 1][j] == 4 && mat[i + 1][j] == 4 && mat[i][j - 1] == 4 && mat[i][j + 1] == 4) {
		if (mat[i - 1][j - 1] == 1) {
			return 3;
		}

		if (mat[i - 1][j + 1] == 1) {
			return 1;
		}

		if (mat[i + 1][j - 1] == 1) {
			return 9;
		}

		if (mat[i + 1][j + 1] == 1) {
			return 7;
		}
	}

	if (mat[i][j] == 4) {
		if (mat[i - 1][j] == 1 && mat[i - 1][j - 1] == 4) {
			return 5;
		}
		if (mat[i + 1][j] == 1 && mat[i + 1][j + 1] == 4) {
			return 5;
		}
		if (mat[i][j - 1] == 1 && mat[i + 1][j - 1] == 4) {
			return 5;
		}
		if (mat[i][j + 1] == 1 && mat[i - 1][j + 1] == 4) {
			return 5;
		}
	}



	if (mat[i - 1][j] == 1 && mat[i][j - 1] == 1) {
		// NW
		return 7;
	}
	if (mat[i - 1][j] == 1 && mat[i][j + 1] == 1) {
		// NE
		return 9;
	}
	if (mat[i + 1][j] == 1 && mat[i][j - 1] == 1) {
		// SW
		return 1;
	}
	if (mat[i + 1][j] == 1 && mat[i][j + 1] == 1) {
		// SE
		return 3;
	}
	if (mat[i - 1][j] == 1) {
		// N
		return 8;
	}
	if (mat[i + 1][j] == 1) {
		// S
		return 2;
	}
	if (mat[i][j - 1] == 1) {
		// W
		return 4;
	}
	if (mat[i][j + 1] == 1) {
		// E
		return 6;
	}
	// center
	return 5;
}

bool Generator::IsFenceEnding(int i, int j, int dir) {
	if ((dir == 2 || dir == 8) && (auxMapMatrix[i][j - 2] == 0 || auxMapMatrix[i][j + 2] == 0)) {
		return true;
	}

	if ((dir == 4 || dir == 6) && (auxMapMatrix[i - 2][j] == 0 || auxMapMatrix[i + 2][j] == 0)) {
		return true;
	}

	return false;
}

glm::vec3 Generator::GetWallOffset(int **mat, int i, int j) {

	if (mat[i][j] != 0) {
		return glm::vec3(0);
	}

	if (mat[i + 1][j + 1] == 4 && mat[i + 1][j] == 1) {
		return glm::vec3(-0.5, 0, 0);
	}

	/*if (mat[i + 1][j - 1] == 4 && mat[i + 1][j] == 1) {
		return 0.f * glm::vec3(0, 1, 0);
	}*/

	/*if (mat[i + 1][j + 1] == 4 && mat[i][j + 1] == 1) {
		return 0.f * glm::vec3(0, 1, 0);
	}*/

	if (mat[i - 1][j + 1] == 4 && mat[i][j + 1] == 1) {
		return glm::vec3(0, 0, 0.5);
	}

	if (mat[i - 1][j - 1] == 4 && mat[i - 1][j] == 1) {
		return glm::vec3(0.5, 0, 0);
	}

	/*if (mat[i - 1][j + 1] == 4 && mat[i - 1][j] == 1) {
		return 0.f * glm::vec3(0, 1, 0);
	}*/

	if (mat[i + 1][j - 1] == 4 && mat[i][j - 1] == 1) {
		return glm::vec3(0, 0, -0.5);
	}

	/*if ((mat[i - 1][j - 1] == 4 && mat[i][j - 1] == 1)) {
		return  0.f * glm::vec3(0, 1.f, 0);
	}*/

	/*if (mat[i + 1][j] == 4 && mat[i + 1][j - 1] == 1) {
		return glm::vec3(0, 2, 0);
	}
	if (mat[i + 1][j] == 4 && mat[i + 1][j + 1] == 1) {
		return glm::vec3(0, 2, 0);
	}
	if (mat[i][j + 1] == 4 && mat[i - 1][j + 1] == 1) {
		return glm::vec3(0, 2, 0);
	}
	if (mat[i][j + 1] == 4 && mat[i + 1][j + 1] == 1) {
		return glm::vec3(0, 2, 0);
	}
	if (mat[i - 1][j] == 4 && mat[i - 1][j + 1] == 1) {
		return glm::vec3(0, 2, 0);
	}
	if (mat[i - 1][j] == 4 && mat[i - 1][j - 1] == 1) {
		return glm::vec3(0, 2, 0);
	}
	if (mat[i][j - 1] == 4 && mat[i - 1][j - 1] == 1) {
		return glm::vec3(0, 2, 0);
	}
	if (mat[i][j - 1] == 4 && mat[i + 1][j - 1] == 1) {
		return glm::vec3(0, 2, 0);
	}*/
	return glm::vec3(0);
}

int Generator::GetNumberOfInvalidNeighbours(int **mat, int i, int j) {
	int count = 0;

	if (mat[i - 1][j - 1] == 1) {
		count++;
	}
	if (mat[i - 1][j] == 1) {
		count++;
	}
	if (mat[i - 1][j + 1] == 1) {
		count++;
	}
	if (mat[i][j - 1] == 1) {
		count++;
	}
	if (mat[i][j + 1] == 1) {
		count++;
	}
	if (mat[i + 1][j - 1] == 1) {
		count++;
	}
	if (mat[i + 1][j] == 1) {
		count++;
	}
	if (mat[i + 1][j + 1] == 1) {
		count++;
	}
	return count;
}

int Generator::GetNumberOfValidNeighbours(int **mat, int i, int j) {
	int count = 0;

	if (mat[i - 1][j - 1] != 1 && mat[i - 1][j - 1] != 5) {
		count++;
	}
	if (mat[i - 1][j] != 1 && mat[i - 1][j] != 5) {
		count++;
	}
	if (mat[i - 1][j + 1] != 1 && mat[i - 1][j + 1] != 5) {
		count++;
	}
	if (mat[i][j - 1] != 1 && mat[i][j - 1] != 5) {
		count++;
	}
	if (mat[i][j + 1] != 1 && mat[i][j + 1] != 5) {
		count++;
	}
	if (mat[i + 1][j - 1] != 1 && mat[i + 1][j - 1] != 5) {
		count++;
	}
	if (mat[i + 1][j] != 1 && mat[i + 1][j] != 5) {
		count++;
	}
	if (mat[i + 1][j + 1] != 1 && mat[i + 1][j + 1] != 5) {
		count++;
	}
	return count;
}

bool Generator::VerifyFinalCorridorPos(int i, int j, int dir) {
	if (auxMapMatrix[i][j] == 0) {
		return false;
	}

	if ((dir == 2 || dir == 8) && (auxMapMatrix[i][j - 1] == 0 || auxMapMatrix[i][j + 1] == 0)) {
		return false;
	}

	if ((dir == 4 || dir == 6) && (auxMapMatrix[i - 1][j] == 0 || auxMapMatrix[i + 1][j] == 0)) {
		return false;
	}

	return true;
}

void Generator::PlaceWall(glm::vec3 pos, int dir, int type, int corridorType, glm::vec3 wallOffset) {
	glm::vec3 objPos = pos;
	glm::vec3 objRot = glm::vec3(0);
	glm::vec3 objScale = glm::vec3(1);

	if (dir == 8) {
		objRot = glm::vec3(0, -M_PI_2, 0);
	}

	if (dir == 7) {
		if (type == 4 && corridorType != 4) {
			objPos += glm::vec3(1, 0, 1);
		}
	}
	
	if (dir == 9) {
		objPos += glm::vec3(1, 0, 0);
		objRot = glm::vec3(0, -M_PI_2, 0);
		if (type == 4 && corridorType != 4) {
			objPos = pos + glm::vec3(0, 0, 1);
		}
	}

	if (dir == 2) {
		objPos += glm::vec3(1, 0, 1);
		objRot = glm::vec3(0, M_PI_2, 0);

	}
		
	if (dir == 3) {
		objRot = glm::vec3(0, M_PI, 0);
		objPos += glm::vec3(1, 0, 1);
		if (type == 4 && corridorType != 4) {
			objPos = pos;
		}
	}

	if (dir == 1) {
		objRot = glm::vec3(0, M_PI_2, 0);
		objPos += glm::vec3(0, 0, 1);
		if (type == 4 && corridorType != 4) {
			objPos = pos + glm::vec3(1, 0, 0);

		}
	}

	if (dir == 6) {
		objPos += glm::vec3(1, 0, 0);
		objRot = glm::vec3(0, M_PI, 0);
	}

	if (dir == 4) {
		objPos += glm::vec3(0, 0, 1);
	}

	if (dir % 2 == 1) {
		wallOffset = glm::vec3(0);
	}

	if (wallOffset != glm::vec3(0)) {
		objScale = glm::vec3(1, 1, 0.5);
	}

	objPos += glm::vec3(0, 1, 0) + wallOffset;
	

	if (type == 0) {
		objScale += glm::vec3(0.625, 0, 0);
		glm::mat4 model = glm::translate(glm::mat4(1), objPos)  * glm::mat4(glm::quat(objRot)) * glm::scale(glm::mat4(1), objScale);

		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("wall01"))->AddInstance(model);
	}
	else if (type == 4) {
		//intersection
		if (dir % 2 != 1) {
			if (!VerifyFinalCorridorPos(int(objPos.z), int(objPos.x), dir)) {
				return;
			}
		}
		objRot += glm::vec3(-M_PI_2, M_PI_2, 0);
		if (corridorType >= 3) {
			//intersection that needs flip
			//if (corridorType == 4) {
			objRot += glm::vec3(0, M_PI, 0);
			glm::mat4 model = glm::translate(glm::mat4(1), objPos)  * glm::mat4(glm::quat(objRot)) * glm::scale(glm::mat4(1), objScale);
			static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("fence03"))->AddInstance(model);
			//}
		}
		else {
			/*glm::vec3 finalPos = object->GetTransform()->GetPos();
			if (IsFenceEnding(int(finalPos.z), int(finalPos.x), dir)) {
				static_cast<PointShadowRenderer*>(object->GetComponent("PointShadowRenderer"))->SetMesh("fence02");
				if (dir == 2 || dir == 6) {
					object->GetTransform()->SetRot(object->GetTransform()->GetRot() + glm::vec3(0, M_PI, 0));
				}
				object->GetTransform()->SetScale(glm::vec3(1, 1, 5));
			}
			else {*/
			glm::mat4 model = glm::translate(glm::mat4(1), objPos)  * glm::mat4(glm::quat(objRot)) * glm::scale(glm::mat4(1), objScale);
			static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("fence01"))->AddInstance(model);
			//}
		}
		//start/end of corridor
		
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
	//printf("from %d %d\n", GetNavMatrixValue(from), GetNavMatrixValue(from, 0));

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

	PlaceCorridor(next, 2);
	next = next + 2.0f * glm::normalize(next - crtPos);
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

void Generator::PlaceFloor(glm::vec3 pos) {
	//ComponentFactory& factory = ComponentFactory::GetInstance();
	//EngineManager& em = EngineManager::GetInstance();

	//auto object = factory.createObject(3);
	//static_cast<PointShadowRenderer*>(object->GetComponent("PointShadowRenderer"))->SetMesh("floor01");
	//static_cast<RigidBodyComponent*>(object->GetComponent("RigidBodyComponent"))->SetWalkable(true);
	//object->GetTransform()->SetPos(pos + glm::vec3(0.5, -0.5, 0.5));
	//object->GetTransform()->SetRot(glm::vec3(-M_PI_2, 0, 0));

	//em.GetGameEngine()->AddObject(object);

	glm::mat4 model = glm::translate(glm::mat4(1), pos + glm::vec3(0.5, -0.5, 0.5)) * glm::rotate(glm::mat4(1), float(-M_PI_2), glm::vec3(1, 0, 0));

	static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("floor01"))->AddInstance(model);
}

void Generator::PlaceCorridor(glm::vec3 location, float size) {
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

std::pair<glm::vec3, glm::vec3> Generator::IsDoor(glm::vec3 from, glm::vec3 to, bool *isDoor) {
	glm::vec3 start, finish, dir, current;
	bool foundStart = false;
	dir = normalize(to - from);
	current = from;
	start = glm::vec3(0);
	finish = glm::vec3(0);

	while (current != (to)) {
		if (!foundStart) {
			if (auxMapMatrix[(int)current.z][(int)current.x] == 4) {
				foundStart = true;
				start = current;
				finish = current;
			}
		}
		else {
			if (auxMapMatrix[(int)current.z][(int)current.x] != 4) {
				break;
			}
			finish = current;
		}
		current += dir;
	}

	*isDoor = foundStart;

	return std::pair<glm::vec3, glm::vec3>(start, finish);
}

void Generator::PlaceColumns(Room room) {

	int res = rand() % 2;

	if (res == 0) {
		glm::vec3 scale = glm::vec3(0.5f, 0.5f, 2);
		glm::vec3 rot = glm::vec3(3 * M_PI / 2, 0, 0);

		glm::vec3 loc = room.location + glm::vec3(room.size.x / 4, 1.5f, room.size.z / 4);
		glm::mat4 model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("column"))->AddInstance(model);
		

		loc = room.location + glm::vec3(-room.size.x / 4, 1.5f, room.size.z / 4);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("column"))->AddInstance(model);

		loc = room.location + glm::vec3(room.size.x / 4, 1.5f, -room.size.z / 4);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("column"))->AddInstance(model);

		loc = room.location + glm::vec3(-room.size.x / 4, 1.5f, -room.size.z / 4);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("column"))->AddInstance(model);
	}
	else {
		return;
	}
	
}

void Generator::PlaceDecorations(Room room) {

	glm::vec3 loc;
	glm::vec3 rot;
	glm::vec3 scale;
	glm::mat4 model;
	glm::vec3 offset = glm::vec3(0);
	
	// Table with chairs and decorations - DONE
	/*{
		loc = room.location + glm::vec3(0, 0.37f, 0) + offset;
		rot = glm::vec3(0, M_PI / 2, M_PI / 2);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("carpet"))->AddInstance(model);

		loc = room.location + glm::vec3(0, 0.5f, 0) + offset;
		rot = glm::vec3(-M_PI_2, -M_PI_2, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("table_big"))->AddInstance(model);

		loc = room.location + glm::vec3(-0.5f, 1.3f, 0) + offset;
		rot = glm::vec3(-M_PI_2, 0, 0);
		scale = glm::vec3(0.5f);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("potion_3"))->AddInstance(model);

		loc = room.location + glm::vec3(0.5f, 1.3f, 0) + offset;
		rot = glm::vec3(-M_PI_2, 0, 0);
		scale = glm::vec3(0.5f);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("book"))->AddInstance(model);

		loc = room.location + glm::vec3(1.5f, 0.5f, 0) + offset;
		rot = glm::vec3(-M_PI_2, -M_PI_2,0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("chair"))->AddInstance(model);

		loc = room.location + glm::vec3(-1.5f, 0.5f, 0) + offset;
		rot = glm::vec3(-M_PI_2, M_PI_2, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("chair"))->AddInstance(model);
	}*/

	// Windows - DONE
	{
		loc = room.location + glm::vec3(room.size.x/2, 1.5f, room.size.z/4);
		rot = glm::vec3(-M_PI_2, M_PI, 0);
		scale = glm::vec3(0.5f, 1, 1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("window"))->AddInstance(model);

		loc = room.location + glm::vec3(room.size.x / 2, 1.5f, -room.size.z / 4);
		rot = glm::vec3(-M_PI_2, M_PI, 0);
		scale = glm::vec3(0.5f, 1, 1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("window"))->AddInstance(model);

		loc = room.location + glm::vec3(-room.size.x / 2, 1.5f, room.size.z / 4);
		rot = glm::vec3(-M_PI_2, 0, 0);
		scale = glm::vec3(0.5f, 1, 1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("window"))->AddInstance(model);

		loc = room.location + glm::vec3(-room.size.x / 2, 1.5f, -room.size.z / 4);
		rot = glm::vec3(-M_PI_2, 0, 0);
		scale = glm::vec3(0.5f, 1, 1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("window"))->AddInstance(model);
	}

	//Skeletons - DONE
	/*{
		loc = room.location + glm::vec3(1, 0.5f, 0) + offset;
		rot = glm::vec3(-M_PI_2, 0, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("bones"))->AddInstance(model);

		loc = room.location + glm::vec3(-1, 0.5f, 0) + offset;
		rot = glm::vec3(-M_PI_2, 0, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("bones_2"))->AddInstance(model);

		loc = room.location + glm::vec3(1, 0.5f, 1) + offset;
		rot = glm::vec3(-M_PI_2, 0, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("skull"))->AddInstance(model);

	}*/

	// Woodfire - DONE
	/*{
		loc = room.location + glm::vec3(0, 0.5f, 0) + offset;
		rot = glm::vec3(-M_PI_2, 0, 0);
		scale = glm::vec3(1.2f);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("woodfire"))->AddInstance(model);
	}*/

	// Torch Wall - DONE
	/*{
		loc = room.location + glm::vec3(room.size.x / 2, 1.5f, room.size.z / 4);
		rot = glm::vec3(-M_PI_2, M_PI, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("torch_wall"))->AddInstance(model);

		loc = room.location + glm::vec3(room.size.x / 2, 1.5f, -room.size.z / 4);
		rot = glm::vec3(-M_PI_2, M_PI, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("torch_wall"))->AddInstance(model);

		loc = room.location + glm::vec3(-room.size.x / 2, 1.5f, room.size.z / 4);
		rot = glm::vec3(-M_PI_2, 0, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("torch_wall"))->AddInstance(model);

		loc = room.location + glm::vec3(-room.size.x / 2, 1.5f, -room.size.z / 4);
		rot = glm::vec3(-M_PI_2, 0, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("torch_wall"))->AddInstance(model);
	}*/

	// Barrel left- DONE
	/*{
		loc = room.location + glm::vec3(room.size.x / 2 - 0.5f, 0.5f, room.size.z / 4);
		rot = glm::vec3(-M_PI_2, M_PI, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("barrel"))->AddInstance(model);

		loc = room.location + glm::vec3(room.size.x / 2 - 0.5f, 0.5f, 0);
		rot = glm::vec3(-M_PI_2, M_PI, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("barrel"))->AddInstance(model);

		loc = room.location + glm::vec3(room.size.x / 2 - 0.5f, 0.5f, -room.size.z / 4);
		rot = glm::vec3(-M_PI_2, M_PI, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("barrel"))->AddInstance(model);

	}

	// Barrel right- DONE

	{
		loc = room.location + glm::vec3(-room.size.x / 2 + 0.5f, 0.5f, room.size.z / 4);
		rot = glm::vec3(-M_PI_2, 0, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("barrel"))->AddInstance(model);

		loc = room.location + glm::vec3(-room.size.x / 2 + 0.5f, 0.5f, 0);
		rot = glm::vec3(-M_PI_2, M_PI, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("barrel"))->AddInstance(model);

		loc = room.location + glm::vec3(-room.size.x / 2 + 0.5f, 0.5f, -room.size.z / 4);
		rot = glm::vec3(-M_PI_2, 0, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("barrel"))->AddInstance(model);
	}

	// Barrel front
	{
		loc = room.location + glm::vec3(room.size.x / 4, 0.5f, room.size.z / 2 - 0.5f);
		rot = glm::vec3(-M_PI_2, 0, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("barrel"))->AddInstance(model);

		loc = room.location + glm::vec3(0, 0.5f, room.size.z / 2 - 0.5f);
		rot = glm::vec3(-M_PI_2, M_PI, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("barrel"))->AddInstance(model);

		loc = room.location + glm::vec3(-room.size.x / 4, 0.5f, room.size.z / 2 - 0.5f);
		rot = glm::vec3(-M_PI_2, 0, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("barrel"))->AddInstance(model);
	}

	// Barrel back
	{
		loc = room.location + glm::vec3(room.size.x / 4, 0.5f, -room.size.z / 2 + 0.5f);
		rot = glm::vec3(-M_PI_2, 0, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("barrel"))->AddInstance(model);

		loc = room.location + glm::vec3(0, 0.5f, -room.size.z / 2 + 0.5f);
		rot = glm::vec3(-M_PI_2, M_PI, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("barrel"))->AddInstance(model);

		loc = room.location + glm::vec3(-room.size.x / 4, 0.5f, -room.size.z / 2 + 0.5f);
		rot = glm::vec3(-M_PI_2, 0, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("barrel"))->AddInstance(model);
	}*/

	// Candelabrum
	/*{
		loc = room.location + glm::vec3(room.size.x / 4, 0.5f, room.size.z / 2 - 0.5f);
		rot = glm::vec3(-M_PI_2, M_PI, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("candelabrum_tall"))->AddInstance(model);

		loc = room.location + glm::vec3(room.size.x / 4, 0.5f, -room.size.z / 2 + 0.5f);
		rot = glm::vec3(-M_PI_2, M_PI, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("candelabrum_tall"))->AddInstance(model);

		loc = room.location + glm::vec3(-room.size.x / 4, 0.5f, room.size.z / 2 - 0.5f);
		rot = glm::vec3(-M_PI_2, 0, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("candelabrum_tall"))->AddInstance(model);

		loc = room.location + glm::vec3(-room.size.x / 4, 0.5f, -room.size.z / 2 + 0.5f);
		rot = glm::vec3(-M_PI_2, 0, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("candelabrum_tall"))->AddInstance(model);
	} */

	// cobweb left- DONE
	/*{
		loc = room.location + glm::vec3(room.size.x / 2 - 0.5f, 0.5f, room.size.z / 4);
		rot = glm::vec3(-M_PI_2, -M_PI_2, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("cobweb"))->AddInstance(model);

		loc = room.location + glm::vec3(room.size.x / 2 - 0.5f, 0.5f, 0);
		rot = glm::vec3(-M_PI_2, -M_PI_2, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("cobweb"))->AddInstance(model);

		loc = room.location + glm::vec3(room.size.x / 2 - 0.5f, 0.5f, -room.size.z / 4);
		rot = glm::vec3(-M_PI_2, -M_PI_2, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("cobweb"))->AddInstance(model);

	}

	// cobweb right- DONE

	{
		loc = room.location + glm::vec3(-room.size.x / 2 + 0.5f, 0.5f, room.size.z / 4);
		rot = glm::vec3(-M_PI_2, M_PI_2, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("cobweb"))->AddInstance(model);

		loc = room.location + glm::vec3(-room.size.x / 2 + 0.5f, 0.5f, 0);
		rot = glm::vec3(-M_PI_2, M_PI_2, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("cobweb"))->AddInstance(model);

		loc = room.location + glm::vec3(-room.size.x / 2 + 0.5f, 0.5f, -room.size.z / 4);
		rot = glm::vec3(-M_PI_2, M_PI_2, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("cobweb"))->AddInstance(model);
	}

	// cobweb front
	{
		loc = room.location + glm::vec3(room.size.x / 4, 0.5f, room.size.z / 2 - 0.5f);
		rot = glm::vec3(-M_PI_2, M_PI, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("cobweb"))->AddInstance(model);

		loc = room.location + glm::vec3(0, 0.5f, room.size.z / 2 - 0.5f);
		rot = glm::vec3(-M_PI_2, M_PI, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("cobweb"))->AddInstance(model);

		loc = room.location + glm::vec3(-room.size.x / 4, 0.5f, room.size.z / 2 - 0.5f);
		rot = glm::vec3(-M_PI_2, M_PI, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("cobweb"))->AddInstance(model);
	}

	// cobweb back
	{
		loc = room.location + glm::vec3(room.size.x / 4, 0.5f, -room.size.z / 2 + 0.5f);
		rot = glm::vec3(-M_PI_2, 0, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("cobweb"))->AddInstance(model);

		loc = room.location + glm::vec3(0, 0.5f, -room.size.z / 2 + 0.5f);
		rot = glm::vec3(-M_PI_2, 0, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("cobweb"))->AddInstance(model);

		loc = room.location + glm::vec3(-room.size.x / 4, 0.5f, -room.size.z / 2 + 0.5f);
		rot = glm::vec3(-M_PI_2, 0, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("cobweb"))->AddInstance(model);
	}*/

	// cobweb corners - DONE
	/*{
		loc = room.location + glm::vec3(room.size.x / 2 - 0.5f, 0.5f, room.size.z / 2 - 0.5f);
		rot = glm::vec3(-M_PI_2, M_PI/6 + M_PI, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("cobweb"))->AddInstance(model);

		loc = room.location + glm::vec3(room.size.x / 2 - 0.5f, 0.5f, -room.size.z / 2 + 0.5f);
		rot = glm::vec3(-M_PI_2, -M_PI/6, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("cobweb"))->AddInstance(model);

		loc = room.location + glm::vec3(-room.size.x / 2 + 0.5f, 0.5f, room.size.z / 2 - 0.5f);
		rot = glm::vec3(-M_PI_2, -M_PI/6 + M_PI, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("cobweb"))->AddInstance(model);

		loc = room.location + glm::vec3(-room.size.x / 2 + 0.5f, 0.5f, -room.size.z / 2 + 0.5f);
		rot = glm::vec3(-M_PI_2, M_PI/6, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("cobweb"))->AddInstance(model);
	}*/

	// crate
	{
		loc = room.location + glm::vec3(0, 0.5f,0);
		rot = glm::vec3(-M_PI_2,0, 0);
		scale = glm::vec3(0.5f);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		//static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("crate"))->AddInstance(model);
	}

	// barrel rand
	{
		loc = room.location + glm::vec3(0, 0.5f, 0);
		rot = glm::vec3(-M_PI_2, 0, 0);
		scale = glm::vec3(1);
		model = glm::translate(glm::mat4(1), loc) * glm::mat4(glm::quat(rot)) * glm::scale(glm::mat4(1), scale);
		//static_cast<MeshInstanced*>(MeshManager::GetInstance().GetMesh("barrel"))->AddInstance(model);
	}
}