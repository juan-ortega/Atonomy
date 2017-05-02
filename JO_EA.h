#include <iostream>
#include <random>
#include <time.h>
#include <assert.h>
#include <algorithm>

#define LYRAND (double)rand()/RAND_MAX
#define PI 3.1415926

using namespace std;

class policy {
public:

	void init(int numofweight);
	void mutate(double mutfactor);

	double fitness;
	int ts;
	vector<double> weights;
};

struct less_than_key
{
	inline bool operator() (const policy& a, const policy& b)
	{
		return (a.fitness < b.fitness);
	}
};

void policy::mutate(double mut_size) {
	for (int i = 0; i<weights.size(); i++) {
		if (rand() % 2 == 0) {
			weights.at(i) += mut_size * LYRAND - mut_size * LYRAND;
		}
	}
}

void policy::init(int num_of_weights) {
	vector<double> b;
	for (int i = 0; i<num_of_weights; i++) {
		b.push_back(LYRAND - LYRAND);
	}
	weights = b;
}

vector<policy> EA_replicate(vector<policy> P, int pop_size) {
	vector<policy> population;
	population = P;

	assert(population.size() == pop_size / 2);

	while (population.size()<pop_size) {
		int spot = rand() % population.size();
		policy b;
		b = population.at(spot);
		b.mutate(.2);
		//test_mututation(population.at(spot),A);
		population.push_back(b);
	}

	assert(population.size() == pop_size);

	return population;
}

vector<policy> EA_downselect(vector<policy> P, int pop_size) {

	vector<policy> population;
	// population is empty
	assert(population.size() == 0);
	assert(P.size() == pop_size);

	// remove half by binary tournament
	while (population.size() < pop_size / 2) {
		// choose 2 at random
		int spot1 = rand() % P.size();
		int spot2 = rand() % P.size();
		while (spot2 == spot1) {
			spot2 = rand() % P.size();
		}
		assert(spot1 != spot2);
		double fit1 = P.at(spot1).fitness;
		double fit2 = P.at(spot2).fitness;

		if (fit1 < fit2) {
			// fit1 wins
			// copy fit1 into "population"
			policy A1 = P.at(spot1);
			population.push_back(A1);
		}
		else if (fit2 <= fit1) {
			//// fit2 wins
			// copy fit2 into "population"
			policy A2 = P.at(spot2);
			population.push_back(A2);
		}
	}

	assert(population.size() == pop_size / 2);
	return population;

}