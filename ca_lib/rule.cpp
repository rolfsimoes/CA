#include "rule.h"

Rule_Tree* lookup_depth(state_t* site, Rule_Tree* chain) {
	return (*chain->lookup)(site + 1, *(chain->down_trees + *site - ASCII_TO_UCHAR_CONF_TRANSITION));
}

Rule_Tree* lookup_final(state_t* site, Rule_Tree* chain) {
	return chain;
}

Rule_Tree::Rule_Tree(size_t radius, size_t states, Rule_Tree* up_tree, size_t level) :
	__down_trees_size(0),
	index(up_tree ? up_tree->index + (up_tree->__down_trees_size - 1) * static_cast<size_t>(pow(states, 2 * radius + 1 - level)) : 0),
	__state(-1),
	lookup(level < 2 * radius + 1 - 1 ? &lookup_depth : &lookup_final),
	down_trees(level < 2 * radius + 1 ? new Rule_Tree*[states] : nullptr) {
	if (down_trees) {
		for (size_t i = 0; i < states; ++i) {
			++__down_trees_size;
			down_trees[i] = new Rule_Tree(radius, states, this, level + 1);
		}
	}
}

Rule_Tree::~Rule_Tree() {
	if (down_trees)
		for (size_t i = 0; i < __down_trees_size; ++i)
			delete down_trees[i];
}

void Rule_Tree::load_string_rule(char* str_rule, size_t str_rule_size) {
	if (__down_trees_size)
		for (size_t i = 0; i < __down_trees_size; ++i)
			down_trees[i]->load_string_rule(str_rule, str_rule_size);
	else if (index < str_rule_size)
		__state = static_cast<state_t>(str_rule[index] - ASCII_TO_UCHAR_CONF);
}