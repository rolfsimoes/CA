#include "rule.h"
#include <cmath>

Rule * create_rule(const uint_t radius,
	const uint_t states) {

	if (states < 2)
		return nullptr;
	Rule * new_rule = new Rule;
	if (!new_rule) 
		return nullptr;
	new_rule->radius = radius;
	new_rule->states = states;
	uint_t rule_table_size = static_cast<uint_t>(states * (pow(states, 2 * radius + 1) - 1) / (states - 1));
	new_rule->rule_table = new state_t[rule_table_size];
	if (!new_rule->rule_table) {
		delete new_rule;
		return nullptr;
	}
	return new_rule;
}

void destroy_rule(Rule * rule) {
	delete[] rule->rule_table;
	delete rule;
}

void load_string_rule(Rule * rule,
	const char_state_t * str_rule,
	const uint_t str_rule_size) {

	uint_t rule_table_position = 0;
	uint_t max_level = 2 * rule->radius + 1;
	for (uint_t level = 1; level <= max_level; ++level) {
		uint_t level_base = static_cast<uint_t>(rule->states * (pow(rule->states, level) - 1) / (rule->states - 1));
		uint_t max_level_index = static_cast<uint_t>(pow(rule->states, level));
		for (uint_t level_index = 0; level_index < max_level_index; ++level_index) {
			if (level < max_level)
				rule->rule_table[rule_table_position] = static_cast<state_t>(level_base + rule->states * level_index);
			else if (level_index < str_rule_size)
				rule->rule_table[rule_table_position] = static_cast<state_t>(str_rule[level_index] - ASCII_TO_UCHAR_CONF);
			++rule_table_position;
		}
	}
}

std::ostream& operator<<(std::ostream& stream, 
	const Rule& rule) {

	uint_t values_level = 2 * rule.radius + 1;
	uint_t values_level_index = static_cast<uint_t>(rule.states * (pow(rule.states, values_level - 1) - 1) / (rule.states - 1));
	uint_t values_level_length = static_cast<uint_t>(pow(rule.states, values_level));
	for (uint_t i = values_level_index; i < values_level_index + values_level_length; ++i) {
		char_state_t char_state = static_cast<char_state_t>(rule.rule_table[i] + ASCII_TO_UCHAR_CONF);
		stream << char_state;
	}
	return stream;
}

