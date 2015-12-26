#include "gkl.h"

CA * create_ca_gkl(const uint_t size_state0,
	const uint_t size_state1,
	const uint_t size_state2,
	const uint_t size_state3,
	const uint_t conf_size) {

	uint_t actual_conf_size = 2;
	if (conf_size > 2)
		actual_conf_size = conf_size;
	//const char_state_t str_map[] = "\x1A\x1B\x19\x18";
	const char_state_t str_map[] = "0123";
	Configuration * ic = create_configuration(1, 0, str_map, 4);
	append_sites(ic, size_state0, '0');
	append_sites(ic, size_state1, '1');
	append_sites(ic, size_state2, '2');
	append_sites(ic, size_state3, '3');
	uint_t states = 4;
	CA * ca = create_CA(states, actual_conf_size, ic);
	if (!ca)
		return nullptr;
	std::cout << "\n%GKL_iv";
	std::cout << "\n%CA_size=" << ca->ic->size;
	return ca;
}

check_return gkl4_rules(CA * ca,
	const uint_t total_parts,
	const uint_t noise_parts) {

	CA_clear_rules(ca);
	if (!CA_add_rule(ca, GKL4, 64, total_parts, noise_parts, major_part))
		return false;
	CA_add_rule(ca, ZERO64, 64, total_parts, noise_parts, minor_part);
	CA_add_rule(ca, ONE64, 64, total_parts, noise_parts, minor_part);
	CA_add_rule(ca, TWO64, 64, total_parts, noise_parts, minor_part);
	CA_add_rule(ca, THREE64, 64, total_parts, noise_parts, minor_part);

	// print-out variables
	std::cout << "\n%noise=" << ((1.0 * noise_parts) / total_parts);
	return true;
}

