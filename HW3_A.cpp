// HW2.cpp : Defines the entry point for the console application.
//

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
#define jrand (rand()%x);
#define orand (rand()%y);
#define arand (rand()%(x*y*x*y)+x*y)

class agent {
public:
	int xpos;
	int ypos;
	int here;
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

int movement(int move,int index) {
	int i = ipos(index);
	int j = jpos(index);
	if (move == 8) {
		i = i;
		j = j + 1;
		return coord(i, j);
	}
	else if (move == 2) {
		i = i;
		j = j - 1;
		return coord(i, j);
	}
	else if (move == 6) {
		i = i + 1;
		j = j;
		return coord(i, j);
	}
	else if (move == 4) {
		i = i - 1;
		j = j;
		return coord(i, j);
	}
	else if (move == 0) { // This is Test A. Moves the agent really far to the right and up.
		i = i + arand;
		j = j + arand;
		return coord(i, j);
	}
	else {
		cout << "Enter a valid move" << endl;
		return -8;
	}
}

int main()
{
	srand(time(NULL));

	vector<agent> grid;
	vector<agent>* pgrid = &grid;
	vector<values> prizes;
	vector<values>* pprizes = &prizes;
	agent gridspace;
	values prizevalues;

	int xposition = jrand;
	int yposition = orand;
	int xprize = jrand;
	int yprize = orand;
	cout << "Prize is at " << xprize << "," << yprize << endl; // "   Position" << xposition << yposition << endl;

///////////////////////////////////
//GRID SET UP
	
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if ((i == xposition) && (j == yposition)) {
				gridspace.here = 1;
				gridspace.xpos = i;
				gridspace.ypos = j;
				grid.push_back(gridspace);

				//cout << "Position at ";
				//cout << i << "," << j << endl;

				prizevalues.value = 0;
				prizevalues.xpos = i;
				prizevalues.ypos = j;
				prizes.push_back(prizevalues);
			}
			else if ((i == xprize) && (j == yprize)) {
				gridspace.here = 0;
				gridspace.xpos = i;
				gridspace.ypos = j;
				grid.push_back(gridspace);

				prizevalues.value = 100;
				prizevalues.xpos = i;
				prizevalues.ypos = j;
				prizes.push_back(prizevalues);

				//cout << "Goal at ";
				//cout << i << "," << j << endl;
			}
			else {
				gridspace.here = 0;
				gridspace.xpos = i;
				gridspace.ypos = j;
				grid.push_back(gridspace);

				prizevalues.value = 0;
				prizevalues.xpos = i;
				prizevalues.ypos = j;
				prizes.push_back(prizevalues);
			}
		}
	}
// GRID SET UP ENDS
/////////////////////////////////////
// SETTING UP WALLS

	int wall_1 = coord(xprize, yprize - 1);
	grid.at(wall_1).here = 2;
	int wall_2 = coord(xprize, yprize + 1);
	grid.at(wall_2).here = 2;
	int wall_3;
	if (xposition < xprize) {
		wall_3 = coord(xprize - 1, yprize);
	}
	else {
		wall_3 = coord(xprize + 1, yprize);
	}
	grid.at(wall_3).here = 2;

// END OF SETTING UP WALL
/////////////////////////////////////
// MANUAL CONTROL SET UP

	int locpos = 0;
	int locprize = 0;
	
	locpos = coord(xposition, yposition);
	//cout << "Position Vector Location " << locpos << endl;
	locprize = coord(xprize, yprize);
	//cout << "Prize Vector Location " << locprize << endl;

	int z = -1;
	//int w = 8; // movements: up, down, right, and left
	//int s = 2;
	//int a = 6;
	//int d = 4;
	int move = 0;
	int locpos2 = 0;
	int play = 0;
	int tempplace = 0;

	cout << "Manual Play-TestB (1) or Agent Play-TestC (2)?" << endl;
	cin >> play;
	cout << endl;

	cout << "Start Position is " << "(" << xposition << "," << yposition << ")" << endl;

	while (z != 0) {
		if (play == 1) {
			cout << "Up (8), Down (2), Right (6), Left (4) or TestA (0)?" << endl;
			cin >> move; cout << endl;
			locpos2 = movement(move,locpos);

			int locpos3 = locpos;

			if (grid.at(locpos2).here==2) {
				cout << "Wall!... Reset" << endl;
			}
			else if (locpos2 < 0) {
				cout << "Wall!... Reset" << endl;
			}
			else if (locpos2 > (x* y - 1)) {
				cout << "Wall!... Reset" << endl;
			}
			else if (locpos2 == -8) {
				cout << "Invalid Movement" << endl;
			}
			else {
				locpos3 = locpos;
				locpos = locpos2;
			}
			if (locpos == locprize) {
				z = 0;

				tempplace = grid.at(locpos3).here;
				grid.at(locpos3).here = grid.at(locpos).here;
				grid.at(locpos).here = tempplace;

				assert(grid.at(locpos).here == 1); // this makes sure that the agent actually moved in the grid and is located at the prize's position.
				assert(prizes.at(locpos).value == 100);
				cout << "GOAL REACHED!!!" << endl;
				int o = 0;
				int j = 0;
				cin >> o;
				assert(o == j);
				return 0;
			}			
			int i = 0;
			int j = 0;
			i = ipos(locpos);
			j = jpos(locpos);
			cout << "Current Position is " << "(" << i << "," << j << ")" << endl;
			cout << "Prize is at " << "(" << xprize << "," << yprize << ")" << endl;

			tempplace = grid.at(locpos3).here;
			grid.at(locpos3).here = grid.at(locpos).here;
			grid.at(locpos).here = tempplace;
		}

//END OF MANUAL CONTROL SET UP 
//////////////////////////////////////
//AGENT SET UP

		else if (play == 2) {
			int i = ipos(locpos);
			int j = jpos(locpos);
			int tempplace = 0;
			
			while (i != xprize) {
				locpos2 = locpos;
				if (i < xprize) {
					i = i + 1;
				}
				else if (i > xprize) {
					i = i - 1;
				}

				locpos = coord(i, j);
				if (grid.at(locpos).here == 2) {
					locpos = locpos2;
					if (i < xprize) {
						i = i - 1;
					}
					else if (i > xprize) {
						i = i + 1;
					}
					//assert(0 == 1);
				}

				tempplace = grid.at(locpos2).here;
				grid.at(locpos2).here = grid.at(locpos).here;
				grid.at(locpos).here = tempplace;
				//cout << "Prize is at is " << "(" << xprize << "," << yprize << ")" << endl;
				cout << "Current Position is " << "(" << i << "," << j << ")" << endl;
			}
			while (j != yprize) {
				locpos2 = locpos;
				if (j < yprize) {
					j = j + 1;
				}
				else if (j > yprize) {
					j = j - 1;
				}

				locpos = coord(i, j);
				if (grid.at(locpos).here == 2) {
					locpos = locpos2;
					if (j < yprize) {
						j = j - 1;
					}
					else if (j > yprize) {
						j = j + 1;
					}
					//assert(0 == 1);
				}

				tempplace = grid.at(locpos2).here;
				grid.at(locpos2).here = grid.at(locpos).here;
				grid.at(locpos).here = tempplace;
				cout << "Current Position is " << "(" << i << "," << j << ")" << endl;
			}
			assert(grid.at(locpos).here == 1); // this makes sure that the agent actually moved in the grid and is located at the prize's position.
			assert(prizes.at(locpos).value == 100);
			cout << "GOAL REACHED!!!" << endl;
			int o = 0;
			j = 0;
			cin >> o;
			assert(o == j);
			return 0;
		}
		else {
			cout << "Play again and choose one of the two modes." << endl;
			int o = 0;
			int j = 0;
			cin >> o;
			assert(o == j);
			return 0;
		}
	}

//END OF AGENT SET UP
//////////////////////////////////////
	int o = 0;
	int j = 0;
	cin >> o;
	assert(o == j);
	return 0;
}

