#include "conf.h"

void Conf::__update_sites(const size_t increase, const state_t state) {
	state_t* new_periodic_border_first_site = new state_t[__size + 2 * radius + increase];
	if (new_periodic_border_first_site) {
		for (size_t i = radius; i < __size + radius; ++i) new_periodic_border_first_site[i] = __periodic_sites[i];
		for (size_t i = __size + radius; i < __size + radius + increase; ++i) new_periodic_border_first_site[i] = state - ASCII_TO_UCHAR_CONF;
		delete __periodic_sites;
		__periodic_sites = new_periodic_border_first_site;
		__first_site = __periodic_sites + radius;
		__size += increase;
		__last_site = __first_site + __size;
		update_periodic_border();
	}
}

Conf::Conf(size_t radius): 
	__size(0), 
	radius(radius), 
	__periodic_sites(new state_t[__size + 2 * radius]), 
	__first_site(__periodic_sites + radius), 
	__last_site(__first_site + __size) {
}

Conf::Conf(Conf& other): 
	__size(other.__size), 
	radius(other.radius), 
	__periodic_sites(new state_t[__size + 2 * radius]), 
	__first_site(__periodic_sites + radius), 
	__last_site(__first_site + __size) {
	for (size_t i = 0; i < __size + 2 * radius; ++i) __periodic_sites[i] = other.__periodic_sites[i];
}

Conf::~Conf() {
	delete __periodic_sites;
}

void Conf::add_sites(const size_t increase, const state_t state) {
	__update_sites(increase, state);
}

void Conf::clear() {
	__size = 0;
	__update_sites(0);
}

void Conf::shuffle(Seed* seed) {
	for (size_t i = __size - 1; i > 1; --i) {
		state_t* exchange_site = __first_site + (seed->JKISS_random_gen() % i);
		state_t exchange_value = *exchange_site;
		*exchange_site = __first_site[i];
		__first_site[i] = exchange_value;
	}
	update_periodic_border();
}

void Conf::update_periodic_border() {
	for (size_t i = 0; i < radius; ++i) {
		__periodic_sites[i] = __last_site[i - radius];
		__periodic_sites[radius + __size + i] = __first_site[i];
	}
}

size_t Conf::count_state(state_t state) {
	size_t result = 0;
	state -= ASCII_TO_UCHAR_CONF;
	for (state_t* site = __first_site; site != __last_site; ++site)
		result += (*site == state);
	return result;
}

bool Conf::operator==(const Conf& other) {
	for (size_t i = 0; i < __size; ++i)
		if (__first_site[i] != other.__first_site[i]) return false;
	return true;
}


Conf_Char_Map::Conf_Char_Map(const state_t char_map[]) :
	states_char_map(char_map) {
}

Conf_Char_Map::~Conf_Char_Map() {
}

Conf_Char_Map& Conf_Char_Map::operator()(Conf* conf) {
	__conf = conf;
	return *this;
}