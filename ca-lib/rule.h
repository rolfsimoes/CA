#pragma once
#ifndef DECLSPEC
#define DECLSPEC __declspec(dllexport)
#endif

#include "conf.h"

struct Rule {
	uint_t radius;
	uint_t states;
	state_t * rule_table;
	friend std::ostream& operator<<(std::ostream& stream, 
		const Rule& rule);
};

DECLSPEC Rule * create_rule(const uint_t radius,
	const uint_t states);

DECLSPEC void destroy_rule(Rule * rule);

DECLSPEC void load_string_rule(Rule * rule,
	const char_state_t * str_rule,
	const uint_t str_rule_size);
