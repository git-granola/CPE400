#ifndef HELP_H
#define HELP_H

#define MAX_NEIGHBOR 5
#define MAX_ENERGY 1000//just the default energy, not actually max

#define B 1//Broadcast energy cost
#define G 1//"Get a message" energy cost
#define T 5//Message transmission energy cost

#include <iostream>
#include <utility>
#include <random>
#include <chrono>
#include <limits>
#include <vector>

using namespace std;


class Node{
public:
	Node();//create node with default energy
	Node(int e);//create node with e energy
	~Node();

	bool addNeighbor(Node &neb, const float dist);//connect this node to node neb at distance dist.  both must be capable of new neighbors
	bool updateNeighbors();//transmits message to each neighbor its new energy AFTER its broadcasts
	bool updateNeighbor(int index, int updatedNodes);//updates a specific neighbor by index. uses updatedNodes to know how much to notify energy change.
	void printNeighbors();


//private:
	Node* neighbors[MAX_NEIGHBOR];
	pair <float,int> nebInfo[MAX_NEIGHBOR];//first is dist, second is power
	int energy, nebSize;
	const int id;
	static int numNodes;
};




bool modASearch(Node& start, Node& dest, float x, int numNodes);

pair <float, vector<int>> hSearch(Node& start, Node& dest, float x, float g, vector<int> path);//Takes Start and Dest node with energy weight x. Returns shortest distance


#endif

