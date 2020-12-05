#include"help.h"


void printWeb(Node* arr, int size){
	for(int i=0; i<size; i++){
		arr[i].printNeighbors();
		cout<<endl;
	}
}

bool sendPkt(Node& start, Node& dest, vector<int> path){
	if(path[0]!=start.id || path[path.size()-1]!=dest.id){
		cout << "Not a valid path!"<<endl;
		return false;
	}

	//The following code simulates sending a packet.
	Node *ptr = &start;
	cout << "Packet at < ";

	for(size_t i=0; i<path.size(); i++){
		cout << ptr->id <<' ';
		if(i==0) ptr->updateEnergy(ptr->energy - S); //since this node ONLY has to send pkt
		else if (i==path.size()-1) ptr->updateEnergy(ptr->energy - R);	//since this node ONLY has to rcv a pkt
		else ptr->updateEnergy(ptr->energy - S - R);//since this node has to send and receive
		
		//For some reason I was getting segmentation fault errors when assigning ptr to the 2nd item specifically, but this avoids that problem.
		for(int j=0; j<ptr->nebSize; j++){
			if(ptr->neighbors[j]->id == path[i+1]){
				ptr = ptr->neighbors[j];
				break;
			}
		}
	}
	cout << ">\n";
	return true;
}


void printPath(vector<int> path){
	for(size_t i=0; i<path.size(); i++){
		cout << path[i] << ' ';
	}
	cout << endl;
}

//Modified A* Search
/*
	Modified A* Search takes a start node and a destination node, with weight x.
	The weight x determines how heavily to favor throughput (distance) vs energy usage.
	
	It costs energy for a node to receive a packet and to send a packet.  As such, start and dest only have to send or receive respectively.
	All additional nodes cost a Send and Rcv.
	As such, if all nodes have a lot of energy, it's cheaper energy wise to take as few nodes as possible regardless of distance.
	The biggest factor for energy, depending on MAX_ENERGY macro, is the amount of energy a node has.
	Nodes with more energy cost less than nodes with more energy, so if a node is very low on energy, the function will try to route around it.

	Distance cost is very simple and relies on a more traditional A* search.
	
	By combining and weighing the 2, we can find a variable best cost path
*/
vector<int> modASearch(Node& start, Node& dest, float x){

	vector<int> path(1);
	path[0] = start.id;

	pair <float, vector<int>> ret (INT32_MAX, path);

	for(int i=0; i<start.nebSize; i++){
		//We use S+R+(next node's energy lost) because we need are Sending a Node, and knowing ahead of time that the next node will receive a packet.
		//We also ignore THIS nodes energy lost because the energy cost doesnt make a difference here. it'll always be the same cost so we can basically ignore it.
		//Each value returned is compared and the "cheapest" path is chosen.
		//Each path that makes it back here is the cheapest recursive sum of that neighbor.

		start.energy -= B - G;//because it has to send and receive a transmission packet each time
		pair <float, vector<int>> temp ( hSearch(*start.neighbors[i], dest, x, pair <float, float> (x*start.nebInfo[i].first, (1.0-x)*(S + R + MAX_ENERGY - start.nebInfo[i].second)), path));
		if(temp.first < ret.first) ret = temp;
	}

	cout << "Shortest path costs " << ret.first << endl;
	return ret.second;
}

pair <float, vector<int>> hSearch(Node& start, Node& dest, float x, pair <float, float> g, vector<int> path){
	//initial g=0
	//cannot go to a previously visited node which....
	//h(n) 			= infinity when target energy is too low
	//otherwise 	= xD + (1-x)(B+R+T); use B energy in place when broadcasting... maybe

	//h=distance to next node
	//g=current distance
	//I RETURN h, the next layer up cares about g
	//what is the cost of current->next? set that to .first
	//save the current path and if

	//we dont need to take into account transmission cost for sending a packet because 

	vector<int> vec (path);
	vec.push_back(start.id);

	pair <float, vector<int>> ret (INT32_MAX, vec);

	if(start.energy < G + R){
		return ret;
	}

	if(start.id==dest.id){
		start.energy -= B - G;//it gets this AND then it sends a message back
		ret.first = g.first + (float)g.second;
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
		if(!backtrack && ((start.neighbors[i]->id == dest.id && start.neighbors[i]->energy > G + R) || 
			(start.neighbors[i]->energy > S+R+B+G) )){//this makes sure that the node has at least enough energy to actually send and/or receive the packet and do proper transmissions for its neighbors.  If it doesn't, even if THIS node doesn't know, not returning anything keeps the cost at maximum.

			//+R+S because this node is going to send AND receive a node as a part of the cost

			start.energy -= B - G;//since it has to broadcast AND receives each return

			pair <float, vector<int>> temp (hSearch(*start.neighbors[i], dest, x, pair <float, float> (g.first + x*start.nebInfo[i].first , g.second + (1.0-x)*(S + R + MAX_ENERGY - start.nebInfo[i].second)), vec));

			if(temp.first < ret.first) ret = temp;
		}
	}

	start.energy -= B;//because it has to send a broadcast back up the line
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
	numNodes=0;
}

int Node::getNebIndex(int nebId){
	for(int i=0; i<nebSize; i++){
		if(neighbors[i]->id==nebId) return i;
	}
	return -1;
}

void Node::updateEnergy(int e){
	energy = e;//We know the new value of energy

	//We want to count the number of "alive" neighbors by what this node knows
	int activeNebs=0;
	for(int i=0; i<nebSize; i++){
		activeNebs += nebInfo[i].second >= G;
	}

	//Now we send a signal to all of our alive neighbors
	for(int i=0; i<nebSize && energy >= B; i++){
		if(neighbors[i]->energy >= G){
			neighbors[i]->nebInfo[neighbors[i]->getNebIndex(id)].second = e - activeNebs*S;//we update that neighbors nebInfo to tell it that THIS node's value will be it's current energy minus its PREDICTED node energy since it will send a broadcast to all nodes it thinks are alive
			neighbors[i]->energy = neighbors[i]->energy - G >= 0 ? neighbors[i]->energy - G : 0;//just in case, we dont want energy going negative somehow
			energy -= B;//it takes B energy to send a broadcast to nodes
		}
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
	cout<<id<<":["<<energy<<"]\n";
	for(int i=0; i<nebSize; i++){
		cout<<"\t-- (" << nebInfo[i].first <<":["<<nebInfo[i].second << "])\t->"<< neighbors[i]->id << endl; 
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
