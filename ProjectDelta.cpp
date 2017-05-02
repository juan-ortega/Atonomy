// ProjectDelta.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include <vector>
#include <assert.h>
#include <time.h>
#include <random>
#include <algorithm>

#include "LY_NN.h"
#include "JO_EA.h"

using namespace std;

#define nu 3.0 
#define dt 0.2
#define T 5.0
//u = [-15,15]

int grid = 10;

#define jrand rand()%grid
#define orand rand()%360
#define pi 3.145926

class ship {   /////////////////////////////////LR_1
public:
	double x;
	double y;
	double theta;
	double omega;

	double u;
	double min_u;
	double max_u;

	double topendx; /////////////////////////////////LR_2
	double bottomendx;
	double topendy;
	double bottomendy;

	void calc_x(); /////////////////////////////////MR_3
	void calc_y();
	void calc_theta(); /////////////////////////////MR_1
	void calc_omega();

	void simulate();
	void init();
};

void ship::calc_x() {
	x = x + nu*sin(theta)*dt; /////////////////////////////////LR_8
}

void ship::calc_y() {
	y = y + nu*cos(theta)*dt;
}

void ship::calc_theta() {
	theta = theta + omega*dt;
}

void ship::calc_omega() {
	omega = omega + (u - omega)*dt / T;
}

void ship::simulate() {
	calc_x();
	calc_y();
	calc_theta();
	calc_omega();
}

void ship::init() {
	max_u = 15;
	min_u = -15;
	x = 0; //jrand;
	y = 0; //jrand;
	topendx = 8;
	topendy = 8;
	bottomendx = topendx;
	bottomendy = topendy - 1;
	theta = pi/2; //orand;
	omega = 0; 
	u = 0;
}

int main()
{
	int maxgen = 100;
	int popsize = 100;
	
	srand(time(NULL));

	ship boat;
	boat.init();

	neural_network NN; /////////////////////////////////LR_4
	NN.setup(4, 5, 1); 

	/////////////////////////////////LR6
	NN.set_in_min_max(-5, grid + 5); //x 
	NN.set_in_min_max(-5, grid + 5); //y
	NN.set_in_min_max(0, 2 * pi); //theta
	NN.set_in_min_max(-10, 10); //omega
	NN.set_out_min_max(boat.min_u, boat.max_u);

	////////////////////////////////////
	// INITIALIZE
	vector<policy> population;
	for (int i = 0; i < popsize; i++) {
		policy b;
		b.init(NN.get_number_of_weights());
		population.push_back(b);
	}
	assert(population.size() == popsize);

	for (int gen = 0; gen < maxgen; gen++) {
		//////////////////////////////////
		//EVALUATE
		boat.init();
		for (int sim = 0; sim < popsize; sim++) {
			
			NN.set_weights(population.at(sim).weights, true);

			int tstep = 0;
			int z = 100;

			while (z != 0) {

				vector<double> state;

				double xbefore = boat.x;

				state.push_back(boat.x);
				state.push_back(boat.y);
				state.push_back(boat.theta);
				state.push_back(boat.omega);

				NN.set_vector_input(state);
				NN.execute();

				boat.u = NN.get_output(0); /////////////////////////////////LR_7

				boat.simulate();
				
				double xafter = boat.x;

				/////////////////////////////////LR_7
				//population.at(sim).fitness = fabs(boat.topendx - boat.x)
					//+ fabs(boat.topendy - boat.y);
				population.at(sim).ts = tstep;
				
				population.at(sim).fitness = sqrt((
					boat.x - boat.topendx)*(boat.x - boat.topendx)
					+ (boat.y - (boat.topendy + boat.bottomendy) / 2)
					*(boat.y - (boat.topendy + boat.bottomendy) / 2));
				//population.at(sim).ts = tstep;
				//assert(population.at(sim).fitness<grid);

				/////////////////////////////////MR_2
				if (boat.y < boat.topendy & boat.y > boat.bottomendy) {
					if (xbefore < boat.topendx&xafter > boat.topendx) {
						//assert(0 == 1);
						z = 0;
						population.at(sim).fitness = 0;
						cout << "GOAL" << endl;
					}
					if (xbefore > boat.topendx&xafter < boat.topendx) {
						//assert(0 == 1);
						z = 0;
						population.at(sim).fitness = 0;
						cout << "GOAL" << endl;
					}
				}

				if (boat.y < 0) { /////////////////////////////////LR_3
					//assert(0 == 1);
					population.at(sim).fitness = 10000;
					//cout << " B " << endl;
					break;
				}
				if (boat.y > grid - 1) {
					//assert(0 == 1);
					population.at(sim).fitness = 10000;
					//cout << " B " << endl;
					break;
				}
				if (boat.x < 0) {
					//assert(0 == 1);
					population.at(sim).fitness = 10000;
					//cout << " B " << endl;
					break;
				}
				if (boat.x > grid - 1) {
					//assert(0 == 1);
					population.at(sim).fitness = 10000;
					//cout << " B " << endl;
					break;
				}
				tstep++;
				cout << tstep << endl;
				cout << boat.x << " , " << boat.y << endl;
				if (tstep > 10000) {
					break;
				}
			}
		}

		///////////////////////////////////
		//DOWNSELECT

		population = EA_downselect(population, popsize);

		///////////////////////////////////
		//REPLICATE

		population = EA_replicate(population, popsize); /////////////////////////////////LR_5
		cout << "Generation" << gen << endl;
		
	}

	sort(population.begin(), population.end(), less_than_key());

	for (int i = 0; i<popsize; i++) {
		cout << i << "\t" << population.at(i).fitness << "\t" << population.at(i).ts << endl;
	}

	int j = 0;
	cin >> j;
	return 0;
}


/////////////////////////////////HR_1

/////////////////////////////////HR_2
//Inspection after getting it tp work.
/////////////////////////////////HR_3
//Inspection after getting it tp work.
/////////////////////////////////HR_4
//Inspection after getting it tp work.