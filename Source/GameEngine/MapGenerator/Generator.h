#pragma once
#include <include/glm.h>
#include <time.h>
#include <list>
#include <stack>
#include <GameEngine/IncludeList.h>
#include <GameEngine/Utils/Delaunay/delaunay.h>
#include <GameEngine/Utils/MSTAlgorithm.h>
#include <GameEngine/Utils/AStarAlgorithm.h>

class Room {

public:
	glm::vec3 location;
	glm::vec3 size;

	Room(glm::vec3 crtLoc, glm::vec3 crtSize) {
		location = crtLoc;
		size = crtSize;
	}

	bool Intersection(Room a, float offset) {
		glm::vec2 l1 = glm::vec2(location.x - size.x / 2.f, location.z + size.z / 2.f);
		glm::vec2 l2 = glm::vec2(a.location.x - a.size.x / 2.f * offset, a.location.z + a.size.z / 2.f * offset);
		glm::vec2 r1 = glm::vec2(location.x + size.x / 2.f, location.z - size.z / 2.f);
		glm::vec2 r2 = glm::vec2(a.location.x + a.size.x / 2.f * offset, a.location.z - a.size.z / 2.f * offset);

		return !((l1.x >= r2.x || l2.x >= r1.x) || (l1.y <= r2.y || l2.y <= r1.y));
	}
};

class Generator{

public:

	Generator(int roomCount, int locationMaxX, int locationMaxY, int sizeMaxX, int sizeMaxY, int navigationMatrixSizeMultiplier);
	~Generator();
	void Init();

	void PlaceRooms();
	glm::vec3 GetSpawnPoint();
	int** GetMatrix() { return mapMatrix; }
	void SetPlayer(BaseGameObject* player) { this->player = player; }
	std::stack<std::pair<float, float>> GetPath(glm::vec3 from, glm::vec3 to);
	int GetNavMatrixValue(glm::vec3 pos, int mode = 1);

private:
	glm::vec3 GetCorridorBoundary(Room* r1, Room* r2);
	int CalcLinearInterp(float nr, int mode = 1);
	
	int roomCount;

	int locationMaxX;
	int locationMaxY;
	int sizeMaxX;
	int sizeMaxY;
	int navSizeMult;

	int** mapMatrix;
	int** auxMapMatrix;

	AStarAlgorithm* astar;

	std::list<Room> rooms;
	Texture2D* texture;

	BaseGameObject* player;

	void PlaceRoom(glm::vec3 location, glm::vec3 size);
	void PlaceCorridor(glm::vec3 location, float size);
	void PlaceCorridors(glm::vec3 crtPos, glm::vec3 finalPos, glm::vec3 size);
	void PlaceWalls(Room room);
	void PlaceWall(Room room);

	bool IsDoor(int x, int y, int dirX, int dirY) {
		while (mapMatrix[x][y] == 0) {
			x += dirX;
			y += dirY;
			if (mapMatrix[x][y] == 4) {
				return true;
			}
				
			
		}
		if (dirX == 0) {
			for (int i = -sizeMaxX; i < sizeMaxX; i++) {
				if (mapMatrix[x + i][y] == 4){
					return true;
				}
			}
		}
		else {
			for (int i = -sizeMaxY; i < sizeMaxY; i++) {
				if (mapMatrix[x][y + i] == 4) {
					return true;
				}
			}
		}
		return false;
	}


};
