// ProjectAlpha.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <assert.h>
#include <random>
#include <time.h>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <limits>

using namespace std;
int n = 3; //number of arms

#define jrand (rand()%90)+11;
#define orand double((rand()%10)+1)/10;
#define arand rand()%n;
#define rrand double(rand()%100)/100;

class ARM {
public:
	int N;
	double mu;
	double sigma;
};

class AGENT {
public:
	double value;
};

//Box Muller taken from Wikipedia
//https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform#Implementation
double armoutput(double mu, double sigma) {
	const double epsilon = std::numeric_limits<double>::min();
	const double two_pi = 2.0*3.14159265358979323846;

	static double z0, z1;
	static bool generate;
	generate = !generate;

	if (!generate)
		return z1*sigma + mu;
			
	double u1, u2;
	do
	{
		u1 = rand() * (1.0 / RAND_MAX);
		u2 = rand() * (1.0 / RAND_MAX);
	} while (u1 <= epsilon);

	z0 = sqrt(-2.0 * log(u1)) * cos(two_pi * u2);
	z1 = sqrt(-2.0 * log(u1)) * sin(two_pi * u2);
	
	return z0*sigma + mu;
	
}

void TestA(vector<ARM>* parmvalues, vector<AGENT>* pagentvalues) {
	n = 1;
	parmvalues->clear();
	pagentvalues->clear();
		
	ARM arm2;
	arm2.N = 1;
	arm2.mu = jrand;
	arm2.sigma = orand;
	parmvalues->push_back(arm2);

	AGENT agent2;
	agent2.value = 0;
	pagentvalues->push_back(agent2);
}

void TestB(vector<ARM>* parmvalues, vector<AGENT>* pagentvalues) {
	parmvalues->at(0).mu = 500;
}

int main()
{
	srand(time(NULL));
	
	////////////////////////////////////////
	//MAKING THE ARMS

	vector<ARM> armvalues;
	vector<ARM>* parmvalues = &armvalues;
	
	for (int i = 0; i < n; i++) {
		
		ARM arm;

		arm.N = i + 1;
		arm.mu = jrand;
		arm.sigma = orand;

		armvalues.push_back(arm);
	}

	for (int i = 0; i < n; i++) {
		cout << armvalues.at(i).N << "\t" << armvalues.at(i).mu << "\t";
		cout << armvalues.at(i).sigma << endl;
	}

	//END OF MAKING THE ARMS
	///////////////////////////////////////
	//MANUAL ARM OUTPUT
	
	
int M = 0;
while (M != 3) {
	cout << "Enter (1) for Manual play or (2) Agent Play" << endl;
	cin >> M;
	if (M == 1) {

		int N = 10000;
		double mvalue = 0;
		double alpha = .25;
		cout << "Pick an Arm ";

		vector<AGENT> agentvalues;

		double mistic = 0; //Value for optimistic or pessimistic

		for (int i = 0; i < n; i++) {
			AGENT agent;
			agent.value = mistic;
			agentvalues.push_back(agent);
		}

		while (N != -1) {
			cin >> N;
			if (N != 0) {
				N = N - 1;
				double reward = armoutput(armvalues.at(N).mu, armvalues.at(N).sigma);
				agentvalues.at(N).value = reward*alpha + agentvalues.at(N).value*(1 - alpha);
				cout << reward << endl;
				
				for (int o = 0; o < n; o++) {
					cout << agentvalues.at(o).value << "\t";
				}
			}
			else {
				N = -1;
			}
		}
		M = 3;
	}

	//END OF MANUAL ARM OUTPUT
	////////////////////////////////////////
	//AGENT CHOOSING
	else if (M == 2) {
		vector<AGENT> agentvalues;
		vector<AGENT>* pagentvalues = &agentvalues;

		double mistic = 0; //Value for optimistic or pessimistic

		for (int i = 0; i < n; i++) {
			AGENT agent;
			agent.value = mistic;
			agentvalues.push_back(agent);
		}

		double epsilon = .1; // percentage of pulls that will be random
		double alpha = .75; // weight of new arm value
		int cycles = 500000; // number of learning cycles the agent has to find the best arm
		int ipulls = 5; // initial pulls to set up variance in values

		for (int i = 0; i < n; i++) { //pulls each arm ipull times before starting the epsilon-greedy algorythm
			for (int j = 0; j < ipulls; j++) {
				double initialout = armoutput(armvalues.at(i).mu, armvalues.at(i).sigma);
				agentvalues.at(i).value = initialout*alpha - agentvalues.at(i).value*(1 - alpha);
			}
		}

		cout << "Agent values after initial pulls" << endl;
		for (int i = 0; i < n; i++) {
			cout << agentvalues.at(i).value << endl;
		}

		///////////////Testing
		int test = 0;
		cout << "Do you want to perform the Tests?" << endl << "Yes(1) or NO(2)" << endl;
		cin >> test;
		int mighttest = 3;
		int testlimit = 3;
		if (test == 1) {
			mighttest = 0;
			testlimit = 2;
		}
		else {
			mighttest = 2;
		}

		for (int c = mighttest; c < testlimit; c++) {
			if (c==0) {
				TestB(parmvalues,pagentvalues); //sets up one arm with a random mu and sigma value

			}
			else if (c == 1) {
				TestA(parmvalues,pagentvalues); //sets up n amount of arms with one being way higher than the rest
				cout << armvalues.size() << "\t" << agentvalues.size() << endl; //checks sizes to see if Test A parameters where set
			}
		///////////////End of Setting up Testing Parameters

			for (int i = 0; i < cycles; i++) {
				double E = rrand;
				int greedychoice = 0;
				if (E >= epsilon) {
					double tempvalue = agentvalues.at(0).value;
					for (int k = 0; k < n; k++) {
						if (tempvalue <= agentvalues.at(k).value) {
							tempvalue = agentvalues.at(k).value;
							greedychoice = k;
						}
						else {
							assert(0 == 0);
						}
					}
					double greedyout = armoutput(armvalues.at(greedychoice).mu, armvalues.at(greedychoice).sigma);
					agentvalues.at(greedychoice).value = greedyout*alpha + agentvalues.at(greedychoice).value*(1 - alpha);
					//cout << "greedy" << "\t" << greedychoice << endl;
				}
				else {
					int randchoice = arand;
					double randout = armoutput(armvalues.at(randchoice).mu, armvalues.at(randchoice).sigma); //normal distribution output from random arm
					agentvalues.at(randchoice).value = randout*alpha + agentvalues.at(randchoice).value*(1 - alpha);
					//cout << "random" << "\t" << randchoice << endl;
				}
			}
			if ((test == 1) && (c == 0)) { //decides if the tests were successful and communicates it
				if (agentvalues.at(0).value > 110) {
					cout << "Test B Passed" << endl;
					cout << "Agent values after Test B" << endl;
					for (int i = 0; i < n; i++) {
						cout << agentvalues.at(i).value << endl;
					}
				}
				else {
					cout << "Test B Failed" << endl;
					assert(0 == 1);
				}
			}
			else if ((test == 1) && (c == 1)) {
				if (abs(armvalues.at(0).mu - agentvalues.at(0).value) < (armvalues.at(0).sigma)) {
					cout << "Test A Passed" << endl;
					cout << "Test A mean of the arm" << endl << armvalues.at(0).mu << "\t" << armvalues.at(0).sigma << endl;
				}
				else {
					cout << "Test A Failed" << endl;
					assert(0 == 1);
				}
			}
		}

		//vt(a)=RaAL+vt-1(1-AL)
		if (test == 1) {
			cout << "Agent values after Test A" << endl;
		}
		else {
			cout << "Agent values after agent pulls" << endl;
		}
		for (int i = 0; i < n; i++) {
			cout << agentvalues.at(i).value << endl;
		}
		M = 3;
	}

	else {
		cout << "ENTER 1 OR 2" << endl;
		M = 0;
	}
}
	//END OF AGENT CHOOSING
	////////////////////////////////////////



	int a = 0;
	int j = 0;
	cin >> a;
	assert(a == j);
	return 0;
	return 0;
}

