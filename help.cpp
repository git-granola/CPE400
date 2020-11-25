#include"help.h"

//Class Node================================
int Node::numNodes=0;

Node::Node():energy(MAX_ENERGY), nebSize(0), id(numNodes++){
	for(int i=0; i<MAX_NEIGHBOR; i++){
		neighbors[i]=NULL;
	}
}

Node::Node(int e):energy(e), nebSize(0), id(numNodes){
	for(int i=0; i<MAX_NEIGHBOR; i++){
		neighbors[i]=NULL;
	}
}

Node::~Node(){
	for(int i=0; i<nebSize; i++){
		neighbors[i]=NULL;
	}
}

bool Node::addNeighbor(Node &neb, const int dist){

	if(!&neb || nebSize >= MAX_NEIGHBOR || neb.nebSize >= MAX_NEIGHBOR || id == neb.id || dist < 0 ) return false;//either node would have too many neighbors or distance creates infinite loop
	
	for(int i=0; i<nebSize; i++){
		if(neighbors[i]->id == neb.id) return false;//this means this node is already in the list
	}

	int i, j;
	for(i=0; i<nebSize; i++){}
	for(j=0; j<neb.nebSize; j++){}

	nebSize++;
	neb.nebSize++;

	neighbors[i]=&neb;
	nebInfo[i].first = dist;
	nebInfo[i].second = neb.energy;
	
	neb.neighbors[j] = this;
	neb.nebInfo[j].first = dist;
	neb.nebInfo[j].second = energy;

	return true;
}

void Node::printNeighbors(){
	cout<<id<<":\n";
	for(int i=0; i<nebSize; i++){
		cout<<"\t-- (" << nebInfo[i].first <<':'<<nebInfo[i].second << ")\t->"<< neighbors[i]->id << endl; 
	}
}
