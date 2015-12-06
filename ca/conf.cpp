#include "conf.h"

void Conf::__update_sites(const size_t increase, const state_t state) {
	state_t* new_periodic_border_first_site = new state_t[length + 2 * __ca->radius + increase];
	if (new_periodic_border_first_site) {
		for (size_t i = __ca->radius; i < length + __ca->radius; ++i) new_periodic_border_first_site[i] = __periodic_border_first_site[i];
		for (size_t i = length + __ca->radius; i < length + __ca->radius + increase; ++i) new_periodic_border_first_site[i] = state - ASCII_TO_UCHAR_CONF;
		delete __periodic_border_first_site;
		__periodic_border_first_site = new_periodic_border_first_site;
		first_site = __periodic_border_first_site + __ca->radius;
		length += increase;
		last_site = first_site + length;
		update_periodic_border();
	}
}

Conf::Conf(CA* ca): __ca(ca), length(0), __periodic_border_first_site(new state_t[length + 2 * __ca->radius]), first_site(__periodic_border_first_site + __ca->radius), last_site(first_site + length) {
}

Conf::Conf(Conf& other): __ca(other.__ca), length(other.length), __periodic_border_first_site(new state_t[length + 2 * __ca->radius]), first_site(__periodic_border_first_site + __ca->radius), last_site(first_site + length) {
	for (size_t i = 0; i < length + 2 * __ca->radius; ++i) __periodic_border_first_site[i] = other.__periodic_border_first_site[i];
}

Conf::~Conf() {
	delete __periodic_border_first_site;
}

CA& Conf::ca() {
	return *__ca;
}

void Conf::add(const size_t increase, const state_t state) {
	__update_sites(increase, state);
}

void Conf::clear() {
	length = 0;
	__update_sites(0);
}

void Conf::shuffle(const random_generator_function g, Seed* seed) {
	for (size_t i = length + __ca->radius - 1; i > __ca->radius; --i) {
		state_t* exchange_site = first_site + (g(seed) % (i - 1));
		state_t exchange_value = *exchange_site;
		*exchange_site = first_site[i];
		first_site[i] = exchange_value;
	}
	update_periodic_border();
}

void Conf::update_periodic_border() {
	for (size_t i = 0; i < __ca->radius; ++i) {
		__periodic_border_first_site[i] = last_site[i - __ca->radius];
		__periodic_border_first_site[__ca->radius + length + i] = first_site[i];
	}
}

bool Conf::operator==(const Conf& other) {
	for (size_t i = 0; i < length; ++i)
		if (first_site[i] != other.first_site[i]) return false;
	return true;
}