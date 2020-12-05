//Hayden Donovan
#include "help.h"
#include<iostream>

using namespace std;

unsigned seed = chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator (seed);
uniform_real_distribution<float> distr(0.0f, 0.99f);

//generator.seed(

int main(){

	vector<int> path;

	Node test[6];

	test[0].addNeighbor(test[1], 3);
	test[0].addNeighbor(test[2], 4);

	test[1].addNeighbor(test[4], 1);
	test[1].addNeighbor(test[3], 7);

	test[2].addNeighbor(test[3], 10);
	test[2].addNeighbor(test[4], 3);

	test[3].addNeighbor(test[5], 6);

	test[4].addNeighbor(test[5], 20);

	printWeb(test, 6);

	path = modASearch(test[0], test[5], 0.5);
	sendPkt(test[0], test[5], path);
	printWeb(test, 6);


	path = modASearch(test[0], test[5], 0.5);
	sendPkt(test[0], test[5], path);
	printWeb(test, 6);


	path = modASearch(test[0], test[5], 0.5);
	sendPkt(test[0], test[5], path);
	printWeb(test, 6);


	return 0;
}
