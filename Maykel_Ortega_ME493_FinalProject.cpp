// ConsoleApplication19.cpp : Defines the entry point for the console application.
//
//Created by Daniel Maykel and Juan Ortega

#include "stdafx.h"
#include <iostream>
#include <assert.h>
#include <string.h>
#include <random>
#include <time.h>
#include <fstream>

#define LYRAND (double)rand()/RAND_MAX

using namespace std;

int xmin = 0;
int ymin = 0;
int xmax = 5;
int ymax = 5;
int num_states = xmax + ymax*(xmax + 1);
int num_mines = 3;
int debug = 0; //Change to 1 to turn on a bunch of couts to help with debugging.

class mine {
public:
	int x;
	int y;
};

class soldier {
public:
	int x;
	int y;
};

class grid {//Each state, i.e. (1,1), (3,4), etc
public:
	vector<double> av;
	int sol;
	int mine;
	int med;
};

class medic {
public:
	int x;
	int y;
	int lastx;
	int lasty;
	int firstx;
	int firsty;
	int state;
	int last_state;
	int bon;

	void move(int d);
	void calc_state();
	void bounce();
	void init();
	void resetbon();

};

void medic::resetbon() { //Used to reset value telling code that the medic tried to go out of bounds
	bon = 0;
	//cout << "bon is now " << bon << endl;
}

void medic::move(int d) {//moves medic based on input
	switch (d) {
	case 0:lasty = y;
		lastx = x;
		y = y + 1;
		if (debug == 1) {
			cout << "going up, y and last y are now " << y << "\t" << lasty << endl;
		}
		break;
	case 1:lastx = x;
		lasty = y;
		x = x + 1;
		if (debug == 1) {
			cout << "going right, x and last x are now " << x << "\t" << lastx << endl;
		}
		break;
	case 2: lasty = y;
		lastx = x;
		y = y - 1;
		if (debug == 1) {
			cout << "going up, y and last y are now " << y << "\t" << lasty << endl;
		}
		break;
	case 3:lastx = x;
		lasty = y;
		x = x - 1;
		if (debug == 1) {
			cout << "going left, x and last x are now " << x << "\t" << lastx << endl;
		}
		break;
	}
	if (debug == 1) {
		cout << "The x and y, and lastx and lasty are " << x << " " << y << "\t" << lastx << " " << lasty << endl;
	}
}

void medic::init(){//resets medic's position ot it's initial poisition
	if (debug == 1) {
		cout << "medic has been initialized" << endl;
	}
	x = firstx;
	y = firsty;
	lastx = firstx;
	lasty = firsty;
	bon = 0;
	if (debug == 1) {
		cout << "The x and y, and lastx and lasty are " << x << " " << y << "\t" << lastx << " " << lasty << endl;
	}
}

void medic::calc_state() {//calculates the state based on the x and y of the medic
	state = x + y*(xmax + 1);
	last_state = lastx + lasty*(xmax + 1);
	if (debug == 1) {
		cout << "the med's state and last state are " << state << "\t" << last_state << endl;
	}
}

void medic::bounce() {//Keeps medic from going out of bounds by putting it back in it's last x, y coordinates if it tries.
	if ((x > xmax) || (x < xmin) || (y > ymax) || (y < ymin)) {
		x = lastx;
		y = lasty;
		bon = 1;
		if (debug == 1) {
			cout << "medic went out of bounds" << endl;
		}
	}
	assert((x <= xmax) && (x >= xmin) && (y <= ymax) && (y >= ymin));
}

int max(medic med, vector<grid> Q) {//Determines the max q value of the state the medic is in
	double f = -10000;
	int d = 0;
	for (int i = 0; i < 4; i++) {
		if (Q.at(med.state).av.at(i) >= f) {
			if (debug == 1) {
				cout << Q.at(med.state).av.at(i) << " is greater than " << f << endl;
			}
			f = Q.at(med.state).av.at(i);
			d = i;
		}
		else {
			if (debug == 1) {
				cout << Q.at(med.state).av.at(i) << " is less than " << f << endl;
			}
			f = f;
			d = d;
		}
	}
	return d;
}

int reward(medic med, vector<grid> Q) {//Determines what type of reward the medic gets 
	int R = 0;
	if (Q.at(med.state).mine == 1) {//If medic hits a mine, it gets a -100 penalty
		R = -100;
	}
	else if (Q.at(med.state).sol == 1) {//If medit finds the soldeir, it gets a 100 reward
		R = 100;
	}
	else if (med.bon==1) {//If medic goes out out of bounds, it gets a -100 penalty
		R = -100;
		if (debug == 1) {
			cout << "med believes it went out of bounds" << endl;
		}
	}
	else {//Medic gets a small penalty for moving and finding nothing
		R = -1;
	}
	return R;
}

int sorm(medic med, vector<grid> Q) {//Used to see if the medic found the soldier or hit a mine. Used to end while loop for every run.
	int inv = 0;
	if (Q.at(med.state).sol == 1) {
		inv = 1;
		if (debug == 1) {
			cout << "medic found soldeier" << endl;
		}
	}
	else if (Q.at(med.state).mine == 1) {
		inv = 2;
		if (debug == 1) {
			cout << "medic hit mine" << endl;
		}
	}
	else {
		inv = 0;
	}
	return inv;
}

int main() {
	srand(time(NULL));
	
	int runs = 100;
	int cycles = 30;
	double alpha = .1;
	double lambda = .9;

	vector<grid> Q;//Sets up initial grid.
	for (int i = 0; i < (num_states+1); i++) {
		grid q;
		for (int j = 0; j < 4; j++) {
			double v =  LYRAND/100;
			q.av.push_back(v);
			q.mine = 0;
			q.sol = 0;
			q.med = 0;
		}
		Q.push_back(q);
	}

	assert(Q.size() == (xmax+1)*(ymax+1)); //Test 1

	int I = rand() % (xmax + 1);//Sets up initial position of medic.
	medic med;
	med.firstx = I;
	med.firsty = 0;
	med.init();
	med.calc_state();

	Q.at(med.state).med = 1;
	
	soldier ryan;//sets up position of soldier
	ryan.x = rand() % (xmax + 1);
	ryan.y = 1 + rand() % (ymax - 1);

	int solstate = ryan.x + ryan.y*(xmax + 1);

	Q.at(solstate).sol = 1;
	
	vector<mine> mines;
	int k = 0;

	for (int i = 0; i < num_mines; i++) {//Sets 3 mines in different locations, and makes sure they are not put where the soldier or medic start.
		k = 0;
		mine m;
		while (k == 0) {
			m.x = rand() % (xmax + 1);
			m.y = 1 + rand() % (ymax - 1);
			int minestate = m.x + m.y*(xmax + 1);
			if (Q.at(minestate).med == 1 || Q.at(minestate).sol == 1 || Q.at(minestate).mine == 1) {
				k = 0;//continuaously makes new random coordinates untill it is not in any already taken positions.
			}
			else {
				k = 1;
				Q.at(minestate).mine = 1;
			}
		}
		mines.push_back(m);
	}

	assert(mines.size() == 3);//Test 2
	
	vector<double> av1;
	vector<double> av2;
	vector<double> moves1;
	vector<double> moves2;

	for (int k = 0; k < cycles; k++) {
		moves1.clear();
		for (int i = 0; i < (num_states + 1); i++) {//resets initial random q values during each cycle.
			grid q;
			for (int j = 0; j < 4; j++) {
				double v = LYRAND / 100;
				q.av.push_back(v);
			}
			Q.push_back(q);
		}

		cout << "New cycle" << endl;
		int P = 1000000;
		for (int i = 0; i < 100; i++) {
			med.init();
			int N = 0;
			int inv = 0;
			while (inv == 0) {
				inv = 0;
				N = N + 1;
				int d = 0;
				int d1 = 0;
				int r = 0;
				int R = 0;
				med.resetbon();
				med.calc_state();
				r = rand() % 10;//Used to determine how likely medic will take exploratory action
				if (r == 0) {
					d = rand() % 4;
				}
				else {
					d = max(med, Q);
				}
				med.move(d);
				med.bounce();
				med.calc_state();
				R = reward(med, Q);
				if (debug == 1) {
					cout << "The reward med recieved is " << R << endl;
				}
				d1 = max(med, Q);
				inv = sorm(med, Q);
				int J = med.last_state;
				int Y = med.state;
				Q.at(J).av.at(d) = Q.at(J).av.at(d) + alpha*(R + lambda*Q.at(Y).av.at(d1) - Q.at(J).av.at(d));//Calculates new Q value
				if (debug == 1) {
					cout << "The Q value of last state, action is " << Q.at(med.last_state).av.at(d) << endl;
				}

			}
			if (N <= P) {
				P = N;
			}
			moves1.push_back(N);
		}
		assert(P < 10); //Used to create charts
		double n = 0;
		for (int t = 0; t < moves1.size(); t++) {
			n = n + moves1.at(t);
		}
		double av = n / runs;
		av1.push_back(av);
		

		vector<grid> Q2;//Sets up inital valus for next q table
		for (int i = 0; i < (num_states + 1); i++) {
			grid q;
			for (int j = 0; j < 4; j++) {
				double v = LYRAND / 100;
				q.av.push_back(v);
				q.mine = 0;
				q.sol = 0;
				q.med = 0;
			}
			Q2.push_back(q);
		}

		for (int i = 0; i < (xmax + 1); i++) {//makes entire top row a goal zone.
			Q2.at(30 + i).sol = 1; 
		}

		med.firstx = ryan.x;//Puts medic where soldier was
		med.firsty = ryan.y;
		med.init();
		med.calc_state();

		for (int i = 0; i < num_mines; i++) {//puts mines into new q table
			int E = mines.at(i).x;
			int V = mines.at(i).y;
			int S = E + V*(xmax + 1);
			Q2.at(S).mine = 1;
		}
		moves2.clear();
		int A = 1000000;
		//To find other side
		for (int i = 0; i < runs; i++) {
			med.init();
			int N = 0;
			int inv = 0;
			while (inv == 0) {
				inv = 0;
				N = N + 1;
				int d = 0;
				int d1 = 0;
				int r = 0;
				int R = 0;
				med.resetbon();
				med.calc_state();
				r = rand() % 10;
				if (r == 0) {
					d = rand() % 4;
				}
				else {
					d = max(med, Q2);
				}
				med.move(d);
				med.bounce();
				med.calc_state();
				R = reward(med, Q2);
				if (debug == 1) {
					cout << "The reward med recieved is " << R << endl;
				}
				d1 = max(med, Q2);
				inv = sorm(med, Q2);
				int J = med.last_state;
				int Y = med.state;
				Q2.at(J).av.at(d) = Q2.at(J).av.at(d) + alpha*(R + lambda*Q2.at(Y).av.at(d1) - Q2.at(J).av.at(d));
				if (debug == 1) {
					cout << "The Q value of last state, action is " << Q.at(med.last_state).av.at(d) << endl;
				}

			}
			if (inv == 1) {
			}
			if (N <= A) {
				A = N;
			}
			moves2.push_back(N);
		}
		assert(A < 10);
		double h = 0;
		for (int t = 0; t < moves2.size(); t++) {
			h = h + moves2.at(t);
		}
		double ave =h/runs;
		av2.push_back(ave);


		//Does one full run with policies developed before
		med.firstx = I;
		med.firsty = 0;
		med.init();
		med.calc_state();
		int inv = 0;
		int B = 0;
		while (inv == 0) {
			B = B + 1;
			int d = max(med, Q);
			med.move(d);
			med.calc_state();
			inv = sorm(med, Q);
		}

		med.firstx = ryan.x;
		med.firsty = ryan.y;
		med.init();
		med.calc_state();
		inv = 0;
		int G = 0;
		while (inv == 0) {
			B = B + 1;
			int d = max(med, Q2);
			med.move(d);
			med.calc_state();
			inv = sorm(med, Q2);
		}
	}
	ofstream out("Maykel_Ortega_ME493_FinalProject_Average1");
	ofstream out1("Maykel_Ortega_ME493_FinalProject_Average2");
	ofstream out2("Maykel_Ortega_ME493_FinalProject_LearningCurve1");
	ofstream out3("Maykel_Ortega_ME493_FinalProject_LearningCurve2");

	for (int i = 0; i < av1.size(); i++) {
		out << av1.at(i) << endl;
	}

	for (int i = 0; i < av2.size(); i++) {
		out1 << av2.at(i) << endl;
	}

	for (int i = 0; i < moves1.size(); i++) {
		out2 << moves1.at(i) << endl;
	}

	for (int i = 0; i < moves2.size(); i++) {
		out3 << moves2.at(i) << endl;
	}

	return 0;
}