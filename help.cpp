#include"help.h"

//Modified A* Search
bool modASearch(Node& start, Node& dest, float x, int numNodes){
	// int* path = new int[numNodes];//make it 1 longer. 0th refers to the length
	// path[0] = 0;
	// path[1] = start.id;
	// pair <float, int*> hn ((float) INT32_MAX, path);

	vector<int> path(1);
	path[0] = start.id;

	pair <float, vector<int>> ret (INT32_MAX, path);

	for(int i=0; i<start.nebSize; i++){
		pair <float, vector<int>> temp ( hSearch(*start.neighbors[i], dest, x, start.nebInfo[i].first, path));
		if(temp.first < ret.first) ret = temp;
	}

	cout << ret.first <<": ";
	for(size_t i=0; i<ret.second.size(); i++){
		cout << ret.second[i]<<' ';
	}

	cout<<endl;
	return 1;
}

pair <float, vector<int>> hSearch(Node& start, Node& dest, float x, float g, vector<int> path){
	//initial g=0
	//cannot go to a previously visited node which....
	//h(n) 			= infinity when target energy is too low
	//otherwise 	= xD + (1-x)(B+R+T); use B energy in place when broadcasting... maybe

	//h=distance to next node
	//g=current distance
	//I RETURN h, the next layer up cares about g
	//what is the cost of current->next? set that to .first
	//save the current path and if

	vector<int> vec (path);
	vec.push_back(start.id);

	pair <float, vector<int>> ret (INT32_MAX, vec);

	if(start.id==dest.id){
		ret.first = g;
		return ret;
	}

	for(int i=0; i<start.nebSize; i++){
		bool backtrack=false;
		for(size_t j=0; j<vec.size(); j++){
			if(start.neighbors[i]->id == vec[j]){
				backtrack = true;
				break;
			}
		}
		if(!backtrack){
			//Goal: each branch is given a temp array and deleted later.
			pair <float, vector<int>> temp (hSearch(*start.neighbors[i], dest, x, g+start.nebInfo[i].first, vec));
			if(temp.first < ret.first) ret = temp;
		}
	}
	return ret;
}


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

bool Node::addNeighbor(Node &neb, const float dist){

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

bool Node::updateNeighbors(){
	// int validNebs=0;
	for(int i=0; i<nebSize; i++){
		if(nebInfo[i].second >= G){//go through nebInfo and if a neighbor can receive an update,
			energy -= B;//reduce energy by B broadcast cost to know what energy to tell its neighbors it actually has
		}	
	}

	for(int i=0; i<nebSize; i++){
		if(neighbors[i]->energy > G){//if a node can receive a broadcast
			for(int j=0; j<neighbors[i]->nebSize; j++){
				if(id==neighbors[i]->neighbors[j]->id){//go through neighbors' neighbors[] list, look for matching id. 
					neighbors[i]->neighbors[j]->nebInfo->second = energy;//set that neighbors nebInfo of this node's energy
					neighbors[i]->neighbors[j]->energy -= G;//update that neighbors energy to the new value post "Get" message
					break;
				}
			}
		}
	}
	return 1;
}
