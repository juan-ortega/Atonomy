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
int numcities = 10; // Number of cities
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


void init_mutate(vector<city>* pcities, int modifytype) { 
	///////////////// LR_4
	//This function uses the same mechanism as the shuffling function in 
	//HW 1.

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
		///////////////// LR_7
		assert(distance < sqrt(100 * 100 * 2));
		//This ensures the equation results within the parameters

		tempfit = tempfit + distance; 
		///////////////// LR_7
		assert(tempfit >= distance);
		//Checks that the distance calculated always increases as the agent travels.
	}

	///////////////// MR_2
	assert(tempfit >= numcities - 1);
	assert(tempfit < numcities*sqrt(100 * 100 * 2));
	//This checks that the total distance between all visited cities
	//is within the possible range

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
	///////////////// HR_2
	assert(fitdis(cities) == numcities - 1); //This checks that the fitness the same as the manually calculated one.

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

	assert(cities.size() == numcities);///////////////// HR_3
									   ///////////////// HR_4
	//This function checks that the number of cities assigned is what it was signifies

	////////////////////////////////////////////////////////
	//INITIALIZE

	modifytype = 0;
	for (int i = 0; i < startpolinum; i++) {
		init_mutate(pcities, modifytype);
		policies.push_back(cities);
	}

	///////////////// MR_1
	assert(policies.size() == startpolinum);

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

	ofstream cout("PG.txt");


	for (int k = 0; k < generations; k++) {
		
		temppolicy.clear();
		temppolicies.clear();
		fitness.clear();

		///////////////// MR_5
		assert(policies.size() == startpolinum);
		//Checks that the number of policies are equal to the number of starting
		//policies.

		for (int i = 0; i < startpolinum; i++) {
			int o = orand;
			temppolicy = policies.at(i);

			init_mutate(ptemppolicy, modifytype); 
			policies.push_back(temppolicy);
		}

		///////////////// LR_4
		//cout << fitdis(policies.at(0)) <<"---------------" <<fitdis(policies.at(startpolinum)) << endl;
		//assert(fitdis(policies.at(0)) == fitdis(policies.at(startpolinum)));
		//This uses the fitdis function to check that a mutation was
		//actually done. If the total distance traveled by following
		//the policy changes so did the policy itself. Sometimes, by chance
		//nothting chances that is why i have commented this part.

		///////////////// MR_5
		assert(policies.size() == startpolinum * 2);
		//Checks that number of policies are double the number of starting
		//policies due to the replication and mutation step.

		////////////////////////////////////////////////////////
		//EVALUATE

		double tempfit;

		for (int i = 0; i < policies.size(); i++) { 
			temppolicy = policies.at(i);
			tempfit = fitdis(temppolicy);
			fitness.push_back(tempfit); ///////////////// MR_3
		}
		
		///////////////// MR_3
		assert(fitness.size() == policies.size());
		//Makes sure each policy has a fitness attached to it

		/*
		for (int i = 0; i < policies.size(); i++) {
			cout << fitness.at(i) << endl;
		}
		*/

		////////////////////////////////////////////////////////
		//DOWN SELECT

		int ds = 0;

		///////////////// MR_4
		assert(policies.size() == startpolinum * 2); 
		//Checks that number of policies are double the number of starting
		//policies due to the replication and mutation step.

		for (int i = 0; i < startpolinum; i++) {
			ds = downselect(fitness); //Binary Tournament
			temppolicies.push_back(policies.at(ds));
		}
		
		policies = temppolicies;

		///////////////// MR_4
		assert(policies.size() == startpolinum);
		//checks that the number of policies goes back to the starting 
		//number, proving the down select

		for (int i = 0; i < policies.size(); i++) {
			cout << fitness.at(i) << "\t";
		}
		cout << endl;
	}

	///////////////// HR_1
	//HR_1 will be proven in the write up. A way could not be found to use
	//an assert to prove this checkpoint.
	double tempfit;
	for (int i = 0; i < policies.size(); i++) {
		temppolicy = policies.at(i);
		tempfit = fitdis(temppolicy);
		fitness.push_back(tempfit);
	}
	
	///////////////// LR_5
	for (int j = 0; j < policies.size(); j++) {
		assert(policies.at(j).at(0).initloc == 1);

	}
	//Checks the final policies to see if the starting city is the same
	//for all policies

	///////////////// LR_6
	for (int j = 0; j < policies.size(); j++) {
		for (int p = 0; p < numcities - 1; p++) {
			assert(policies.at(j).at(p).initloc != policies.at(j).at(p + 1).initloc);
		}
	}
	//This ensures that no city was visited twice using the initial location 
	//attribute given to it in the begining, which also works as an identifier.
	
	
	/*
	for (int i = 0; i < policies.size(); i++) {
		cout << fitness.at(i) << endl;
	}
	*/

	int j;
	cin >> j;
	return 0;
}

