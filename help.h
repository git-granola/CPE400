#ifndef HELP_H
#define HELP_H

#define MAX_NEIGHBOR 5
#define MAX_ENERGY 100

#include <iostream>
#include <utility>
using namespace std;


class Node{
public:
	Node();//create node with default energy
	Node(int e);//create node with e energy
	~Node(){}

	bool addNeighbor(Node &neb, const int dist);//connect this node to node neb at distance dist.  both must be capable of new neighbors



//private:
	Node* neighbors[MAX_NEIGHBOR];
	pair <int,int> nebInfo[MAX_NEIGHBOR];//first is dist, second is power
	int energy, nebSize;

};





#endif

