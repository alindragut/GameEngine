#pragma once
#include <include/glm.h>
#include <time.h>
#include <list>
#include <GameEngine/IncludeList.h>

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

	Generator(int roomCount, int locationMaxX, int locationMaxY, int sizeMaxX, int sizeMaxY);
	~Generator();

	void PlaceRooms();
	glm::vec3 GetSpawnPoint();
	int** GetMatrix() { return mapMatrix; }

private:
	
	int roomCount;

	int locationMaxX;
	int locationMaxY;
	int sizeMaxX;
	int sizeMaxY;

	int** mapMatrix;

	std::list<Room> rooms;
	Texture2D* texture;

	void PlaceRoom(glm::vec3 location, glm::vec3 size);
	void PlaceCorridor(glm::vec3 location);
	void PlaceCorridors(glm::vec3 crtPos, glm::vec3 finalPos, glm::vec3 size);
	void PlaceWalls(Room room);
	void PlaceWall(Room room);


};
