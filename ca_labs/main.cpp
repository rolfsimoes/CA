#include "ca.h"
#include "rule.h"

void experiment1() {
	const state_t str_map[] = "\x1A\x1B\x19\x18";
	Conf_Char_Map state_to_char(str_map);
	CA ca(1, 4, 660);
	ca.ic.add_sites(50, '0');
	ca.ic.add_sites(60, '1');
	ca.ic.shuffle(&ca.seed);
	ca.update_confs_transition();
	ca.add_rule("0000313102000200333331312122212233333131212221220000313121222122", 64, 192);
	ca.add_rule("0000000000000000000000000000000000000000000000000000000000000000", 64, 1);
	ca.add_rule("1111111111111111111111111111111111111111111111111111111111111111", 64, 1);
	ca.add_rule("2222222222222222222222222222222222222222222222222222222222222222", 64, 1);
	ca.add_rule("3333333333333333333333333333333333333333333333333333333333333333", 64, 1);
	for (size_t t = 0; t < ca.ic.__size * 6; ++t) {
		std::cout << t << ' ' << state_to_char(ca.confs[t % ca.confs_size]) << ' ' << ca.confs[t % ca.confs_size]->count_state('0') << '\n';
		ca.transition(t);
		if (ca.is_stead_state(t, 3)) break;
	}
	ca.clear_rules();
}

int main() {
	experiment1();
}