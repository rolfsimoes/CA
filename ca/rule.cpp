#include "rule.h"

Dereferencing* lookup_depth(state_t* site, Dereferencing* chain) {
	return (*(chain->next))(site + 1, *(chain->next_level + *site - ASCII_TO_UCHAR_CONF_TRANSITION));
}

Dereferencing* lookup_final(state_t* site, Dereferencing* chain) {
	return chain;
}


Dereferencing::Dereferencing(CA* ca, Dereferencing* up_chain, const size_t chain_index, const char* str_rule, const size_t str_rule_size) :
	__ca(ca), __up_chain(up_chain), 
	__level(__up_chain ? __up_chain->__level + 1 : 0), 
	__index(__ca && __up_chain ? __up_chain->__index + chain_index * static_cast<size_t>(pow(__ca->states, 2 * __ca->radius + 1 - __level)) : 0),
	next(__ca ? (__level < 2 * __ca->radius + 1 - 1 ? &lookup_depth : &lookup_final) : nullptr),
	next_level(__ca ? (__level < 2 * __ca->radius + 1 ? new Dereferencing*[__ca->states] : nullptr) : nullptr), 
	state(__ca ? (__level < 2 * __ca->radius + 1 ? -1 : (__index < str_rule_size ? static_cast<state_t>(str_rule[__index] - ASCII_TO_UCHAR_CONF) : 0)) : -1) {
	if (next_level) {
		for (size_t i = 0; i < __ca->states; ++i)
			next_level[i] = new Dereferencing(__ca, this, i, str_rule, str_rule_size);
	}
}

Dereferencing::~Dereferencing() {
	if (next_level) {
		for (size_t i = 0; i < __ca->states; ++i)
			delete next_level[i];
		delete next_level;
	}
}

void Rule::__update_entries(const size_t entries, Dereferencing* entry_chain) {
	Dereferencing** new_entries = new Dereferencing*[__entries_size + entries];
	if (new_entries) {
		for (size_t i = 0; i < __entries_size; ++i) new_entries[i] = __entries[i];
		for (size_t i = __entries_size; i < __entries_size + entries; ++i) new_entries[i] = entry_chain;
		delete __entries;
		__entries = new_entries;
		first_entry = __entries;
		__entries_size += entries;
		last_entry = first_entry + __entries_size;
	}
}

Rule::Rule(CA* ca): __ca(ca), __entries_size(0), __entries(new Dereferencing*[__entries_size]) {
}


Rule::~Rule() {
	delete __entries;
}

CA& Rule::ca() {
	return *__ca;
}

void Rule::new_entry(const char* str_rule, const size_t str_rule_len, const size_t entries) {
	Dereferencing* entry_chain = new Dereferencing(__ca, nullptr, 0, str_rule, str_rule_len);
	__update_entries(entries, entry_chain);
}

void Rule::clear_entries() {
	__entries_size = 0;
	__update_entries(0);
}

void Rule::transition(Conf* conf_from, Conf* conf_to, const random_generator_function g, Seed* seed) {
	if (__entries_size) {
		for (size_t i = 0; i < conf_from->length; ++i)
			conf_to->first_site[i] = lookup_depth(conf_from->first_site + i,
				*(first_entry[g(seed) % __entries_size]->next_level + *(conf_from->first_site + i - __ca->radius) - ASCII_TO_UCHAR_CONF_TRANSITION))->state;
		conf_to->update_periodic_border();
	}
}

size_t Rule::entries() {
	return __entries_size;
}
