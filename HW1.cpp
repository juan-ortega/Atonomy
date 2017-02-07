// HW1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <assert.h>
#include <random>
#include <time.h>
#include <vector>
#include <fstream>


int n = 1; //sts number of decks
int numofcards = n * 52; //total number of cards
#define jrand rand() % numofcards; //creates random function with appropriate range

using namespace std;

class Deck { //creates a class called Deck with properties, suit and rank
public:
	int suit;
	int rank;
};

void shuffle(vector<Deck>* pstack) {
	for (int i = 0; i < 52 * numofcards; i++) {
		int card1 = jrand;
		int card2 = jrand;
		Deck cardtemp;

		cardtemp.rank = pstack->at(card1).rank;
		pstack->at(card1).rank = pstack->at(card2).rank;
		pstack->at(card2).rank = cardtemp.rank;

		cardtemp.suit = pstack->at(card1).suit;
		pstack->at(card1).suit = pstack->at(card2).suit;
		pstack->at(card2).suit = cardtemp.suit;

		//Testing Test A function : this switches a rank between two random cards, effectively deleting 
		//one card as it creates a copy of another random card.
		/*int p1 = jrand;
		int p2 = jrand;
		pstack->at(p1).rank = pstack->at(p2).rank;*/

		//Testing Test B function : this switches the first card with a random card reducing the number 
		//of that card by 1. However there is a chance that the random card is the same as the first card,
		//invalidating this test.
		/*int p1 = jrand;
		pstack->at(0).rank = pstack->at(p1).rank;
		pstack->at(0).suit = pstack->at(p1).suit;*/
	}
}

void TestA(vector<Deck>* pstack) { //goes card by cards and compares them to all other cards
	for (int i = 0; i < numofcards; i++) { //card being compared
		for (int j = 0; j < numofcards; j++) { //all cards compared to the mentioned card
			if (i == j) { // this keeps the test from comparing a card with itself
				assert(i == j); 
			}
			else {
				if ((pstack->at(i).rank == pstack->at(j).rank) && (pstack->at(i).suit == pstack->at(j).suit)) {
					cout << "Test A Failed" << endl;
					assert(1 == 0);
				}
				else {
					assert(1 == 1);
				}
			}
		}
	}
	cout << "Test A Done" << endl;
}

void TestB(vector<Deck>* pstack) {
	Deck testcard; //creates variable of type Deck
	testcard.rank = pstack->at(0).rank; 
	testcard.suit = pstack->at(0).suit; //copies the first card's properties to testcard
	int z = 0;
	for (int i = 0; i < numofcards; i++) {
		if ((testcard.rank == pstack->at(i).rank) && (testcard.suit == pstack->at(i).suit)) {
			z = z + 1; //counts how many times a card like testcard shows up
		}
	}
	if (n == z) { //if the number of times the first card of the deck shows up is the same as the 
				  //number of decks, then this test is passed.
		cout << "Test B Done" << endl;
	}
	else {
		assert(1 == 0);
	}
}

int main()
{
	srand(time(NULL));

	///////////////////////////
	//CARD CREATION

	vector<Deck> stack; //creates a vector of class Deck named stack
	
	cout << "Initial size: " << stack.size() << endl;

	for (int k = 0; k < n; k++) { //number of decks 
		
		for (int i = 0; i < 4; i++) { //number of suits
						
			for (int j = 1; j < 14; j++) { //number of ranks
				
				Deck card; //creates objects of class Deck named card
				card.suit = i;
				card.rank = j;
				
				stack.push_back(card);
			}
		}
	}
	cout << "New size: " << stack.size() << endl;
	assert(stack.size() == numofcards); 
	cout << "First Card Before Shuffle:" << "\t";
	cout << stack.at(0).suit << "\t" << stack.at(0).rank << endl;
	cout << "Last Card Before Shuffle:" << "\t";
	cout << stack.at(numofcards - 1).suit << "\t" << stack.at(numofcards - 1).rank << endl;
	assert(stack.at(numofcards - 1).suit == 3); // checks that the last card is suit 3
	assert(stack.at(numofcards - 1).rank == 13); // checks that the last card is rank 12
	
	//CARDS CREATED
	///////////////////////////////
	//Shuffling
	
	vector<Deck>* pstack = &stack;
	shuffle(pstack);

	cout << "First Card After Shuffle:" << "\t";
	cout << stack.at(0).suit << "\t" << stack.at(0).rank << endl;
	cout << "Last Card After Shuffle:" << "\t";
	cout << stack.at(numofcards - 1).suit << "\t" << stack.at(numofcards - 1).rank << endl;
	
	assert(stack.size() == numofcards);
	
	//Cards Shuffled
	//////////////////////////////
	//Test A

	if (n == 1) {
		TestA(pstack);
	}
	
	// End of Test A
	//////////////////////////////
	//Test B

	TestB(pstack);

	//End Test B
	//////////////////////////////
	// Print to Text File

	ofstream cout("HW1.txt");
	cout << "Position" << "\t" << "Suit" << "\t" << "Rank" << endl;
	for (int p = 0; p < stack.size(); p++) {
		cout << p+1 << "\t" << stack.at(p).suit << "\t" << stack.at(p).rank << endl;
	}
	
	cout << n << " deck(s)" << endl; //prints number of decks
	
	//used to test the random function
	/*
	for (int i = 0; i < 10000; i++) {

		int t = jrand;
		cout << t << endl;

		if (t == 52*n-1) {
			i = 9999;
		}
	}
	*/

	//this allows program window to stay open until user inputs "j"
	int a = n;
	int j = n;
	cin >> a;
	assert(a == j);
	return 0;
};

