// Project_Gamma.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <assert.h>
#include <random>
#include <time.h>
#include <vector>
#include <fstream>

int numcities = 10; // Number of cities
int startpolinum = 80; // Number of policies being tested

#define jrand double(rand()%9901)/100;  //random number to assign position to a city
#define arand int(rand()%(numcities-1))+1; //random number that ensures the first city stays the same.
                                           //i.e. zero is not given
#define orand int(rand()%startpolinum);
#define rrand int(rand()%(startpolinum*2));

using namespace std;

class city {
public:
	double xpos;
	double ypos;
	int initloc;
};


void init_mutate(vector<city>* pcities, int modifytype) {
	city citytemp;
	int n;

	if (modifytype = 0) {
		n = numcities;
	}
	if (modifytype = 1) {
		n = floor(numcities / 8);
	}

	for (int i = 0; i < n; i++) {
		int city1 = arand;
		int city2 = arand;
		
		citytemp.xpos = pcities->at(city1).xpos;
		pcities->at(city1).xpos = pcities->at(city2).xpos;
		pcities->at(city2).xpos = citytemp.xpos;
		
		citytemp.ypos = pcities->at(city1).ypos;
		pcities->at(city1).ypos = pcities->at(city2).ypos;
		pcities->at(city2).ypos = citytemp.ypos;

		citytemp.initloc = pcities->at(city1).initloc;
		pcities->at(city1).initloc = pcities->at(city2).initloc;
		pcities->at(city2).initloc = citytemp.initloc;
	}
}

double fitdis(vector<city> temppolicy) {
	double tempfit = 0;
	double distance = 0;;
	for (int i = 0; i < (numcities - 1); i++) {
		distance = sqrt((temppolicy.at(i + 1).xpos - temppolicy.at(i).xpos)*(temppolicy.at(i + 1).xpos - temppolicy.at(i).xpos)
			+ (temppolicy.at(i + 1).ypos - temppolicy.at(i).ypos)*(temppolicy.at(i + 1).ypos - temppolicy.at(i).ypos));
		tempfit = tempfit + distance;
	}
	return tempfit;
}

int downselect(vector<double> fitness) {
	int ds;
	int binary0 = rrand;
	int binary1 = rrand;
	if (fitness.at(binary0) < fitness.at(binary1)) {
		ds = binary0;
	}
	else {
		ds = binary1;
	}
	return ds;
}

int main()
{
	srand(time(NULL));
	
	vector<city> cities; // Policy followed by the agent, gives the order to visit the cities.
	vector<city>* pcities = &cities;
	city cityloc;
	vector<vector<city>> policies; //Vector that houses all the tested policies for the agent to follow.
	
	int modifytype; //If 0 modify for initialization, if 1 modify to mutate. 

	///////////////////////////////////////////////////////
	//MAKING THE CITIES
	
	for (int i = 0; i < numcities; i++) {
		cityloc.xpos = jrand;
		cityloc.ypos = jrand;
		cityloc.initloc = i + 1;
		cities.push_back(cityloc);
	}
	
	/*
	for (int i = 0; i < numcities; i++) {
		cout << cities.at(i).initloc << "\t" << cities.at(i).xpos << " , " << cities.at(i).ypos << endl;
	}
	*/

	////////////////////////////////////////////////////////
	//INITIALIZE

	modifytype = 0;

	for (int i = 0; i < startpolinum; i++) {
		init_mutate(pcities, modifytype);
		policies.push_back(cities);
	}

	/*
	for (int i = 0; i < startpoli; i++) {
		for (int j = 0; j < numcities; j++) {
			cout << policies.at(i).at(j).initloc << "   ";
		}
		cout << endl;
	}
	*/

	////////////////////////////////////////////////////////
	//REPLICATE

	modifytype = 1;
	vector<city> temppolicy;
	vector<city>* ptemppolicy = &temppolicy;
	vector<double> fitness;
	vector<vector<city>> temppolicies;

	for (int k = 0; k < startpolinum*numcities; k++) {
		
		temppolicy.clear();
		temppolicies.clear();
		fitness.clear();

		for (int i = 0; i < startpolinum; i++) {
			int o = orand;
			temppolicy = policies.at(i);

			init_mutate(ptemppolicy, modifytype);
			policies.push_back(temppolicy);
		}

		////////////////////////////////////////////////////////
		//EVALUATE

		double tempfit;

		for (int i = 0; i < policies.size(); i++) {
			temppolicy = policies.at(i);
			tempfit = fitdis(temppolicy);
			fitness.push_back(tempfit);
		}

		/*
		for (int i = 0; i < policies.size(); i++) {
			cout << fitness.at(i) << endl;
		}
		*/

		////////////////////////////////////////////////////////
		//DOWN SELECT

		int ds = 0;

		assert(policies.size() == startpolinum * 2);

		for (int i = 0; i < startpolinum; i++) {
			ds = downselect(fitness); //Binary Tournament
			temppolicies.push_back(policies.at(ds));
		}
		
		policies = temppolicies;

		assert(policies.size() == startpolinum);
	}

	double tempfit;
	for (int i = 0; i < policies.size(); i++) {
		temppolicy = policies.at(i);
		tempfit = fitdis(temppolicy);
		fitness.push_back(tempfit);
	}
	for (int i = 0; i < policies.size(); i++) {
		cout << fitness.at(i) << endl;
	}

	int j;
	cin >> j;
	return 0;
}

