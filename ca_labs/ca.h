#pragma once

#include "kiss.h"
#include "conf.h"
#include "rule.h"

class __declspec(dllexport) CA
{
public:
	const size_t radius;
	const size_t states;
	size_t __rules_size;
	Rule_Tree** __rules;
	const size_t confs_size;
	Conf** const confs;
	Seed seed;
	Conf ic;
	void __update_rules(Rule_Tree* rule_tree, size_t entries);
	CA(size_t radius, size_t states, size_t confs_size);
	~CA();
	void update_confs_transition();
	void add_rule(char* str_rule, size_t str_rule_size, size_t entries);
	void clear_rules();
	void transition(size_t step);
	bool is_stead_state(size_t step, size_t last_steps);
};

