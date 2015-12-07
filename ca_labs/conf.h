#pragma once

#include <iostream>
#include "kiss.h"

#define ASCII_TO_UCHAR_CONF '0'
#define ASCII_TO_UCHAR_CONF_TRANSITION ('0' - ASCII_TO_UCHAR_CONF)

typedef unsigned char state_t;

class __declspec(dllimport) Conf {
public:
	size_t __size;
	const size_t radius;
	state_t* __periodic_sites;
	state_t* __first_site;
	state_t* __last_site;
	void __update_sites(const size_t increase, const state_t state = 0);
	Conf(size_t radius);
	Conf(Conf& other);
	~Conf();
	void add_sites(const size_t increase, const state_t state);
	void clear();
	void shuffle(Seed* seed);
	void update_periodic_border();
	size_t count_state(state_t state);
	bool operator==(const Conf& other);
};

class __declspec(dllimport) Conf_Char_Map {
public:
	const state_t* states_char_map;
	Conf* __conf;
	Conf_Char_Map(const state_t char_map[]);
	~Conf_Char_Map();
	Conf_Char_Map& operator()(Conf* conf);
	friend std::ostream& operator<<(std::ostream& stream, Conf_Char_Map& me) {
		for (size_t i = 0; i < me.__conf->__size; ++i)
			stream << static_cast<unsigned char>(me.states_char_map[me.__conf->__first_site[i] - ASCII_TO_UCHAR_CONF_TRANSITION]);
		return stream;
	}
};