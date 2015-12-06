#pragma once
#include "conf.h"

#define ASCII_TO_UCHAR_CONF_TRANSITION (48 - ASCII_TO_UCHAR_CONF)

class Dereferencing;
class Rule;

typedef Dereferencing* (*lookup_function)(state_t*, Dereferencing*);

Dereferencing* lookup_depth(state_t* site, Dereferencing* chain);
Dereferencing* lookup_final(state_t* site, Dereferencing* chain);

class Dereferencing {
private:
	CA* __ca;
	Dereferencing* __up_chain;
	size_t __level;
	size_t __index;
public:
	lookup_function next;
	Dereferencing** next_level;
	state_t state;
	Dereferencing(CA* ca, Dereferencing* up_chain, const size_t chain_index, const char* str_rule, const size_t str_rule_size);
	~Dereferencing();
};

class Rule {
private:
	CA* __ca;
	size_t __entries_size;
	Dereferencing** __entries;
	void __update_entries(const size_t entries, Dereferencing* entry_chain = nullptr);
public:
	Dereferencing** first_entry;
	Dereferencing** last_entry;
	Rule(CA* ca);
	~Rule();
	CA& ca();
	void new_entry(const char* str_rule, const size_t str_rule_len, const size_t entries);
	void clear_entries();
	void transition(Conf* conf_from, Conf* conf_to, const random_generator_function g, Seed* seed);
	size_t entries();
};

