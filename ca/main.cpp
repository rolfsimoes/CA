#include "ca.h"
#include "rule.h"
#include <iostream>

void experiment1() {
	CA ca;
	Seed seed;
	Conf* conf[2]; 
	conf[0] = new Conf(&ca);
	conf[0]->add(5, '0');
	conf[0]->add(5, '1');
	conf[0]->shuffle(JKISS_random_gen, &seed);
	conf[1] = new Conf(*conf[0]);
	Rule rule(&ca);
	rule.new_entry("0000313102000200333331312122212233333131212221220000313121222122", 64, 1);
	for (size_t t = 0; t < conf[0]->length * 6; ++t) {
		rule.transition(conf[t & 1], conf[(t + 1) & 1], JKISS_random_gen, &seed);
		for (size_t i = 0; i < conf[0]->length * 4; ++i)
			std::cout << conf[t & 1]->first_site[i % conf[0]->length] + UCHAR_CONF_TO_ASCII;
		if (*conf[0] == *conf[1]) break;
		std::cout << '\n';
	}
	delete conf[0];
	delete conf[1];
}

int main() {
	experiment1();
}