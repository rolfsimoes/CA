#define DECLSPEC __declspec(dllimport)

#include <iomanip>
#include <string>
#include <sstream>
#include "..\ca-lib\gkl.h"

typedef check_return(*cmd_func)(const uint_t*);
typedef void(*param_func)();
enum commands_params_index {
	command_no_args = -1,
	command_index_size_state0,
	command_index_size_state1,
	command_index_size_state2,
	command_index_size_state3,
	command_index_total_parts,
	command_index_noise_parts,
	command_index_max_time,
	command_index_stead_state
};
enum measure_params_index {
	measure_index_experiments = 8
};
enum density_params_index {
	density_index_transient = 8,
	density_index_experiments
};
enum recoveryrate_index {
	recoveryrate_index_damage_size = 8,
	recoveryrate_index_damaging_states,
	recoveryrate_index_majority_state,
	recoveryrate_index_experiments
};

void __shuffle_ic();

struct param_struct {
	const char * param_name;
	const uint_t arg_index;
	const param_func option_func;
};
struct cmd_struct {
	const char * cmd_name;
	const param_struct * cmd_params;
	const uint_t cmd_params_size;
	uint_t * cmd_args;
	const cmd_func exprm_func;
};

const int timespace_params_size = 13;
const int measure_params_size = 12;
const int density_params_size = 13;
const int recoveryrate_params_size = 15;

const param_struct timespace_params[timespace_params_size]{
	{ "/size_st0",		command_index_size_state0,	nullptr },
	{ "/st0",			command_index_size_state0,	nullptr },
	{ "/size_st1",		command_index_size_state1,	nullptr },
	{ "/st1",			command_index_size_state1,	nullptr },
	{ "/size_st2",		command_index_size_state2,	nullptr },
	{ "/st2",			command_index_size_state2,	nullptr },
	{ "/size_st3",		command_index_size_state3,	nullptr },
	{ "/st3",			command_index_size_state3,	nullptr },
	{ "/parts",			command_index_total_parts,	nullptr },
	{ "/noise_parts",	command_index_noise_parts,	nullptr },
	{ "/max_time",		command_index_max_time,		nullptr },
	{ "/stead",			command_index_stead_state,	nullptr },
	{ "/shuffle_ic",	command_no_args,			__shuffle_ic}
};
const param_struct measure_params[measure_params_size]{
	{ "/size_st0",		command_index_size_state0,	nullptr },
	{ "/st0",			command_index_size_state0,	nullptr },
	{ "/size_st1",		command_index_size_state1,	nullptr },
	{ "/st1",			command_index_size_state1,	nullptr },
	{ "/size_st2",		command_index_size_state2,	nullptr },
	{ "/st2",			command_index_size_state2,	nullptr },
	{ "/size_st3",		command_index_size_state3,	nullptr },
	{ "/st3",			command_index_size_state3,	nullptr },
	{ "/parts",			command_index_total_parts,	nullptr },
	{ "/noise_parts",	command_index_noise_parts,	nullptr },
	{ "/max_time",		command_index_max_time,		nullptr },
	{ "/experiments",	measure_index_experiments,	nullptr }
};
const param_struct density_params[density_params_size]{
	{ "/size_st0",		command_index_size_state0,	nullptr },
	{ "/st0",			command_index_size_state0,	nullptr },
	{ "/size_st1",		command_index_size_state1,	nullptr },
	{ "/st1",			command_index_size_state1,	nullptr },
	{ "/size_st2",		command_index_size_state2,	nullptr },
	{ "/st2",			command_index_size_state2,	nullptr },
	{ "/size_st3",		command_index_size_state3,	nullptr },
	{ "/st3",			command_index_size_state3,	nullptr },
	{ "/parts",			command_index_total_parts,	nullptr },
	{ "/noise_parts",	command_index_noise_parts,	nullptr },
	{ "/max_time",		command_index_max_time,		nullptr },
	{ "/transient",		density_index_transient,	nullptr },
	{ "/experiments",	density_index_experiments,	nullptr }
};
const param_struct recoveryrate_params[recoveryrate_params_size]{
	{ "/size_st0",		command_index_size_state0,	nullptr },
	{ "/st0",			command_index_size_state0,	nullptr },
	{ "/size_st1",		command_index_size_state1,	nullptr },
	{ "/st1",			command_index_size_state1,	nullptr },
	{ "/size_st2",		command_index_size_state2,	nullptr },
	{ "/st2",			command_index_size_state2,	nullptr },
	{ "/size_st3",		command_index_size_state3,	nullptr },
	{ "/st3",			command_index_size_state3,	nullptr },
	{ "/parts",			command_index_total_parts,	nullptr },
	{ "/noise_parts",	command_index_noise_parts,	nullptr },
	{ "/max_time",		command_index_max_time,		nullptr },
	{ "/damage_size",	recoveryrate_index_damage_size,		nullptr },
	{ "/damage_states",	recoveryrate_index_damaging_states,	nullptr },
	{ "/majority",		recoveryrate_index_majority_state,	nullptr },
	{ "/experiments",	recoveryrate_index_experiments,		nullptr }
};

const int timespace_args_size = 8;
const int measure_args_size = 9;
const int density_args_size = 10;
const int recoveryrate_args_size = 12;

uint_t timespace_args[timespace_args_size] = {
	80,
	0,
	0,
	0,
	1000,
	10,
	2400,
	2
};
uint_t measure_args[measure_args_size] = {
	400,
	0,
	0,
	0,
	1000,
	10,
	2400,
	2,
	200
};
uint_t density_args[density_args_size] = {
	400,
	0,
	0,
	0,
	1000,
	10,
	2400,
	2,
	400,
	100
};
uint_t recoveryrate_args[recoveryrate_args_size] = {
	400,
	0,
	0,
	0,
	1000,
	10,
	2400,
	2,
	150,
	123,
	0,
	100
};

bool __gkl_timespace_shuffle_ic = false;
check_return gkl_timespace_experiment(const uint_t args[]) {
	CA * ca = create_ca_gkl(args[command_index_size_state0],
		args[command_index_size_state1],
		args[command_index_size_state2],
		args[command_index_size_state3],
		args[command_index_stead_state]);
	if (!ca)
		return false;
	if (!gkl4_rules(ca,
		args[command_index_total_parts],
		args[command_index_noise_parts])) {
		destroy_CA(ca);
		return false;
	};
	if (__gkl_timespace_shuffle_ic)
		CA_new_random_ic(ca);
	time_space(ca,
		args[command_index_max_time],
		args[command_index_stead_state]);
	destroy_CA(ca);
	return true;
}
check_return gkl_measure_experiment(const uint_t args[]) {
	CA * ca = create_ca_gkl(args[command_index_size_state0],
		args[command_index_size_state1],
		args[command_index_size_state2],
		args[command_index_size_state3]);
	if (!ca)
		return false;
	if (!gkl4_rules(ca,
		args[command_index_total_parts],
		args[command_index_noise_parts])) {
		destroy_CA(ca);
		return false;
	};
	measure(ca,
		measure_args[command_index_max_time],
		measure_args[measure_index_experiments]);
	destroy_CA(ca);
	return true;
}
check_return gkl_density_experiment(const uint_t args[]) {
	CA * ca = create_ca_gkl(args[command_index_size_state0],
		args[command_index_size_state1],
		args[command_index_size_state2],
		args[command_index_size_state3]);
	if (!ca)
		return false;
	if (!gkl4_rules(ca,
		args[command_index_total_parts],
		args[command_index_noise_parts])) {
		destroy_CA(ca);
		return false;
	};
	density(ca,
		density_args[command_index_max_time],
		density_args[density_index_transient],
		density_args[density_index_experiments]);
	destroy_CA(ca);
	return true;
}
check_return gkl_recoveryrate_experiment(const uint_t args[]) {
	CA * ca = create_ca_gkl(args[command_index_size_state0],
		args[command_index_size_state1],
		args[command_index_size_state2],
		args[command_index_size_state3]);
	if (!ca)
		return false;
	if (!gkl4_rules(ca,
		args[command_index_total_parts],
		args[command_index_noise_parts])) {
		destroy_CA(ca);
		return false;
	};
	std::string damaging_states(std::to_string(args[recoveryrate_index_damaging_states]));
	instant_recovery_rate(ca,
		args[recoveryrate_index_damage_size],
		damaging_states.c_str(),
		(uint_t)damaging_states.length(),
		static_cast<char_state_t>(args[recoveryrate_index_majority_state] + ASCII_TO_UCHAR_CONF),
		args[command_index_max_time],
		args[recoveryrate_index_experiments]);
	destroy_CA(ca);
	return true;
}

const int gkl_commands_size = 4;
const cmd_struct gkl_commands[gkl_commands_size] = {
	{ "timespace", timespace_params, timespace_params_size, timespace_args, gkl_timespace_experiment },
	{ "measure", measure_params, measure_params_size, measure_args, gkl_measure_experiment },
	{ "density", density_params, density_params_size, density_args, gkl_density_experiment },
	{ "recoveryrate", recoveryrate_params, recoveryrate_params_size, recoveryrate_args, gkl_recoveryrate_experiment }
};

check_return __print_help(const cmd_struct * commands, const uint_t commands_size) {
	for (uint_t i = 0; i < commands_size; ++i) {
		std::cout << '\n' << commands[i].cmd_name;
		for (uint_t j = 0; j < commands[i].cmd_params_size; ++j)
			std::cout << '\n' << '\t' << commands[i].cmd_params[j].param_name;
	}
	return false;
}

void __shuffle_ic() {
	__gkl_timespace_shuffle_ic = true;
}

enum parse_state {
	parse_param_init,
	parse_param_name,
	parse_param_value,
	parse_param_not_found
};

check_return parse_cmd_params(int argc, 
	char ** argv, 
	const cmd_struct * command) {

	parse_state parse_state = parse_param_init;
	int arg_index = 0;
	for (int i = 2; i < argc; ++i) {
		std::string argument(argv[i]);
		if (parse_state == parse_param_init || parse_state == parse_param_value) {
			for (uint_t j = 0; j < command->cmd_params_size; ++j) {
				if (argument.compare(command->cmd_params[j].param_name) == 0) {
					arg_index = command->cmd_params[j].arg_index;
					if (arg_index != command_no_args)
						parse_state = parse_param_name;
					else {
						parse_state = parse_param_value;
						if (command->cmd_params[j].option_func)
							command->cmd_params[j].option_func();
					}
					break;
				}
				parse_state = parse_param_not_found;
			}
		}
		else if (parse_state == parse_param_name) {
			std::istringstream value_string(argument);
			value_string >> command->cmd_args[arg_index];
			parse_state = parse_param_value;
			continue;
		}
		else
			return false;
	}
	return command->exprm_func(command->cmd_args);
}

check_return parse_cmd(int argc, 
	char ** argv,
	const cmd_struct * commands,
	const uint_t commands_size) {

	if (argc == 1) 
		return __print_help(commands, commands_size);
	parse_state parse_state = parse_param_init;
	std::string argument(argv[1]);
	int command_index = 0;
	for (uint_t i = 0; i < commands_size; ++i) {
		if (argument.compare(commands[i].cmd_name) == 0) {
			command_index = i;
			parse_state = parse_param_name;
			break;
		}
	}
	if (parse_state == parse_param_name) {
		return parse_cmd_params(argc, argv, &commands[command_index]);
	}
	else
		return false;
	return true;
}

int main(int argc, char ** argv) {
	std::cout.precision(std::numeric_limits<double>::digits10);
	new_random_seed();
	if (!parse_cmd(argc, argv, gkl_commands, gkl_commands_size))
		return -1;
	return 0;
}