#include "main.h"

__inline int __arg_to_uint(const arg_value_t * arg_value) {
	uint_t result;
	std::stringstream arg_converter(*arg_value);
	arg_converter >> result;
	return result;
};

check_return gkl_timespace_experiment() {
	CA * ca = create_ca_gkl4(__arg_to_uint(&timespace_args[timespace_arg_size_state0].value),
		__arg_to_uint(&timespace_args[timespace_arg_size_state1].value),
		__arg_to_uint(&timespace_args[timespace_arg_size_state2].value),
		__arg_to_uint(&timespace_args[timespace_arg_size_state3].value),
		__arg_to_uint(&timespace_args[timespace_arg_stead_state].value));
	if (!ca)
		return false;
	if (!gkl4_rules(ca,
		__arg_to_uint(&timespace_args[timespace_arg_total_parts].value),
		__arg_to_uint(&timespace_args[timespace_arg_noise_parts].value))) {
		destroy_CA(ca);
		return false;
	};
	if (__arg_to_uint(&timespace_args[timespace_arg_shuffle_ic].value))
		CA_new_random_ic(ca);
	time_space(ca,
		__arg_to_uint(&timespace_args[timespace_arg_max_time].value),
		__arg_to_uint(&timespace_args[timespace_arg_stead_state].value));
	destroy_CA(ca);
	return true;
}
check_return gkl_measure_experiment() {
	CA * ca = create_ca_gkl4(__arg_to_uint(&measure_args[measure_arg_size_state0].value),
		__arg_to_uint(&measure_args[measure_arg_size_state1].value),
		__arg_to_uint(&measure_args[measure_arg_size_state2].value),
		__arg_to_uint(&measure_args[measure_arg_size_state3].value));
	if (!ca)
		return false;
	if (!gkl4_rules(ca,
		__arg_to_uint(&measure_args[measure_arg_total_parts].value),
		__arg_to_uint(&measure_args[measure_arg_noise_parts].value))) {
		destroy_CA(ca);
		return false;
	};
	measure(ca,
		__arg_to_uint(&measure_args[measure_arg_max_time].value),
		__arg_to_uint(&measure_args[measure_arg_experiments].value));
	destroy_CA(ca);
	return true;
}
check_return gkl_density_experiment() {
	CA * ca = create_ca_gkl4(__arg_to_uint(&density_args[density_arg_size_state0].value),
		__arg_to_uint(&density_args[density_arg_size_state1].value),
		__arg_to_uint(&density_args[density_arg_size_state2].value),
		__arg_to_uint(&density_args[density_arg_size_state3].value));
	if (!ca)
		return false;
	if (!gkl4_rules(ca,
		__arg_to_uint(&density_args[density_arg_total_parts].value),
		__arg_to_uint(&density_args[density_arg_noise_parts].value))) {
		destroy_CA(ca);
		return false;
	};
	density(ca,
		__arg_to_uint(&density_args[density_arg_max_time].value),
		__arg_to_uint(&density_args[density_arg_transient].value),
		__arg_to_uint(&density_args[density_arg_experiments].value));
	destroy_CA(ca);
	return true;
}
check_return gkl_recoveryrate_experiment() {
	CA * ca = create_ca_gkl4(__arg_to_uint(&density_args[recoveryrate_arg_size_state0].value),
		__arg_to_uint(&density_args[recoveryrate_arg_size_state1].value),
		__arg_to_uint(&density_args[recoveryrate_arg_size_state2].value),
		__arg_to_uint(&density_args[recoveryrate_arg_size_state3].value));
	if (!ca)
		return false;
	if (!gkl4_rules(ca,
		__arg_to_uint(&density_args[recoveryrate_arg_total_parts].value),
		__arg_to_uint(&density_args[recoveryrate_arg_noise_parts].value))) {
		destroy_CA(ca);
		return false;
	};
	instant_recovery_rate(ca,
		__arg_to_uint(&density_args[recoveryrate_arg_damage_size].value),
		density_args[recoveryrate_arg_damaging_states].value.c_str(),
		static_cast<uint_t>(density_args[recoveryrate_arg_damaging_states].value.length()),
		static_cast<char_state_t>(__arg_to_uint(&density_args[recoveryrate_arg_majority_state].value) + ASCII_TO_UCHAR_CONF),
		__arg_to_uint(&density_args[recoveryrate_arg_max_time].value),
		__arg_to_uint(&density_args[recoveryrate_arg_experiments].value));
	destroy_CA(ca);
	return true;
}

int main(int argc, char ** argv) {
	std::cout.precision(std::numeric_limits<double>::digits10);
	new_random_seed();
	int next_arg = 1;
	if (!parse_params(argc, argv, &next_arg, gkl_commands, gkl_commands_size, parse_state_init))
		return -1;
	return 0;
}