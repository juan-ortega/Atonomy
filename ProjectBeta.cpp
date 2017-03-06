// ProjectBeta.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <assert.h>
#include <random>
#include <time.h>
#include <vector>
#include <fstream>

using namespace std;

int x = 14;
int y = 10;
#define jrand (rand()%x);
#define orand (rand()%y);
#define arand double(rand()%1000)/100000;

class agent {
public:
	int xpos;
	int ypos;
	int position;
	
	double nval;
	double sval;
	double eval;
	double wval;
};

class values {
public:
	int xpos;
	int ypos;
	double value;
};

void setup(vector<agent>* pgrid, vector<values>* pprizes, int i, int j, int p, int v) {
	agent gridspace;
	values prizevalues;

	gridspace.position = p;
	gridspace.xpos = i;
	gridspace.ypos = j;
	pgrid->push_back(gridspace);
	gridspace.nval = arand;
	gridspace.sval = arand;
	gridspace.eval = arand;
	gridspace.wval = arand;

	prizevalues.value = v;
	prizevalues.xpos = i;
	prizevalues.ypos = j;
	pprizes->push_back(prizevalues);
}

int coord(int xcoord, int ycoord) {
	int z = 0;
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if ((i == xcoord) && (j == ycoord)) {
				return z;
			}
			else {
				z = z + 1;
			}
		}
	}
}

int ipos(int index) {
	int z = 0;
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (z == index) {
				return i;
			}
			else {
				z = z + 1;
			}
		}
	}
}

int jpos(int index) {
	int z = 0;
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (z == index) {
				return j;
			}
			else {
				z = z + 1;
			}
		}
	}
}

int main()
{
	srand(time(NULL));

	vector<agent> grid;
	vector<agent>* pgrid = &grid;
	vector<values> prizes;
	vector<values>* pprizes = &prizes;

	int xposition = jrand;
	int yposition = orand;
	int xprize = jrand;
	int yprize = orand;
	cout << "Prize is at " << xprize << "," << yprize << endl; 
	cout << "Agent is at " << xposition << "," << yposition << endl;

///////////////////////////////////
//GRID AND AGENT SET UP
	int p;
	int v;

	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if ((i == xposition) && (j == yposition)) {
				p = 1;
				v = -1;
				setup(pgrid, pprizes, i, j, p, v);
				//cout << "AP" << i << "," << j << endl;
			}
			else if ((i == xprize) && (j == yprize)) {
				p = 0;
				v = 100;
				setup(pgrid, pprizes, i, j, p, v);
				//cout << "GP" << i << "," << j << endl;
			}
			else {
				p = 0;
				v = -1;
				setup(pgrid, pprizes, i, j, p, v);
			}
		}
	}
//END OF SET UP
////////////////////////////////////////////////
//Q-LEARNING AGENT 
	int cycles = 30;
	int agentcoord = coord(xposition, yposition);
	int prizecoord = coord(xprize, yprize);
	int stepstaken = 0;

	for (int m = 0; m < cycles; m++) {
		while (agentcoord != prizecoord) {



			//CHOOSING A MOVE

			//LARN FROM MOVE



			stepstaken = stepstaken + 1;
		}
		cout << stepstaken << endl;
	}

//END OF Q-LEARNING AGENT
////////////////////////////////////////////////
//





	
	int a = 0;
	int j = 0;
	cin >> a;
	assert(a == j);
	return 0;
	return 0;
}
