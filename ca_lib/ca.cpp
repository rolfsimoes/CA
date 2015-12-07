#include "ca.h"

void CA::__update_rules(Rule_Tree* rule_tree, size_t entries) {
	Rule_Tree** new_rules = new Rule_Tree*[__rules_size + entries];
	if (new_rules) {
		for (size_t i = 0; i < __rules_size; ++i) new_rules[i] = __rules[i];
		for (size_t i = __rules_size; i < __rules_size + entries; ++i) new_rules[i] = rule_tree;
		delete __rules;
		__rules = new_rules;
		__rules_size += entries;
	}
}

CA::CA(size_t radius, size_t states, size_t confs_size) :
	radius(radius),
	states(states),
	__rules_size(0),
	__rules(new Rule_Tree*[__rules_size]),
	confs_size(confs_size),
	confs(new Conf*[confs_size]),
	ic(radius) {
	for (size_t i = 0; i < confs_size; ++i)
		confs[i] = new Conf(radius);
}

CA::~CA() {
	Rule_Tree* last_rule_deleted = nullptr;
	for (size_t i = 0; i < __rules_size; ++i)
		if (__rules[i] != last_rule_deleted) {
			last_rule_deleted = __rules[i];
			delete __rules[i];
		}
	delete __rules;
	for (size_t i = 0; i < confs_size; ++i)
		delete confs[i];
	delete confs;
}

void CA::update_confs_transition() {
	for (size_t i = 0; i < confs_size; ++i)
		delete confs[i];
	for (size_t i = 0; i < confs_size; ++i)
		confs[i] = new Conf(ic);
}

void CA::add_rule(char* str_rule, size_t str_rule_size, size_t entries) {
	Rule_Tree* new_rule = new Rule_Tree(radius, states, nullptr, 0);
	new_rule->load_string_rule(str_rule, str_rule_size);
	if (new_rule) __update_rules(new_rule, entries);
}

void CA::clear_rules() {
	__rules_size = 0;
	__update_rules(nullptr, 0);
}

void CA::transition(size_t step) {
	Conf* conf_from = confs[step % confs_size];
	Conf* conf_to = confs[(step + 1) % confs_size];
	if (__rules_size > 1) {
		for (size_t i = 0; i < conf_from->__size; ++i) {
			Rule_Tree* entry_rule = __rules[seed.JKISS_random_gen() % __rules_size];
			conf_to->__first_site[i] = entry_rule->lookup(conf_from->__first_site + i,
				*(entry_rule->down_trees + *(conf_from->__first_site + i - radius) - ASCII_TO_UCHAR_CONF_TRANSITION))->__state;
		}
		conf_to->update_periodic_border();
	}
	else if (__rules_size > 0) {
		for (size_t i = 0; i < conf_from->__size; ++i) {
			Rule_Tree* entry_rule = __rules[0];
			conf_to->__first_site[i] = entry_rule->lookup(conf_from->__first_site + i,
				*(entry_rule->down_trees + *(conf_from->__first_site + i - radius) - ASCII_TO_UCHAR_CONF_TRANSITION))->__state;
		}
		conf_to->update_periodic_border();
	}
}

bool CA::is_stead_state(size_t step, size_t last_steps) {
	if (last_steps > confs_size) last_steps = confs_size;
	if (step < last_steps - 1) return false;
	for (size_t i = 1; i < confs_size; ++i) {
		last_steps -= *confs[step % confs_size] == *confs[(step + confs_size - i) % confs_size];
		if (last_steps == 1) return true;
	}
	return false;
}