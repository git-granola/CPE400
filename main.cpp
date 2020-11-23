//Hayden Donovan
#include "help.h"
#include<iostream>

using namespace std;

int main(){

	Node a1;
	Node a2(55);

	a1.addNeighbor(a2, 10);

//	cout<<a1.neighbors[0]->energy<<endl<<a1.distance[0]<<'\t'<<a1.neighbors[0]->distance[0]<<endl;
	cout << a1.neighbors[0]->energy<<endl<<a1.nebInfo[0].first<<'\t'<<a1.neighbors[0]->nebInfo[0].first<<endl;

	
	return 0;
}
