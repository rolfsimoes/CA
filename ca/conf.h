#pragma once
#include "ca.h"
#include "kiss.h"

#define ASCII_TO_UCHAR_CONF '0'
#define UCHAR_CONF_TO_ASCII 0

class Conf {
private:
	CA* __ca;
public:
	size_t length;
private:
	state_t* __periodic_border_first_site;
public:
	state_t* first_site;
	state_t* last_site;
private:
	void __update_sites(const size_t increase, const state_t state = 0);
public:
	Conf(CA* ca);
	Conf(Conf& other);
	~Conf();
	CA& ca();
	void add(const size_t increase, const state_t state);
	void clear();
	void shuffle(const random_generator_function g, Seed* seed);
	void update_periodic_border();
	bool operator==(const Conf& other);
};

