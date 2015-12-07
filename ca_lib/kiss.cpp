#include "kiss.h"

Seed::Seed(){
	new_seed();
}

void Seed::new_seed() {
	x = __random();
	while (!(y = __random()));
	z = __random();
	c = (__random() % 698769068) + 1;
	__iter_x = x;
	__iter_y = y;
	__iter_z = z;
	__iter_c = c;
}

/*
Public domain code for JKISS RNG - The period of JKISS is aproximately 2**127 (MT's period is much larger: 2**19937-1)
but KISS is about 40% more fast than MT PRNG. (Source: http://www0.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf)
*/
// main function KISS generator proposed by G. Marsaglia (see source above to references);
unsigned int Seed::JKISS_random_gen() {
	unsigned long long t;
	__iter_x = 314527869 * __iter_x + 1234567;
	__iter_y ^= __iter_y << 5; __iter_y ^= __iter_y >> 7; __iter_y ^= __iter_y << 22;
	t = 4294584393ULL * __iter_z + __iter_c;
	__iter_c = static_cast<unsigned int>(t >> 32);
	__iter_z = static_cast<unsigned int>(t);
	return static_cast<unsigned int>(__iter_x + __iter_y + __iter_z);
}

unsigned int Seed::zero_const_gen() {
	return 0;
}

Seed& Seed::operator=(Seed& other) {
	x = other.x;
	y = other.y;
	z = other.z;
	c = other.c;
	__iter_x = x;
	__iter_y = y;
	__iter_z = z;
	__iter_c = c;
	return *this;
}