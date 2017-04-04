// Project_Gamma.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <assert.h>
#include <random>
#include <time.h>
#include <vector>
#include <fstream>

///////////////// HR_3
///////////////// HR_4
int numcities = 25; // Number of cities
int startpolinum = numcities * 10; // Number of policies being tested
int generations = 300;

#define jrand double(rand()%9901)/100;  //random number to assign position to a city
#define arand int(rand()%(numcities-1))+1; ///////////////// LR_5
										   //random number that ensures the first city stays the same.
                                           //i.e. zero is not given
#define orand int(rand()%startpolinum);
#define rrand int(rand()%(startpolinum*2));

using namespace std;

class city {  ///////////////// LR_1
public:
	double xpos;
	double ypos;
	int initloc;
};


void init_mutate(vector<city>* pcities, int modifytype) { ///////////////// LR_6
	city citytemp;
	int n;

	if (modifytype = 0) {
		n = numcities;
	}
	if (modifytype = 1) {
		n = 1;
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

double fitdis(vector<city> temppolicy) { ///////////////// MR_2
	double tempfit = 0;
	double distance = 0;;
	for (int i = 0; i < (numcities - 1); i++) {
		distance = sqrt((temppolicy.at(i + 1).xpos - temppolicy.at(i).xpos)*(temppolicy.at(i + 1).xpos - temppolicy.at(i).xpos)
			+ (temppolicy.at(i + 1).ypos - temppolicy.at(i).ypos)*(temppolicy.at(i + 1).ypos - temppolicy.at(i).ypos));
		tempfit = tempfit + distance; ///////////////// LR_7
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
	
	vector<city> cities; ///////////////// LR_3
	vector<city>* pcities = &cities;
	city cityloc;
	vector<vector<city>> policies; ///////////////// LR_2
	
	int modifytype; //If 0 modify for initialization, if 1 modify to mutate. 

	///////////////////////////////////////////////////////
	//MAKING THE CITIES
	
	for (int i = 0; i < numcities; i++) {
		cityloc.xpos = i;
		cityloc.ypos = 0;
		cityloc.initloc = i + 1;
		cities.push_back(cityloc);
	}
	
	/*
	for (int i = 0; i < numcities; i++) { 
	cityloc.xpos = jrand;
	cityloc.ypos = jrand;
	cityloc.initloc = i + 1;
	cities.push_back(cityloc);
	}
	*/
	
	/*
	for (int i = 0; i < numcities; i++) {
		cout << cities.at(i).initloc << "\t" << cities.at(i).xpos << " , " << cities.at(i).ypos << endl;
	}
	*/

	////////////////////////////////////////////////////////
	//INITIALIZE
	///////////////// MR_1

	modifytype = 0;
	///////////////// HR_2
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
	///////////////// MR_5

	modifytype = 1;
	vector<city> temppolicy;
	vector<city>* ptemppolicy = &temppolicy;
	vector<double> fitness;
	vector<vector<city>> temppolicies;

	ofstream cout("PG.txt");

	for (int k = 0; k < generations; k++) {
		
		temppolicy.clear();
		temppolicies.clear();
		fitness.clear();

		for (int i = 0; i < startpolinum; i++) {
			int o = orand;
			temppolicy = policies.at(i);

			init_mutate(ptemppolicy, modifytype); ///////////////// LR_4
			policies.push_back(temppolicy);
		}

		////////////////////////////////////////////////////////
		//EVALUATE

		double tempfit;

		for (int i = 0; i < policies.size(); i++) { ///////////////// LR_8
			temppolicy = policies.at(i);
			tempfit = fitdis(temppolicy);
			fitness.push_back(tempfit); ///////////////// MR_3
		}

		/*
		for (int i = 0; i < policies.size(); i++) {
			cout << fitness.at(i) << endl;
		}
		*/

		////////////////////////////////////////////////////////
		//DOWN SELECT
		///////////////// MR_4

		int ds = 0;

		assert(policies.size() == startpolinum * 2);

		for (int i = 0; i < startpolinum; i++) {
			ds = downselect(fitness); //Binary Tournament
			temppolicies.push_back(policies.at(ds));
		}
		
		policies = temppolicies;

		assert(policies.size() == startpolinum);

		for (int i = 0; i < policies.size(); i++) {
			cout << fitness.at(i) << "\t";
		}
		cout << endl;
	}

	double tempfit;
	for (int i = 0; i < policies.size(); i++) {
		temppolicy = policies.at(i);
		tempfit = fitdis(temppolicy);
		fitness.push_back(tempfit);
	}
	///////////////// HR_1
	
	/*
	for (int i = 0; i < policies.size(); i++) {
		cout << fitness.at(i) << endl;
	}
	*/

	int j;
	cin >> j;
	return 0;
}

