//Juan Ortega
//Daniel Maykel
//Final Project
//May 11, 2017

#include "stdafx.h"
#include <iostream>
#include <assert.h>
#include <random>
#include <time.h>
#include <vector>
#include <fstream>

using namespace std;

int x = 10;
int y = 10;
int numofruns = 30;
int cycles = 300;
int numofmines = y - 1;

#define jrand (rand()%(x*y));//random numbers for agent and prize positioning
#define orand (rand()%y);
#define o2rand (rand()%x);
#define rrand (rand()%4); //randome number for action
#define arand double(rand()%1000)/1000000; //random number for small action value
#define crand double(rand()%100)/100;


class medic {
public:
	int numpos;

	int heresol;
	int heremedic;
	int heremine;

	double value;
};

class qtable {
public:
	double nval;
	double sval;
	double eval;
	double wval;
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

int choice(vector<qtable> value, int agentcoord) {
	if (value.at(agentcoord).nval >= value.at(agentcoord).sval) {
		if (value.at(agentcoord).nval > value.at(agentcoord).eval) {
			if (value.at(agentcoord).nval >= value.at(agentcoord).wval) {
				return 0;
			}
			else {
				return 3;
			}
		}
		else {
			if (value.at(agentcoord).eval > value.at(agentcoord).wval) {
				return 2;
			}
			else {
				return 3;
			}
		}
	}
	else {
		if (value.at(agentcoord).sval >= value.at(agentcoord).eval) {
			if (value.at(agentcoord).sval > value.at(agentcoord).wval) {
				return 1;
			}
			else {
				return 3;
			}
		}
		else {
			if (value.at(agentcoord).eval >= value.at(agentcoord).wval) {
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

void learn(vector<qtable> value, vector<medic> reward, int tplus, int t, int Q) {
	double alpha = .9;
	double gamma = .9;
	double maxQspa = 0;
	int maxQ;
	maxQ = choice(value, tplus);
	//cout << maxQ << "\t" << tplus << endl;
	if (maxQ == 0) { //finding the max value of the new state
		maxQspa = value.at(tplus).nval;
	}
	else if (maxQ = 1) {
		maxQspa = value.at(tplus).sval;
	}
	else if (maxQ = 2) {
		maxQspa = value.at(tplus).eval;
	}
	else if (maxQ = 3) {
		maxQspa = value.at(tplus).wval;
	}
	else {
		assert(0 == 1);
	}
	//cout << maxQspa << endl;
	if (Q == 0) { //updating the value of the previous state
		value.at(t).nval = value.at(t).nval + 
			alpha*(reward.at(tplus).value + gamma*maxQspa - value.at(t).nval);
	}
	else if (Q = 1) {
		value.at(t).sval = value.at(t).sval + 
			alpha*(reward.at(tplus).value + gamma*maxQspa - value.at(t).sval);
	}
	else if (Q = 2) {
		value.at(t).eval = value.at(t).eval + 
			alpha*(reward.at(tplus).value + gamma*maxQspa - value.at(t).eval);
	}
	else if (Q = 3) {
		value.at(t).wval = value.at(t).wval + 
			alpha*(reward.at(tplus).value + gamma*maxQspa - value.at(t).wval);
	}
	else {
		assert(0 == 1);
	}
}


int main() {
	srand(time(NULL));

	vector<medic> grid;
	vector<qtable> qtosoldier;
	vector<qtable> qtostart;
	
	vector<int> totalsteps;
	//vector<vector<int>> run;
	
	//for(int oh=0;oh<numofruns;oh++){}

	/////////////////////////////////////////////////////
	//INITIALIZE GRID
	
	int xpos = o2rand;
	int ypos = orand;
	int xprize = o2rand;
	int yprize = orand;
	cout << "Soldier is at " << xprize << "," << yprize << endl;
	cout << "Medic is at " << xpos << "," << ypos << endl;

	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			medic rescue;
			qtable qvalue;

			rescue.numpos = coord(i, j);
			qvalue.nval = arand;
			qvalue.sval = arand;
			qvalue.eval = arand;
			qvalue.wval = arand;

			if (i <= 0) {
				qvalue.wval = -10000;
			}
			if (j <= 0) {
				qvalue.sval = -10000;
			}
			if (i >= x) {
				qvalue.eval = -10000;
			}
			if (j >= y) {
				qvalue.nval = -10000;
			}

			if ((i == xprize) && (j == yprize)) {
				rescue.heresol = 1;
				rescue.heremedic = 0;
				rescue.heremine = 0;
				rescue.value = 1000000;
			}
			else if ((i == xpos) && (j == ypos)) {
				rescue.heresol = 0;
				rescue.heremedic = 1;
				rescue.heremine = 0;
				rescue.value = -1;
			}
			else {
				rescue.heresol = 0;
				rescue.heremedic = 0;
				rescue.heremine = 0;
				rescue.value = -1;
			}
			grid.push_back(rescue);
			qtosoldier.push_back(qvalue);
			//qtostart.push_back(qvalue);
		}
	}

	int z = 0;
	int xmine;
	int ymine;
	int minecoord;
	while (z < numofmines) {
		xmine = o2rand;
		ymine = orand;
		minecoord = coord(xmine, ymine);
		if (grid.at(minecoord).heremedic == 1 |
			grid.at(minecoord).heresol == 1 |
			grid.at(minecoord).heremine == 1) {

		}
		else {
			grid.at(minecoord).heremine = 1;
			grid.at(minecoord).value = -10000;
			z++;
		}
	}

	/////////////////////////////////////////////////////
	//Q-LEARNING
	int prizecoord = coord(xprize, yprize);
	double epsilon = .5;
	int stepstaken = 0;
	int agentcoord = 0;
	int agentcoordtemp;
	int agentcoordsaver;
	int choicemade = 0;
	totalsteps.clear();

	for (int m = 0; m < cycles; m++) {
		agentcoord = coord(xpos, ypos);
		grid.at(agentcoord).heremedic = 1;
		grid.at(prizecoord).heremedic = 1;
		stepstaken = 0;
		while (agentcoord != prizecoord) {
			
			//CHOOSING A MOVE
			double E = crand;
			if (E >= epsilon) {
				choicemade = choice(qtosoldier, agentcoord);
			}
			else {
				choicemade = rrand;
			}

			//MAKING THE MOVE
			agentcoordtemp = movement(choicemade, agentcoord);
			agentcoordsaver = agentcoord;
			
			if (agentcoordtemp < 0) { //bumper: keeps the agent in place if it decides to move out of the play zone
									  //cout << "Wall" << endl;
			}
			else if (agentcoordtemp >(x*y - 1)) {
				//cout << "Wall" << endl;
			}
			else {
				agentcoord = agentcoordtemp;
			}
			grid.at(agentcoordsaver).heremedic = 0;
			grid.at(agentcoord).heremedic = 1;

			//LEARNING FROM THE MOVE
			learn(qtosoldier, grid, agentcoord, agentcoordsaver, choicemade);

			stepstaken;
		}
		totalsteps.push_back(stepstaken);
	}
	//ren.push_back(totalsteps);

	/////////////////////////////////////////////////////
	//EXPORT DATA
	ofstream cout("Final_Data.txt");
	int exportdata;
	cout << "____________________________" << endl;
	for (int m = 0; m < cycles; m++) {
		//for (int n = 0; n < numofruns:n++) {
			//exportdata = run.at(n).at(m);
		exportdata = totalsteps.at(m);
		cout << exportdata << endl;
		//cout<<exportdata<<"\t";
		//}
		//cout<<endl;
	}



	/////////////////////////////////////////////////////
	//



	/////////////////////////////////////////////////////
	//

	int j;
	cin >> j;
	return 0;
}