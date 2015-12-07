#pragma once

#include "conf.h"

class __declspec(dllimport) Rule_Tree {
public:
	size_t __down_trees_size;
	const size_t index;
	state_t __state;
	Rule_Tree* (*lookup)(state_t*, Rule_Tree*);
	Rule_Tree** down_trees;
	Rule_Tree(size_t radius, size_t states, Rule_Tree* up_tree, size_t level);
	~Rule_Tree();
	void load_string_rule(char* str_rule, size_t str_rule_size);
};

