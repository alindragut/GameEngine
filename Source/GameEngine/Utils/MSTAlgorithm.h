#pragma once
#include <vector>
#include <GameEngine\Utils\Delaunay\delaunay.h>
#include <map>

/* https://www.geeksforgeeks.org/prims-minimum-spanning-tree-mst-greedy-algo-5/ */

class MSTAlgorithm
{
private:
	static float distance(std::pair<float, float> a, std::pair<float, float> b) { return sqrt((a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second)); }
	static std::pair<float, float> minKey(std::map<std::pair<float, float>, float> key, std::map<std::pair<float, float>, bool> mstSet, std::vector<Delaunay::VertexType> points)
	{
		// Initialize min value  
		float min = FLT_MAX;
		std::pair<float, float> min_index;

		for (Delaunay::VertexType point : points) {
			std::pair<float, float> aux(point.x, point.y);
			if (mstSet[aux] == false && key[aux] < min) {
				min = key[aux];
				min_index = aux;
			}
		}

		return min_index;
	}

	static bool hasEdge(std::vector<Delaunay::TriangleType> edges, std::pair<float, float> u, std::pair<float, float> v) {
		for (Delaunay::TriangleType triangle : edges) {
			std::pair<float, float> a(triangle.a->x, triangle.a->y);
			std::pair<float, float> b(triangle.b->x, triangle.b->y);
			std::pair<float, float> c(triangle.c->x, triangle.c->y);
			if (((u == a) || (u == b) || (u == c)) && ((v == a) || (v == b) || (v == c))) {
				return true;
			}
		}
		return false;
	}
public:
	static std::map<std::pair<float, float>, std::pair<float, float>> Algorithm(std::vector<Delaunay::TriangleType> edges, std::vector<Delaunay::VertexType> points) {
		int n = points.size();

		// Array to store constructed MST  
		std::map<std::pair<float, float>, std::pair<float, float>> parent;

		// Key values used to pick minimum weight edge in cut  
		std::map<std::pair<float, float>, float> key;

		// To represent set of vertices not yet included in MST  
		std::map<std::pair<float, float>, bool> mstSet;

		// Initialize all keys as INFINITE  
		for (Delaunay::VertexType point : points) {
			std::pair<float, float> aux(point.x, point.y);
			key[aux] = FLT_MAX;
			mstSet[aux] = false;
		}

		// Always include first 1st vertex in MST.  
		// Make key 0 so that this vertex is picked as first vertex. 
		std::pair<float, float> root(points[0].x, points[0].y);
		key[root] = 0;
		parent[root] = root; // First node is always root of MST  

		// The MST will have V vertices  
		for (int count = 0; count < n - 1; count++)
		{
			// Pick the minimum key vertex from the  
			// set of vertices not yet included in MST  
			std::pair<float, float> u = minKey(key, mstSet, points);

			// Add the picked vertex to the MST Set  
			mstSet[u] = true;

			// Update key value and parent index of  
			// the adjacent vertices of the picked vertex.  
			// Consider only those vertices which are not  
			// yet included in MST  
			for (Delaunay::TriangleType triangle : edges) {

				std::pair<float, float> v(triangle.a->x, triangle.a->y);

				// graph[u][v] is non zero only for adjacent vertices of m  
				// mstSet[v] is false for vertices not yet included in MST  
				// Update the key only if graph[u][v] is smaller than key[v]  
				if (hasEdge(edges, u, v) && mstSet[v] == false && distance(u, v) < key[v]) {
					parent[v] = u;
					key[v] = distance(u, v);
				}

				v = std::pair<float, float>(triangle.b->x, triangle.b->y);

				// graph[u][v] is non zero only for adjacent vertices of m  
				// mstSet[v] is false for vertices not yet included in MST  
				// Update the key only if graph[u][v] is smaller than key[v]  
				if (hasEdge(edges, u, v) && mstSet[v] == false && distance(u, v) < key[v]) {
					parent[v] = u;
					key[v] = distance(u, v);
				}

				v = std::pair<float, float>(triangle.c->x, triangle.c->y);

				// graph[u][v] is non zero only for adjacent vertices of m  
				// mstSet[v] is false for vertices not yet included in MST  
				// Update the key only if graph[u][v] is smaller than key[v]  
				if (hasEdge(edges, u, v) && mstSet[v] == false && distance(u, v) < key[v]) {
					parent[v] = u;
					key[v] = distance(u, v);
				}		
			}
		}

		return parent;
	}
};