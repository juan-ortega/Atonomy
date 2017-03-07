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

int x = 5;
int y = 5;
int numofruns = 30;
int cycles = 500;
#define jrand (rand()%x);//random numbers for agent and prize positioning
#define orand (rand()%y);
#define rrand (rand()%4); //randome number for action
#define arand double(rand()%10000)/1000000; //random number for small action value
#define crand double(rand()%100)/100;

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

int coord(int xcoord, int ycoord) {
	int z = 0;
	if (xcoord < 0) {
		return -1;
	}
	else if (ycoord < 0) {
		return -1;
	}
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
	return (x*y);
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

int choice(vector<agent> grid, int agentcoord) {
	if (grid.at(agentcoord).nval >= grid.at(agentcoord).sval) {
		if (grid.at(agentcoord).nval > grid.at(agentcoord).eval) {
			if (grid.at(agentcoord).nval >= grid.at(agentcoord).wval) {
				return 0;
			}
			else {
				return 3;
			}
		}
		else {
			if (grid.at(agentcoord).eval > grid.at(agentcoord).wval) {
				return 2;
			}
			else {
				return 3;
			}
		}
	}
	else {
		if (grid.at(agentcoord).sval >= grid.at(agentcoord).eval) {
			if (grid.at(agentcoord).sval > grid.at(agentcoord).wval) {
				return 1;
			}
			else {
				return 3;
			}
		}
		else {
			if (grid.at(agentcoord).eval >= grid.at(agentcoord).wval) {
				return 2;
			}
			else {
				return 3;
			}
		}
	}
}

int movement(int move, int index) {
	int i = ipos(index);
	int j = jpos(index);
	if (move == 0) {
		i = i;
		j = j + 1;
		return coord(i, j);
	}
	else if (move == 1) {
		i = i;
		j = j - 1;
		return coord(i, j);
	}
	else if (move == 2) {
		i = i + 1;
		j = j;
		return coord(i, j);
	}
	else if (move == 3) {
		i = i - 1;
		j = j;
		return coord(i, j);
	}
	else {
		assert(0 == 1);
	}
}

void learn(vector<agent> grid, vector<values> prizes, int tplus, int t, int Q) {
	double alpha = .9;
	double gamma = .9;
	double maxQspa = 0;
	int maxQ;
	maxQ = choice(grid, tplus);
	//cout << maxQ << "\t" << tplus << endl;
	if (maxQ == 0) { //finding the max value of the new state
		maxQspa = grid.at(tplus).nval;
	}
	else if (maxQ = 1) {
		maxQspa = grid.at(tplus).sval;
	}
	else if (maxQ = 2) {
		maxQspa = grid.at(tplus).eval;
	}
	else if (maxQ = 3) {
		maxQspa = grid.at(tplus).wval;
	}
	else {
		assert(0 == 1);
	}
	//cout << maxQspa << endl;
	if (Q == 0) { //updating the value of the previous state
		grid.at(t).nval = grid.at(t).nval + alpha*(prizes.at(tplus).value + gamma*maxQspa - grid.at(t).nval);
	}
	else if (Q = 1) {
		grid.at(t).sval = grid.at(t).sval + alpha*(prizes.at(tplus).value + gamma*maxQspa - grid.at(t).sval);
	}
	else if (Q = 2) {
		grid.at(t).eval = grid.at(t).eval + alpha*(prizes.at(tplus).value + gamma*maxQspa - grid.at(t).eval);
	}
	else if (Q = 3) {
		grid.at(t).wval = grid.at(t).wval + alpha*(prizes.at(tplus).value + gamma*maxQspa - grid.at(t).wval);
	}
	else {
		assert(0 == 1);
	}
}

int main()
{
	srand(time(NULL));

	vector<agent> grid;
	vector<agent>* pgrid = &grid;
	vector<values> prizes;
	vector<values>* pprizes = &prizes;
	vector<int> totalsteps;
	vector<vector<int>> run;

	for (int oh = 0; oh < numofruns; oh++) {
		

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
				agent gridspace;
				values prizevalues;

				gridspace.xpos = i;
				gridspace.ypos = j;
				gridspace.nval = arand;
				gridspace.sval = arand;
				gridspace.eval = arand;
				gridspace.wval = arand;

				prizevalues.xpos = i;
				prizevalues.ypos = j;

				if (i <= 0) {
					gridspace.wval = -1000;
				}
				else if (j <= 0) {
					gridspace.sval = -1000;
				}
				else if (i >= x) {
					gridspace.eval = -1000;
				}
				else if (j >= y) {
					gridspace.nval = -1000;
				}
				else {
				}
				if ((i == xprize) && (j == yprize)) {
					p = 0;
					v = 1000;
					gridspace.position = p;
					prizevalues.value = v;
				}
				else {
					p = 0;
					v = -1;
					gridspace.position = p;
					prizevalues.value = v;
				}
				grid.push_back(gridspace);
				prizes.push_back(prizevalues);
			}
		}
		//END OF SET UP
		////////////////////////////////////////////////
		//Q-LEARNING AGENT 
		int prizecoord = coord(xprize, yprize);
		int stepstaken = 0;
		double epsilon = .5;
		int agentcoord = 0;
		int agentcoordtemp;
		int agentcoordsaver;
		int choicemade = 0;
		totalsteps.clear();
		for (int m = 0; m < cycles; m++) {
			agentcoord = coord(xposition, yposition);//moving agent back to the start
			grid.at(agentcoord).position = 1;
			grid.at(prizecoord).position = 0;
			stepstaken = 0;
			while (agentcoord != prizecoord) {
				//CHOOSING A MOVE
				double E = crand;
				//cout << E << endl;
				if (E >= epsilon) {
					choicemade = choice(grid, agentcoord);
					//cout << choicemade << endl;
				}
				else {
					choicemade = rrand;
					//cout << choicemade << endl;
				}
				//MAKING THE MOVE
				agentcoordtemp = movement(choicemade, agentcoord);
				//cout << agentcoordtemp << endl;
				agentcoordsaver = agentcoord;
				if (agentcoordtemp < 0) { //bumper: keeps the agent in place if it decides to move out of the play zone
					//cout << "Wall" << endl;
				}
				else if (agentcoordtemp > (x*y - 1)) {
					//cout << "Wall" << endl;
				}
				else {
					agentcoord = agentcoordtemp;
				}
				grid.at(agentcoordsaver).position = 0; //moving the agent to the new position
				grid.at(agentcoord).position = 1;
				//LEARNING FROM THE MOVE
				learn(grid, prizes, agentcoord, agentcoordsaver, choicemade);

				stepstaken = stepstaken + 1;
				//cout << stepstaken << endl;
				//cout << agentcoord << "\t" << prizecoord << endl;
			}
			totalsteps.push_back(stepstaken);
			//cout << stepstaken << endl;
		}
		run.push_back(totalsteps);
	}
//END OF Q-LEARNING AGENT
////////////////////////////////////////////////
//
	ofstream cout("PB.txt");
	int exportdata;
	for (int m = 0; m < cycles; m++) {
		for (int n = 0; n < numofruns; n++) {
			exportdata = run.at(n).at(m);
			cout << exportdata << "\t";
		}
		cout << endl;
	}

	///////////TEST D

	for (int k = 0; k < (x*y); k++) {
		if (grid.at(k).nval > 1000) {
			assert(0 == 1);
		}
		else if (grid.at(k).sval > 1000) {
			assert(0 == 1);
		}
		else if (grid.at(k).eval > 1000) {
			assert(0 == 1);
		}
		else if (grid.at(k).wval > 1000) {
			assert(0 == 1);
		}
		else {
		}
		//cout << "TEST D PASSED!" << endl;
	}
	////////////TEST E
	//this test is implemented to the working code
	////////////TEST F
	//this test is part of the q learning portion of the code
	////////////TEST G
	//this test was not implemented past its inception

	int a = 0;
	int j = 0;
	cin >> a;
	assert(a == j);
	return 0;
	return 0;
}
