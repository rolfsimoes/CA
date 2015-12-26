#pragma once
#ifndef DECLSPEC
#define DECLSPEC __declspec(dllexport)
#endif

typedef unsigned int uint_t;

struct Seed {
public:
	uint_t x;
	uint_t y;
	uint_t z;
	uint_t c;
	uint_t w;
};

extern Seed __init_seed;
extern Seed __iter_seed;

DECLSPEC void new_random_seed();

DECLSPEC void set_seed(Seed& new_seed);

DECLSPEC uint_t jkiss_generator(uint_t max_value);
