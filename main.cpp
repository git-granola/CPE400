//Hayden Donovan
#include "help.h"
#include<iostream>

using namespace std;

unsigned seed = chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator (seed);
uniform_real_distribution<float> distr(0.0f, 0.99f);

//generator.seed(

int main(){

	int testNum = 5;

	Node* a = new Node[testNum];
	int index, dist;

	for(int i=0; i<testNum; i++){
		for(int j=0; j<MAX_NEIGHBOR; j++){
			index = (int)(distr(generator)*(testNum));
			dist = (int)(distr(generator)*15)+1;
			a[i].addNeighbor(a[index], dist);
		}
	}

	for(int i=0; i<testNum; i++)
		a[i].printNeighbors();

	delete[] a;

	return 0;
}
