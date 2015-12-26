#include <random>
#include "jkiss.h"

std::random_device __random;
Seed __init_seed;
Seed __iter_seed;

void new_random_seed() {
	__init_seed.x = __random();
	while (!__init_seed.y) 
		__init_seed.y = __random();
	__init_seed.z = __random();
	__init_seed.c = (__random() % 698769068) + 1;
	__iter_seed.x = __init_seed.x;
	__iter_seed.y = __init_seed.y;
	__iter_seed.z = __init_seed.z;
	__iter_seed.c = __init_seed.c;
};

void set_seed(Seed& new_seed) {
	__init_seed.x = new_seed.x;
	__init_seed.z = new_seed.z;
	__init_seed.y = new_seed.y;
	__init_seed.c = new_seed.c;
	__iter_seed.x = __init_seed.x;
	__iter_seed.y = __init_seed.y;
	__iter_seed.z = __init_seed.z;
	__iter_seed.c = __init_seed.c;
}

/*
Public domain code for JKISS RNG - The period of JKISS is aproximately 2**127 (MT's period is much larger: 2**19937-1)
but KISS is about 40% more fast than MT PRNG. (Source: http://www0.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf)
*/
// main function KISS generator proposed by G. Marsaglia (see reference above to more informations);
#ifdef _WIN64
//#define _RND_MAX 9007199254740992.0
#define _RND_MAX 4294967296.0
uint_t jkiss_generator(uint_t max_value) {
	unsigned __int64 t;
	__iter_seed.x = 314527869 * __iter_seed.x + 1234567;
	__iter_seed.y ^= __iter_seed.y << 5; __iter_seed.y ^= __iter_seed.y >> 7; __iter_seed.y ^= __iter_seed.y << 22;
	t = 4294584393ULL * __iter_seed.z + __iter_seed.c;
	__iter_seed.c = static_cast<uint_t>(t >> 32);
	__iter_seed.z = static_cast<uint_t>(t);
	uint_t result = static_cast<uint_t>(((__iter_seed.x + __iter_seed.y + __iter_seed.z) / _RND_MAX) * max_value);
	return result;
}
#else
#define _RND_MAX 4294967296.0
/* 
Implementation of a 32-bit KISS generator which uses no multiply instructions. 
The period is ~2^121 = 2.6 x 10^36
*/
uint_t jkiss_generator(uint_t max_value) {
	int t;
	__iter_seed.y ^= (__iter_seed.y << 5); __iter_seed.y ^= (__iter_seed.y >> 7); __iter_seed.y ^= (__iter_seed.y << 22);
	t = __iter_seed.z + __iter_seed.w + __iter_seed.c;
	__iter_seed.z = __iter_seed.w;
	__iter_seed.c = t < 0;
	__iter_seed.w = t & 2147483647;
	__iter_seed.x += 1411392427;
	return static_cast<uint_t>(((__iter_seed.x + __iter_seed.y + __iter_seed.w) / _RND_MAX) * max_value);
}
#endif
