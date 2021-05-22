// Copyright 2020 Junekey Jeon
//
// The contents of this file may be used under the terms of
// the Apache License v2.0 with LLVM Exceptions.
//
//    (See accompanying file LICENSE-Apache or copy at
//     https://llvm.org/foundation/relicensing/LICENSE.txt)
//
// Alternatively, the contents of this file may be used under the terms of
// the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE-Boost or copy at
//     https://www.boost.org/LICENSE_1_0.txt)
//
// Unless required by applicable law or agreed to in writing, this software
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.

#include "dragonbox/dragonbox_to_chars.h"
#include "random_float.h"
#include "ryu/ryu.h"
#include <iostream>
#include <cstring>


inline void ryu_to_chars(float x, char* dest) {
	f2s_buffered(x, dest);
}
inline void ryu_to_chars(double x, char* dest) {
	d2s_buffered(x, dest);
}

template <class Float, class TypenameString>
static bool uniform_random_test(std::size_t number_of_tests, TypenameString&& type_name_string)
{
	char buffer1[64];
	char buffer2[64];
	auto rg = generate_correctly_seeded_mt19937_64();
	bool success = true;
	for (std::size_t test_idx = 0; test_idx < number_of_tests; ++test_idx) {
		auto x = uniformly_randomly_generate_general_float<Float>(rg);

		// Check if the output is identical to that of Ryu
		jkj::dragonbox::to_chars(x, buffer1);
		ryu_to_chars(x, buffer2);

		if (std::strcmp(buffer1, buffer2) != 0) {
			std::cout << "Error detected! [Ryu = " << buffer2
				<< ", Dragonbox = " << buffer1 << "]\n";
			success = false;
		}
	}

	if (success) {
		std::cout << "Uniform random test for " << type_name_string
			<< " with " << number_of_tests << " examples succeeded.\n";
	}

	return success;
}

int main()
{
	constexpr bool run_float = true;
	constexpr std::size_t number_of_uniform_random_tests_float = 10000000;

	constexpr bool run_double = true;
	constexpr std::size_t number_of_uniform_random_tests_double = 10000000;

	bool success = true;

	if (run_float) {
		std::cout << "[Testing uniformly randomly generated float inputs...]\n";
		success &= uniform_random_test<float>(number_of_uniform_random_tests_float, "float");
		std::cout << "Done.\n\n\n";
	}
	if (run_double) {
		std::cout << "[Testing uniformly randomly generated double inputs...]\n";
		success &= uniform_random_test<double>(number_of_uniform_random_tests_double, "double");
		std::cout << "Done.\n\n\n";
	}

	if (!success) {
		return -1;
	}
}
