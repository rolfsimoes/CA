#include "ca.h"

__inline uint_t __gcd(const uint_t num1,
	const uint_t num2) {

	return num2 == 0 ? num1 : __gcd(num2, num1 % num2);
}

CA * create_CA(const uint_t states,
	const uint_t confs_size,
	const Configuration * ic) {

	CA * new_CA = new CA;
	if (!new_CA) 
		return nullptr;
	new_CA->ic = create_configuration(ic->radius, ic->size, ic->char_map, ic->char_map_size);
	if (!new_CA->ic) {
		delete new_CA;
		return nullptr;
	}
	copy_configuration(new_CA->ic, ic);
	new_CA->radius = ic->radius;
	new_CA->states = states;
	new_CA->rules_size = 0;
	new_CA->rules = new Rule*[new_CA->rules_size];
	if (!new_CA->rules) {
		delete new_CA;
		return nullptr;
	}
	new_CA->confs = new Configuration*[confs_size];
	if (!new_CA->confs) {
		delete[] new_CA->rules;
		delete new_CA;
		return nullptr;
	}
	new_CA->confs_size = confs_size;
	for (uint_t i = 0; i < confs_size; ++i)
		new_CA->confs[i] = create_configuration(ic->radius, ic->size, ic->char_map, ic->char_map_size);
	copy_configuration(new_CA->confs[0], new_CA->ic);
	return new_CA;
}

void destroy_CA(CA * ca) {
	Rule * last_rule_deleted = nullptr;
	for (uint_t i = 0; i < ca->rules_size; ++i)
		if (ca->rules[i] != last_rule_deleted) {
			last_rule_deleted = ca->rules[i];
			destroy_rule(ca->rules[i]);
		}
	delete[] ca->rules;
	for (uint_t i = 0; i < ca->confs_size; ++i)
		destroy_configuration(ca->confs[i]);
	delete[] ca->confs;
	destroy_configuration(ca->ic);
}

check_return CA_add_rule(CA * ca, 
	const char_state_t * str_rule,
	const uint_t str_rule_size,
	const uint_t total_parts,
	const uint_t noise_parts,
	const CA_rule_entries_part entries_part) {

	uint_t entries;
	if ((total_parts < noise_parts) || (total_parts < 1))
		return false;
	if (entries_part == major_part)
		entries = ((total_parts - noise_parts) * ca->states) / __gcd(((total_parts - noise_parts) * ca->states), noise_parts);
	else if (entries_part == minor_part)
		entries = (noise_parts) / __gcd(((total_parts - noise_parts) * ca->states), noise_parts);
	else
		return false;
	if (entries < 1)
		return false;
	Rule ** new_rules = new Rule*[ca->rules_size + entries];
	if (!new_rules) 
		return false;
	Rule * rule = create_rule(ca->radius, ca->states);
	if (!rule) {
		delete[] new_rules;
		return false;
	}
	load_string_rule(rule, str_rule, str_rule_size);
	for (uint_t i = 0; i < ca->rules_size; ++i) 
		new_rules[i] = ca->rules[i];
	for (uint_t i = ca->rules_size; i < ca->rules_size + entries; ++i) 
		new_rules[i] = rule;
	delete[] ca->rules;
	ca->rules = new_rules;
	ca->rules_size += entries;
	return true;
}

check_return CA_clear_rules(CA* ca) {
	if (ca->rules_size == 0) 
		return true;
	Rule ** new_ca_rules = new Rule*[0];
	if (!new_ca_rules) 
		return false;
	Rule * last_rule_deleted = nullptr;
	for (uint_t i = 0; i < ca->rules_size; ++i)
		if (ca->rules[i] != last_rule_deleted) {
			last_rule_deleted = ca->rules[i];
			destroy_rule(ca->rules[i]);
		}
	delete[] ca->rules;
	ca->rules = new_ca_rules;
	ca->rules_size = 0;
	return true;
}

bool CA_is_stead_state(const CA * ca, 
	const uint_t step, 
	const uint_t last_steps) {

	uint_t stead_state_steps = last_steps;
	if (stead_state_steps < 2)
		return false;
	if (stead_state_steps > ca->confs_size)
		stead_state_steps = ca->confs_size;
	if (step < stead_state_steps - 1)
		return false;
	for (uint_t i = 1; i < ca->confs_size; ++i) {
		stead_state_steps -= are_equal_configurations(ca->confs[step % ca->confs_size], ca->confs[(int)(step + ca->confs_size - i) % ca->confs_size]);
		if (stead_state_steps == 1)
			return true;
	}
	return false;
}

void CA_transition(Configuration ** confs,
	const uint_t confs_size,
	Rule ** rules,
	const uint_t rules_size,
	const uint_t step) {

	Configuration * conf_from = confs[step % confs_size];
	Configuration * conf_to = confs[(step + 1) % confs_size];
	state_t * periodic_sites_from = conf_from->periodic_sites;
	state_t * first_site_to = conf_to->first_site;
	state_t * last_site_to = conf_to->last_site;
	uint_t loop_lookups = 2 * conf_from->radius - 1;
	state_t * neighborhood_sites_from;
	state_t * rule_table;
	state_t lookup;
	if (rules_size > 1)
		while (first_site_to != last_site_to) {
			neighborhood_sites_from = periodic_sites_from;
			rule_table = rules[jkiss_generator(rules_size)]->rule_table;
			lookup = rule_table[*neighborhood_sites_from++];
			for (uint_t i = loop_lookups; i > 0; --i)
				lookup = rule_table[lookup + *neighborhood_sites_from++];
			*first_site_to = rule_table[lookup + *neighborhood_sites_from];
			++first_site_to;
			++periodic_sites_from;
		}
	else if (rules_size > 0)
		while (first_site_to != last_site_to) {
			neighborhood_sites_from = periodic_sites_from;
			rule_table = rules[0]->rule_table;
			lookup = rule_table[*neighborhood_sites_from++];
			for (uint_t i = loop_lookups; i > 0; --i)
				lookup = rule_table[lookup + *neighborhood_sites_from++];
			*first_site_to = rule_table[lookup + *neighborhood_sites_from];
			++first_site_to;
			++periodic_sites_from;
		}
	update_periodic_border(conf_to);
}

void CA_new_random_ic(CA * ca) {
	if (ca->confs_size) {
		copy_configuration(ca->confs[0], ca->ic);
		shuffle_configuration(ca->confs[0]);
	}
}

__inline void __log_progress() {
	std::clog << '.';
}

void time_space(CA * ca,
	const uint_t max_time,
	const uint_t stead_state) {

	// verify conditions
	if ((ca->ic->size < 1) || (ca->confs_size < 1))
		return;
	if (max_time < 1)
		return;

	// execute experiment && print-out
	for (uint_t step = 0; step < max_time; ++step) {
		CA_transition(ca->confs, ca->confs_size, ca->rules, ca->rules_size, step);
		std::cout << '\n' << step << ' ' << *ca->confs[step % ca->confs_size];
		if (CA_is_stead_state(ca, step, stead_state)) break;
	}

	// free-up variables
}

void measure(CA * ca,
	const uint_t max_time,
	const uint_t experiments) {

	// verify conditions
	if ((ca->ic->size < 1) || (ca->confs_size < 1))
		return;
	if ((max_time < 1) || (experiments < 1))
		return;

	// create variables
	uint_t * result = new uint_t[ca->states];
	for (state_t state = 0; state < ca->states; ++state)
		result[state] = 0;

	// execute experiments
	std::clog << '\n';
	for (uint_t i = 0; i < experiments; ++i) {
		new_random_seed();
		CA_new_random_ic(ca);
		__log_progress();
		uint_t step;
		for (step = 0; step < max_time; ++step) {
			CA_transition(ca->confs, ca->confs_size, ca->rules, ca->rules_size, step);
			if (CA_is_stead_state(ca, step, 2))
				break;
		}
		Configuration * last_conf = ca->confs[(step + 1) % ca->confs_size];
		for (state_t state = 0; state < ca->states; ++state)
			result[state] += count_conf_state(last_conf, state);
	}

	// print-out variables
	std::cout << "\n%value_description=\"classification task efficiency measure\"";
	std::cout << "\n%experiments=" << experiments;
	std::cout << "\n%max_time=" << max_time;
	for (uint_t state = 0; state < ca->states; ++state) {
		std::cout << "\n%state=" << state;
		std::cout << "\n%initial_density=" << ((1.0 * count_conf_state(ca->ic, state)) / ca->ic->size);
		std::cout << "\n" << (((1.0 * result[state]) / ca->ic->size) / experiments);
	}

	// free-up variables
	delete[] result;
}

void density(CA * ca,
	const uint_t max_time,
	const uint_t measure_after,
	const uint_t experiments) {

	// verify conditions
	if ((ca->ic->size < 1) || (ca->confs_size < 1))
		return;
	if ((max_time < 1) || (experiments < 1))
		return;

	// create variables
	uint_t ** result = new uint_t*[ca->states];
	for (uint_t state = 0; state < ca->states; ++state) {
		result[state] = new uint_t[ca->ic->size + 1];
		for (uint_t i = 0; i < ca->ic->size + 1; ++i)
			result[state][i] = 0;
	}

	// execute experiments
	Configuration** ca_confs = ca->confs;
	uint_t ca_confs_size = ca->confs_size;
	std::clog << '\n';
	for (uint_t i = 0; i < experiments; ++i) {
		new_random_seed();
		CA_new_random_ic(ca);
		__log_progress();
		uint_t step;
		for (step = 0; step < max_time + measure_after; ++step) {
			CA_transition(ca->confs, ca->confs_size, ca->rules, ca->rules_size, step);
			if (step >= measure_after)
				for (uint_t state = 0; state < ca->states; ++state)
					++result[state][count_conf_state(ca_confs[(step + 1) % ca_confs_size], state)];
		}
	}

	// print-out variables
	std::cout << "\n%value_description=\"probability to find a given CA density\"";
	std::cout << "\n%experiments=" << experiments;
	std::cout << "\n%max_time=" << max_time;
	std::cout << "\n%measuring_after=" << measure_after;
	for (uint_t state = 0; state < ca->states; ++state) {
		std::cout << "\n%state=" << state;
		std::cout << "\n%state_count_from=" << 0;
		for (uint_t i = 0; i < ca->ic->size + 1; ++i)
			std::cout << '\n' << (((1.0 * result[state][i]) / max_time) / experiments);
		std::cout << "\n%state_count_to=" << ca->ic->size;
	}

	// free-up variables
	for (uint_t state = 0; state < ca->states; ++state)
		delete[] result[state];
	delete[] result;
}

void instant_recovery_rate(CA * ca,
	const uint_t damage_size,
	const char_state_t * damaging_char_states,
	const uint_t damaging_states_size,
	const char_state_t majority_char_state,
	const uint_t max_time,
	const uint_t experiments) {

	// verify conditions
	if ((ca->ic->size < 1) || (ca->confs_size < 1))
		return;
	if ((max_time < 1) || (experiments < 1))
		return;

	// create variables
	uint_t actual_damage_size = damage_size;
	if (actual_damage_size > ca->ic->size)
		actual_damage_size = ca->ic->size;
	state_t  majority_state = static_cast<state_t>(majority_char_state - ASCII_TO_UCHAR_CONF);
	int** state_step_count = new int*[ca->states];
	int** result_experiment = new int*[ca->states];
	int** result = new int*[ca->states];
	for (uint_t state = 0; state < ca->states; ++state) {
		state_step_count[state] = new int[2];
		result_experiment[state] = new int[max_time];
		result[state] = new int[max_time];
		// initialization
		result_experiment[state][0] = 0;
		for (uint_t i = 0; i < max_time; ++i)
			result[state][i] = 0;
	}

	uint_t max_step = 0;
	// execute experiments
	std::clog << '\n';
	for (uint_t i = 0; i < experiments; ++i) {
		__log_progress();
		new_random_seed();
		CA_new_random_ic(ca);
		damage_configuration(ca->confs[0],
			damage_size,
			damaging_char_states,
			damaging_states_size);
		// experiment initialization variables
		for (state_t state = 0; state < ca->states; ++state) {
			state_step_count[state][0] = static_cast<int>(count_conf_state(ca->confs[0], state));
			state_step_count[state][1] = 0;
		}
		uint_t step;
		for (step = 0; step < max_time - 1; ++step) {
			CA_transition(ca->confs, ca->confs_size, ca->rules, ca->rules_size, step);
			for (state_t state = 0; state < ca->states; ++state) {
				state_step_count[state][(step + 1) & 1] = static_cast<int>(count_conf_state(ca->confs[(step + 1) % ca->confs_size], state));
				result_experiment[state][step + 1] = state_step_count[state][(step + 1) & 1] - state_step_count[state][step & 1];
			}
			if (CA_is_stead_state(ca, step + 1, 2))
				break;
		}
		// is a valid experiment only if CA converged to the majority state
		if ((count_conf_state(ca->confs[(step + 1) % ca->confs_size], majority_state) >(ca->ic->size - damage_size))) {
			for (uint_t j = 0; j < step; ++j)
				for (uint_t state = 0; state < ca->states; ++state)
					result[state][j] += result_experiment[state][j];
			if (max_step < step) max_step = step;
		}
	}
	if (max_step < 1) max_step = 1;

	// print-out variables
	std::cout << "\n%value_description=\"instantaneous recovery rate mean of a given time step over the subset of correctly classified experiments\"";
	std::cout << "\n%experiments=" << experiments;
	std::cout << "\n%max_time=" << max_time;
	std::cout << "\n%damage_size=" << damage_size;
	std::cout << "\n%damaging_states=\"" << damaging_char_states << '\"';
	for (uint_t state = 0; state < ca->states; ++state) {
		std::cout << "\n%state=" << state;
		std::cout << "\n%time_from=" << 0;
		for (uint_t i = 0; i < max_step; ++i)
			std::cout << '\n' << ((1.0 * result[state][i]) / ca->ic->size) / experiments;
		std::cout << "\n%time_to=" << max_step;
	}
	// free-up variables
	for (uint_t state = 0; state < ca->states; ++state) {
		delete[] state_step_count[state];
		delete[] result_experiment[state];
		delete[] result[state];
	}
	delete[] state_step_count;
	delete[] result_experiment;
	delete[] result;
}
