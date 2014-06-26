/***********
Travis Sanders

Project 4: Conway's Game of Life

February 2014

Compile using: g++ -D_GLIBCXX_USE_NANOSLEEP -std=c++0x life.cpp -o life
************/

#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <limits.h>
#include <string>
#include <sstream>
#include <time.h>

using std::cin;
using std::cout;
using std::endl;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

int width = 80;
int height = 22;
int gamesize = 1760;
int game[1760];
int game_plusplus[1760];

void rand_array();
void print_game();
void overpop();
void rando();
void grow();
void evolve();
void evolve(int seconds);
int neighbors(int cell);
int cell_check(int cell);
int verify_range(int cell);

/*	Randomizes every cell in the array. Currently obsolete.
	Use rando(). */
void rand_array() {
	for (int i=0; i<gamesize; i++) {
		game[i] = rand() % 2;
	}
}

/* Clears environment. */
void clear() {
	for (int i=0; i<gamesize; i++) {
		game[i] = 0;
	}
}

/* Floods environment. Test scenario. */
void overpop() {
	for (int i=0; i<gamesize; i++) {
		game[i] = 1;
	}
}

/*	Prints the current environment. */
void print_game() {
	int i=0;
	while (i<gamesize) {
		for (int j=0; j<width; j++) {
			if (game[i] == 0) 
				cout << " ";
			else cout << "*";
			i++;
		}
		cout << endl;
	}
}

/* Debug output. Prints how many neighbors each cell has. */
void print_neighbors() {
	int i=0;
	while (i<gamesize) {
		for (int j=0; j<width; j++) {
			cout << neighbors(i);
			i++;
		}
		cout << endl;
	}
}

/* Calculates one step of environment growth. */
void grow() {
	for (int i=0; i<gamesize; i++) {
		game_plusplus[i] = cell_check(i);
	}
	for (int i=0; i<gamesize; i++) {
		game[i] = game_plusplus[i];
	}
}

/* Determines the next state of a single cell. */
int cell_check(int cell) {
	int near = neighbors(cell);
	if (game[cell] == 0) {
		if (near == 3) {
			return 1;
		} else return 0;
	}
	if (game[cell] == 1) {
		if (near < 2 || near > 3) {
			return 0;
		} else return 1;
	} 
	return 0;
}

/* Returns number of neighbors for given cell. */
int neighbors(int cell) {
	int neighbors = 0;
	int length = width;
	if (game[verify_range(cell - 1)] == 1) neighbors++;
	if (game[verify_range(cell + 1)] == 1) neighbors++;
	if (game[verify_range(cell - length)] == 1) neighbors++;
	if (game[verify_range(cell - length - 1)] == 1) neighbors++;
	if (game[verify_range(cell - length + 1)] == 1) neighbors++;
	if (game[verify_range(cell + length + 1)] == 1) neighbors++;
	if (game[verify_range(cell + length - 1)] == 1) neighbors++;
	if (game[verify_range(cell + length)] == 1) neighbors++;
	return neighbors;
}

/* 	Verifies that a cell is within the environment. 
	Used in wrap-around effect. */
int verify_range(int cell) {
	if (cell < 0) {
		return cell = gamesize + cell;
	} else if (cell > gamesize-1) {
		return cell = cell - gamesize;
	} else return cell;
}

/* Simulates Game of Life indefinitely. */
void evolve() {
	int seconds = INT_MAX;
	cout << "EVOLVING INDEFINITELY!" << endl;
	for (int i=0; i<seconds; i++) {
		grow();
		print_game();
		std::this_thread::sleep_for (std::chrono::milliseconds(100));
	}
}

/* Simulates Game of Life for given number of seconds. */
void evolve(int seconds) {
	seconds = seconds * 10;
	for (int i=0; i<seconds; i++) {
		grow();
		print_game();
		std::this_thread::sleep_for (std::chrono::milliseconds(100));
	}
}

/* Randomly fills a given percent of the environment. */
void rando(int percent) {
	int stop_at = (gamesize * percent) / 100;
	for (int i = 0; i<stop_at; i++) {
		game[rand() % gamesize] = 1;
	}
}

/* Creates a pulsar structure in the center of the environment. */
void pulsar(int mod2) {
	game[758+mod2] = 1;
	game[759+mod2] = 1;
	game[761+mod2] = 1;
	game[762+mod2] = 1;
	game[837+mod2] = 1;
	game[840+mod2] = 1;
	game[843+mod2] = 1;
	game[918+mod2] = 1;
	game[919+mod2] = 1;
	game[921+mod2] = 1;
	game[922+mod2] = 1;
}

/* Creates a glider gun. */
void make_gliders() {
	int glider_gun[36]={481,482,561,562,491,571,651,412,732,333,
		813,334,814,575,416,736,497,577,657,578,341,342,421,422,
		501,502,263,583,185,265,585,665,355,356,435,436};
	for (int i=0; i<36; i++) {
		game[glider_gun[i]] = 1;
	}
}

/* Gets a user-input int. */
int get_int() {
	std::stringstream ss;
	std::string input;
	std::getline(std::cin, input);
	ss << input;
	int return_me;
	if (ss >> return_me) {
		return return_me;
	} else {
		std::cout << "FAILURE. Invalid input. Please enter an int: ";
		return get_int();
	}
}

int main() {
	srand(time(NULL));
	bool run = true;
	rando(50);
	while (run) {
		int selection;
		cout << "Conway's Game of Life." << endl 
		<< "Enter 1 for pulsars, 2 for a glider gun, or 3 for randomization." << endl
		<< "Enter any other number to continue the previous pattern."
		<< endl << "Press 0 to exit. Input: ";
		selection = get_int();
		if (selection == 0) {
			run = false;
		}
		if (selection == 1) {
			clear();
			pulsar(0);
		}
		if (selection == 2) {
			cout << "WARNING: Glider gun will self destruct in approx. 10 seconds." << endl;
			clear();
			make_gliders();
		}
		if (selection == 3) {
			clear();
			cout << "What percent of the board should be covered?: ";
			rando(get_int());
		}
		if (run) {
			cout << "How many seconds should the program run for?" << endl
			<< "(Game runs at 10 frames per second.)" << endl
			<< "Enter 0 to run indefinitely (not recommended): ";
			int seconds = get_int();
			if (seconds == 0) {
				evolve();
			} else evolve(seconds);
			print_neighbors();
		}
	}
	return 0;
}