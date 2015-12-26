#pragma once
#ifndef DECLSPEC
#define DECLSPEC __declspec(dllexport)
#endif

#include <iostream>
#include "jkiss.h"

#define ASCII_TO_UCHAR_CONF '0'

typedef char char_state_t;
typedef unsigned int state_t;
typedef bool check_return;

struct Configuration {
	uint_t size;
	uint_t radius;
	state_t * periodic_sites;
	state_t * first_site;
	state_t * last_site;
	char_state_t * char_map;
	uint_t char_map_size;
	friend std::ostream& operator<<(std::ostream& stream, 
		const Configuration& conf);
};

DECLSPEC Configuration * create_configuration(const uint_t radius,
	const uint_t size,
	const char_state_t char_map[],
	const uint_t char_map_size);

DECLSPEC void destroy_configuration(Configuration * conf);

DECLSPEC check_return append_sites(Configuration * conf,
	const uint_t increase, 
	const char_state_t char_state);

DECLSPEC check_return copy_configuration(Configuration * conf_to,
	const Configuration * conf_from);

DECLSPEC void shuffle_configuration(Configuration * conf);

DECLSPEC void update_periodic_border(Configuration * conf);

DECLSPEC uint_t count_conf_state(Configuration * conf,
	const state_t state);

DECLSPEC bool are_equal_configurations(const Configuration * conf1,
	const Configuration * conf2);

DECLSPEC void damage_configuration(Configuration * conf,
	uint_t damage_size,
	const char_state_t* damaging_char_states,
	const uint_t damaging_char_states_size);

std::ostream& operator<<(std::ostream& stream,
	const Configuration& conf);
