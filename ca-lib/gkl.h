#pragma once
#ifndef DECLSPEC
#define DECLSPEC __declspec(dllexport)
#endif

#include "ca.h"

#define GKL4	"0000313102000200333331312122212233333131212221220000313121222122"
#define ZERO64	"0000000000000000000000000000000000000000000000000000000000000000"
#define ONE64	"1111111111111111111111111111111111111111111111111111111111111111"
#define TWO64	"2222222222222222222222222222222222222222222222222222222222222222"
#define THREE64 "3333333333333333333333333333333333333333333333333333333333333333"

DECLSPEC CA * create_ca_gkl(const uint_t size_state0,
	const uint_t size_state1,
	const uint_t size_state2,
	const uint_t size_state3,
	const uint_t conf_size = 2);

DECLSPEC check_return gkl4_rules(CA * ca,
	const uint_t total_parts,
	const uint_t noise_parts);
