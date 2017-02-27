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

int coord()

int main()
{
	srand(time(NULL));

	vector<agent> grid;
	vector<values> prizes;
	agent gridspace;
	values prizevalues;

	int xposition = jrand;
	int yposition = orand;
	int xprize = jrand;
	int yprize = orand;
	cout << xprize << yprize << xposition << yposition << endl;
	
	///////////////////////////////////
	//GRID SET UP
	
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if ((i == xposition) && (j == yposition)) {
				gridspace.here = 1;
				gridspace.xpos = i;
				gridspace.ypos = j;
				grid.push_back(gridspace);

				cout << "Position at ";
				cout << i << "," << j << endl;

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

				cout << "Goal at ";
				cout << i << "," << j << endl;
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

	int o = 0;
	int j = 0;
	cin >> o;
	assert(o == j);
	return 0;
}

