#pragma once
#define DECLSPEC __declspec(dllimport)

#include <iomanip>
#include <string>
#include <sstream>
#include "..\ca-lib\gkl.h"


enum commands_params_index {
	param_no_arg = -1,
};
enum timespace_params_index {
	timespace_arg_size_state0,
	timespace_arg_size_state1,
	timespace_arg_size_state2,
	timespace_arg_size_state3,
	timespace_arg_total_parts,
	timespace_arg_noise_parts,
	timespace_arg_max_time,
	timespace_arg_stead_state,
	timespace_arg_shuffle_ic
};
enum measure_params_index {
	measure_arg_size_state0,
	measure_arg_size_state1,
	measure_arg_size_state2,
	measure_arg_size_state3,
	measure_arg_total_parts,
	measure_arg_noise_parts,
	measure_arg_max_time,
	measure_arg_experiments
};
enum density_params_index {
	density_arg_size_state0,
	density_arg_size_state1,
	density_arg_size_state2,
	density_arg_size_state3,
	density_arg_total_parts,
	density_arg_noise_parts,
	density_arg_max_time,
	density_arg_transient,
	density_arg_experiments
};
enum recoveryrate_index {
	recoveryrate_arg_size_state0,
	recoveryrate_arg_size_state1,
	recoveryrate_arg_size_state2,
	recoveryrate_arg_size_state3,
	recoveryrate_arg_total_parts,
	recoveryrate_arg_noise_parts,
	recoveryrate_arg_max_time,
	recoveryrate_arg_damage_size,
	recoveryrate_arg_damaging_states,
	recoveryrate_arg_majority_state,
	recoveryrate_arg_experiments
};

enum parse_state {
	parse_state_init,
	parse_state_get_option,
	parse_state_not_found
};


typedef std::string arg_value_t;

struct arg_struct {
	const char * description;
	arg_value_t value;
	bool informed;
	bool required;
};

const int timespace_args_size = 9;
const int measure_args_size = 8;
const int density_args_size = 9;
const int recoveryrate_args_size = 11;

arg_struct timespace_args[timespace_args_size] = {
	{ "state 0 lenght",		arg_value_t("80"),		false,	false },
	{ "state 1 lenght",		arg_value_t("0"),		false,	false },
	{ "state 2 lenght",		arg_value_t("0"),		false,	false },
	{ "state 3 lenght",		arg_value_t("0"),		false,	false },
	{ "noise total parts",	arg_value_t("1000"),	false,	false },
	{ "noise parts",		arg_value_t("10"),		false,	false },
	{ "max steps",			arg_value_t("2400"),	false,	false },
	{ "stead state",		arg_value_t("2"),		false,	false },
	{ "shuffle ic",			arg_value_t("0"),		false,	false }
};
arg_struct measure_args[measure_args_size] = {
	{ "state 0 lenght",			arg_value_t("400"),		false,	false },
	{ "state 1 lenght",			arg_value_t("0"),		false,	false },
	{ "state 2 lenght",			arg_value_t("0"),		false,	false },
	{ "state 3 lenght",			arg_value_t("0"),		false,	false },
	{ "noise total parts",		arg_value_t("1000"),	false,	false },
	{ "noise parts",			arg_value_t("10"),		false,	false },
	{ "max steps",				arg_value_t("2400"),	false,	false },
	{ "number of experiments",	arg_value_t("200"),		false,	false }
};
arg_struct density_args[density_args_size] = {
	{ "state 0 lenght",				arg_value_t("400"),		false,	false },
	{ "state 1 lenght",				arg_value_t("0"),		false,	false },
	{ "state 2 lenght",				arg_value_t("0"),		false,	false },
	{ "state 3 lenght",				arg_value_t("0"),		false,	false },
	{ "noise total parts",			arg_value_t("1000"),	false,	false },
	{ "noise parts",				arg_value_t("10"),		false,	false },
	{ "max steps",					arg_value_t("2400"),	false,	false },
	{ "measure after some steps",	arg_value_t("400"),		false,	false },
	{ "number of experiments",		arg_value_t("100"),		false,	false }
};
arg_struct recoveryrate_args[recoveryrate_args_size] = {
	{ "state 0 lenght",			arg_value_t("400"),		false,	false },
	{ "state 1 lenght",			arg_value_t("0"),		false,	false },
	{ "state 2 lenght",			arg_value_t("0"),		false,	false },
	{ "state 3 lenght",			arg_value_t("0"),		false,	false },
	{ "noise total parts",		arg_value_t("1000"),	false,	false },
	{ "noise parts",			arg_value_t("10"),		false,	false },
	{ "max steps",				arg_value_t("2400"),	false,	false },
	{ "damage size",			arg_value_t("150"),		false,	false },
	{ "damaging states",		arg_value_t("123"),		false,	false },
	{ "majority state",			arg_value_t("0"),		false,	false },
	{ "number of experiments",	arg_value_t("100"),		false,	false }
};

typedef check_return(*param_func)();

struct param_struct {
	const char * name;
	const int arg_index;
	const arg_value_t arg_value;
	arg_struct * args_var;
	const uint_t args_var_size;
	param_struct * sub_params;
	const uint_t sub_params_size;
	const param_func option_func;
	const char * help;
};


check_return gkl_timespace_experiment();
check_return gkl_measure_experiment();
check_return gkl_density_experiment();
check_return gkl_recoveryrate_experiment();


check_return help();
check_return parse_params(int argc,
	char ** argv,
	int * next_arg,
	param_struct * params,
	const uint_t params_size,
	parse_state parse_state);


const int timespace_params_size = 14;
const int measure_params_size = 12;
const int density_params_size = 13;
const int recoveryrate_params_size = 15;

param_struct timespace_params[timespace_params_size]{
	{ "/size_st0",		timespace_arg_size_state0, arg_value_t(""),  timespace_args, timespace_args_size, nullptr, 0, nullptr, "" },
	{ "/st0",			timespace_arg_size_state0, arg_value_t(""),  timespace_args, timespace_args_size, nullptr, 0, nullptr, "" },
	{ "/size_st1",		timespace_arg_size_state1, arg_value_t(""),  timespace_args, timespace_args_size, nullptr, 0, nullptr, "" },
	{ "/st1",			timespace_arg_size_state1, arg_value_t(""),  timespace_args, timespace_args_size, nullptr, 0, nullptr, "" },
	{ "/size_st2",		timespace_arg_size_state2, arg_value_t(""),  timespace_args, timespace_args_size, nullptr, 0, nullptr, "" },
	{ "/st2",			timespace_arg_size_state2, arg_value_t(""),  timespace_args, timespace_args_size, nullptr, 0, nullptr, "" },
	{ "/size_st3",		timespace_arg_size_state3, arg_value_t(""),  timespace_args, timespace_args_size, nullptr, 0, nullptr, "" },
	{ "/st3",			timespace_arg_size_state3, arg_value_t(""),  timespace_args, timespace_args_size, nullptr, 0, nullptr, "" },
	{ "/parts",			timespace_arg_total_parts, arg_value_t(""),  timespace_args, timespace_args_size, nullptr, 0, nullptr, "" },
	{ "/noise_parts",	timespace_arg_noise_parts, arg_value_t(""),  timespace_args, timespace_args_size, nullptr, 0, nullptr, "" },
	{ "/max_time",		timespace_arg_max_time,	   arg_value_t(""),  timespace_args, timespace_args_size, nullptr, 0, nullptr, "" },
	{ "/stead",			timespace_arg_stead_state, arg_value_t(""),  timespace_args, timespace_args_size, nullptr, 0, nullptr, "" },
	{ "/shuffle_ic",	timespace_arg_shuffle_ic,  arg_value_t("1"), timespace_args, timespace_args_size, nullptr, 0, nullptr, "" },
	{ "/shuffle",		timespace_arg_shuffle_ic,  arg_value_t("1"), timespace_args, timespace_args_size, nullptr, 0, nullptr, "" }
};
param_struct measure_params[measure_params_size]{
	{ "/size_st0",		measure_arg_size_state0, arg_value_t(""), measure_args, measure_args_size, nullptr, 0, nullptr, "" },
	{ "/st0",			measure_arg_size_state0, arg_value_t(""), measure_args, measure_args_size, nullptr, 0, nullptr, "" },
	{ "/size_st1",		measure_arg_size_state1, arg_value_t(""), measure_args, measure_args_size, nullptr, 0, nullptr, "" },
	{ "/st1",			measure_arg_size_state1, arg_value_t(""), measure_args, measure_args_size, nullptr, 0, nullptr, "" },
	{ "/size_st2",		measure_arg_size_state2, arg_value_t(""), measure_args, measure_args_size, nullptr, 0, nullptr, "" },
	{ "/st2",			measure_arg_size_state2, arg_value_t(""), measure_args, measure_args_size, nullptr, 0, nullptr, "" },
	{ "/size_st3",		measure_arg_size_state3, arg_value_t(""), measure_args, measure_args_size, nullptr, 0, nullptr, "" },
	{ "/st3",			measure_arg_size_state3, arg_value_t(""), measure_args, measure_args_size, nullptr, 0, nullptr, "" },
	{ "/parts",			measure_arg_total_parts, arg_value_t(""), measure_args, measure_args_size, nullptr, 0, nullptr, "" },
	{ "/noise_parts",	measure_arg_noise_parts, arg_value_t(""), measure_args, measure_args_size, nullptr, 0, nullptr, "" },
	{ "/max_time",		measure_arg_max_time,	 arg_value_t(""), measure_args, measure_args_size, nullptr, 0, nullptr, "" },
	{ "/experiments",	measure_arg_experiments, arg_value_t(""), measure_args, measure_args_size, nullptr, 0, nullptr, "" }
};
param_struct density_params[density_params_size]{
	{ "/size_st0",		density_arg_size_state0, arg_value_t(""), density_args, density_args_size, nullptr, 0, nullptr, "" },
	{ "/st0",			density_arg_size_state0, arg_value_t(""), density_args, density_args_size, nullptr, 0, nullptr, "" },
	{ "/size_st1",		density_arg_size_state1, arg_value_t(""), density_args, density_args_size, nullptr, 0, nullptr, "" },
	{ "/st1",			density_arg_size_state1, arg_value_t(""), density_args, density_args_size, nullptr, 0, nullptr, "" },
	{ "/size_st2",		density_arg_size_state2, arg_value_t(""), density_args, density_args_size, nullptr, 0, nullptr, "" },
	{ "/st2",			density_arg_size_state2, arg_value_t(""), density_args, density_args_size, nullptr, 0, nullptr, "" },
	{ "/size_st3",		density_arg_size_state3, arg_value_t(""), density_args, density_args_size, nullptr, 0, nullptr, "" },
	{ "/st3",			density_arg_size_state3, arg_value_t(""), density_args, density_args_size, nullptr, 0, nullptr, "" },
	{ "/parts",			density_arg_total_parts, arg_value_t(""), density_args, density_args_size, nullptr, 0, nullptr, "" },
	{ "/noise_parts",	density_arg_noise_parts, arg_value_t(""), density_args, density_args_size, nullptr, 0, nullptr, "" },
	{ "/max_time",		density_arg_max_time,	 arg_value_t(""), density_args, density_args_size, nullptr, 0, nullptr, "" },
	{ "/transient",		density_arg_transient,	 arg_value_t(""), density_args, density_args_size, nullptr, 0, nullptr, "" },
	{ "/experiments",	density_arg_experiments, arg_value_t(""), density_args, density_args_size, nullptr, 0, nullptr, "" }
};
param_struct recoveryrate_params[recoveryrate_params_size]{
	{ "/size_st0",		recoveryrate_arg_size_state0,	  arg_value_t(""), recoveryrate_args, recoveryrate_args_size, nullptr, 0, nullptr, "" },
	{ "/st0",			recoveryrate_arg_size_state0,	  arg_value_t(""), recoveryrate_args, recoveryrate_args_size, nullptr, 0, nullptr, "" },
	{ "/size_st1",		recoveryrate_arg_size_state1,	  arg_value_t(""), recoveryrate_args, recoveryrate_args_size, nullptr, 0, nullptr, "" },
	{ "/st1",			recoveryrate_arg_size_state1,	  arg_value_t(""), recoveryrate_args, recoveryrate_args_size, nullptr, 0, nullptr, "" },
	{ "/size_st2",		recoveryrate_arg_size_state2,	  arg_value_t(""), recoveryrate_args, recoveryrate_args_size, nullptr, 0, nullptr, "" },
	{ "/st2",			recoveryrate_arg_size_state2,	  arg_value_t(""), recoveryrate_args, recoveryrate_args_size, nullptr, 0, nullptr, "" },
	{ "/size_st3",		recoveryrate_arg_size_state3,	  arg_value_t(""), recoveryrate_args, recoveryrate_args_size, nullptr, 0, nullptr, "" },
	{ "/st3",			recoveryrate_arg_size_state3,	  arg_value_t(""), recoveryrate_args, recoveryrate_args_size, nullptr, 0, nullptr, "" },
	{ "/parts",			recoveryrate_arg_total_parts,	  arg_value_t(""), recoveryrate_args, recoveryrate_args_size, nullptr, 0, nullptr, "" },
	{ "/noise_parts",	recoveryrate_arg_noise_parts,	  arg_value_t(""), recoveryrate_args, recoveryrate_args_size, nullptr, 0, nullptr, "" },
	{ "/max_time",		recoveryrate_arg_max_time,		  arg_value_t(""), recoveryrate_args, recoveryrate_args_size, nullptr, 0, nullptr, "" },
	{ "/damage_size",	recoveryrate_arg_damage_size,	  arg_value_t(""), recoveryrate_args, recoveryrate_args_size, nullptr, 0, nullptr, "" },
	{ "/damage_states",	recoveryrate_arg_damaging_states, arg_value_t(""), recoveryrate_args, recoveryrate_args_size, nullptr, 0, nullptr, "" },
	{ "/majority",		recoveryrate_arg_majority_state,  arg_value_t(""), recoveryrate_args, recoveryrate_args_size, nullptr, 0, nullptr, "" },
	{ "/experiments",	recoveryrate_arg_experiments,	  arg_value_t(""), recoveryrate_args, recoveryrate_args_size, nullptr, 0, nullptr, "" }
};


const int gkl_commands_size = 7;
param_struct gkl_commands[gkl_commands_size] = {
	{ "timespace",	  param_no_arg, arg_value_t(""), nullptr, 0, timespace_params,	  timespace_params_size,	gkl_timespace_experiment,	 "" },
	{ "measure",	  param_no_arg, arg_value_t(""), nullptr, 0, measure_params,	  measure_params_size,		gkl_measure_experiment,		 "" },
	{ "density",	  param_no_arg, arg_value_t(""), nullptr, 0, density_params,	  density_params_size,		gkl_density_experiment,		 "" },
	{ "recoveryrate", param_no_arg, arg_value_t(""), nullptr, 0, recoveryrate_params, recoveryrate_params_size, gkl_recoveryrate_experiment, "" },
	{ "/help",		  param_no_arg, arg_value_t(""), nullptr, 0, nullptr,			  0,						help,						 "" },
	{ "-help",		  param_no_arg, arg_value_t(""), nullptr, 0, nullptr,			  0,						help,						 "" },
	{ "--help",		  param_no_arg, arg_value_t(""), nullptr, 0, nullptr,			  0,						help,						 "" },
};


void __print_help_params(param_struct * params, const uint_t params_size) {
	for (uint_t i = 0; i < params_size; ++i) {
		std::cout << "\n" << params[i].name;
		if (params[i].arg_index != param_no_arg) {
			std::clog << "\t- " << params[i].args_var[params[i].arg_index].description;
			if (!params[i].args_var[params[i].arg_index].required)
				std::clog << ". Default: " << params[i].args_var[params[i].arg_index].value;
			else
				std::clog << ". (Required)";
		}
		std::cout << "\n  " << params[i].help;
		if (params[i].sub_params && params[i].sub_params_size) {
			std::clog << "\nParameter " << params[i].name << " option(s):";
			__print_help_params(params[i].sub_params, params[i].sub_params_size);
		}
	}
};

check_return help() {
	__print_help_params(gkl_commands, gkl_commands_size);
	return false;
};

check_return parse_params(int argc,
	char ** argv,
	int * next_arg,
	param_struct * params,
	const uint_t params_size,
	parse_state parse_state) {

	check_return result;
	std::string argument;
	while (*next_arg < argc) {
		argument = argv[*next_arg];
		if (parse_state == parse_state_init || parse_state == parse_state_get_option) {
			parse_state = parse_state_not_found;
			for (uint_t i = 0; i < params_size; ++i) {
				if (argument.compare(params[i].name) == 0) {
					if (params[i].arg_index != param_no_arg) {
						if (params[i].arg_value.compare("") != 0) {
							params[i].args_var[params[i].arg_index].value = params[i].arg_value;
							params[i].args_var[params[i].arg_index].informed = true;
						}
						else {
							(*next_arg)++;
							if (*next_arg < argc) {
								params[i].args_var[params[i].arg_index].value = argv[*next_arg];
								params[i].args_var[params[i].arg_index].informed = true;
							}
							else
								return false;
						}
					}
					(*next_arg)++;
					if (params[i].sub_params && params[i].sub_params_size) {
						result = parse_params(argc, argv,
							next_arg,
							params[i].sub_params,
							params[i].sub_params_size,
							parse_state_get_option);
						if (!result)
							return false;
					}
					if (params[i].option_func) {
						result = params[i].option_func();
						if (!result)
							return false;
					}
					parse_state = parse_state_get_option;
					break;
				}
			}
			// only one lineage descendent along the parameters' tree is allowed;
			if (parse_state == parse_state_not_found)
				return false;
		}
	}
	return (parse_state != parse_state_init);
};
