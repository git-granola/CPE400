//Hayden Donovan
#include "help.h"
#include<iostream>

using namespace std;

unsigned seed = chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator (seed);
uniform_real_distribution<float> distr(0.0f, 0.99f);

//generator.seed(

int main(){

	Node test[5];

	test[0].addNeighbor(test[1], 4);
	test[0].addNeighbor(test[2], 8);

	test[1].addNeighbor(test[3], 2);
	test[1].addNeighbor(test[4], 7);

	test[2].addNeighbor(test[4], 5);

	test[3].addNeighbor(test[4], 3);

	// vector<int> vec (2);
	// vec[0]=1;
	// vec[1]=2;

	// for(size_t i=0; i<vec.size(); i++) cout << vec[i] << ' ';
	// cout << endl;

	// vec.push_back(3);

	// for(size_t i=0; i<vec.size(); i++) cout << vec[i] << ' ';
	// cout << endl;

	// vector<int> vec2 (vec);
	// vec2.push_back(4);

	// for(size_t i=0; i<vec.size(); i++) cout << vec[i] << ' ';
	// cout << endl;

	// for(size_t i=0; i<vec2.size(); i++) cout << vec2[i] << ' ';
	// cout << endl;


	modASearch(test[0], test[4], 1.0, 5);

	return 0;
}
