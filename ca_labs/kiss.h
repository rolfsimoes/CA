#pragma once
#include <random>

class __declspec(dllimport) Seed {
private:
	std::random_device __random;
	unsigned int __iter_x;
	unsigned int __iter_y;
	unsigned int __iter_z;
	unsigned int __iter_c;
public:
	unsigned int x;
	unsigned int y;
	unsigned int z;
	unsigned int c;
	Seed();
	void new_seed();
	unsigned int JKISS_random_gen();
	unsigned int zero_const_gen();
	Seed& operator=(Seed& other);
};
