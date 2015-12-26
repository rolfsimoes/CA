#pragma once
#ifndef DECLSPEC
#define DECLSPEC __declspec(dllexport)
#endif

#include "jkiss.h"
#include "conf.h"
#include "rule.h"

typedef enum {
	major_part,
	minor_part
} CA_rule_entries_part;

struct CA {
	uint_t radius;
	Configuration * ic;
	uint_t states;
	uint_t rules_size;
	Rule ** rules;
	uint_t confs_size;
	Configuration ** confs;
};

DECLSPEC CA * create_CA(const uint_t states,
	const uint_t confs_size,
	const Configuration * ic);

DECLSPEC void destroy_CA(CA * ca);

DECLSPEC check_return CA_add_rule(CA * ca,
	const char_state_t * str_rule,
	const uint_t str_rule_size,
	const uint_t total_parts,
	const uint_t noise_parts,
	const CA_rule_entries_part entries_part);

DECLSPEC check_return CA_clear_rules(CA* ca);

DECLSPEC bool CA_is_stead_state(const CA * ca,
	const uint_t step, 
	const uint_t last_steps);

DECLSPEC void CA_transition(Configuration ** confs,
	const uint_t confs_size,
	Rule ** rules,
	const uint_t rules_size,
	const uint_t step);

DECLSPEC void CA_new_random_ic(CA * ca);

DECLSPEC void time_space(CA * ca,
	const uint_t max_time,
	const uint_t stead_state = 2);

DECLSPEC void measure(CA * ca,
	const uint_t max_time,
	const uint_t experiments);

DECLSPEC void density(CA * ca,
	const uint_t max_time,
	const uint_t measure_after,
	const uint_t experiments);

DECLSPEC void instant_recovery_rate(CA * ca,
	const uint_t damage_size,
	const char_state_t * damaging_char_states,
	const uint_t damaging_states_size,
	const char_state_t majority_char_state,
	const uint_t max_time,
	const uint_t experiments);