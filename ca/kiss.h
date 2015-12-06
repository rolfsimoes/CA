#pragma once
#include <random>

class Seed;

typedef unsigned int(*random_generator_function)(Seed*);

class Seed {
private:
public:
	unsigned int x;
	unsigned int y;
	unsigned int z;
	unsigned int c;
	unsigned int iter_x;
	unsigned int iter_y;
	unsigned int iter_z;
	unsigned int iter_c;
	Seed();
	void new_seed();
};

unsigned int JKISS_random_gen(Seed* seed);
unsigned int zero_const_gen(Seed* seed);
