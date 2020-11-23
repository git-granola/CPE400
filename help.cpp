#include"help.h"

//Class Node================================
Node::Node():energy(MAX_ENERGY), nebSize(0){
	for(int i=0; i<MAX_NEIGHBOR; i++){
		neighbors[i]=NULL;
	}
}

Node::Node(int e):energy(e), nebSize(0){
	for(int i=0; i<MAX_NEIGHBOR; i++){
		neighbors[i]=NULL;
	}
}

bool Node::addNeighbor(Node &neb, const int dist){
	if(nebSize >= MAX_NEIGHBOR || neb.nebSize >= MAX_NEIGHBOR || dist < 0) return false;//either node would have too many neighbors or distance creates infinite loop
	int i, j;
	for(i=0; i<nebSize; i++){}
	for(j=0; j<neb.nebSize; i++){}

	nebSize++;
	neb.nebSize++;

	neighbors[i]=&neb;
	distance[i] = dist;
	
	neb.neighbors[j] = this;
	neb.distance[j] = dist;

	return true;
}
