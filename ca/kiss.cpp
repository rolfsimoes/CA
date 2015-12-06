#include "kiss.h"

std::random_device random;

Seed::Seed(){
	new_seed();
}

void Seed::new_seed() {
	x = random();
	while (!(y = random()));
	z = random();
	c = (random() % 698769068) + 1;
	iter_x = x;
	iter_y = y;
	iter_z = z;
	iter_c = c;
}

//Kiss::Kiss(Seed* seed) : seed(seed) {
//
//}
//
//
//Kiss::~Kiss() {
//
//}

/*
Public domain code for JKISS RNG - The period of JKISS is aproximately 2**127 (MT's period is much larger: 2**19937-1)
but KISS is about 40% more fast than MT PRNG. (Source: http://www0.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf)
*/

// main function KISS generator proposed by G. Marsaglia (see source above to references);
unsigned int JKISS_random_gen(Seed* seed) {
	unsigned long long t;
	seed->iter_x = 314527869 * seed->iter_x + 1234567;
	seed->iter_y ^= seed->iter_y << 5; seed->iter_y ^= seed->iter_y >> 7; seed->iter_y ^= seed->iter_y << 22;
	t = 4294584393ULL * seed->iter_z + seed->iter_c;
	seed->iter_c = static_cast<unsigned int>(t >> 32);
	seed->iter_z = static_cast<unsigned int>(t);
	return static_cast<unsigned int>(seed->iter_x + seed->iter_y + seed->iter_z);
}

unsigned int zero_const_gen(Seed* seed) {
	return 0;
}

//Seed* Kiss::seed() {
//	return seed;
//}
//
//void Kiss::seed(Seed* new_seed) {
//	seed = new_seed;
//}
