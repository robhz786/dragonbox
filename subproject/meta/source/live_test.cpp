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

#include <iostream>
#include <iomanip>
#include <string>

#if defined(__cpp_if_constexpr)
#	define JKJ_IF_CONSTEXPR if constexpr
#else
#	define JKJ_IF_CONSTEXPR if
#	if defined(_MSC_VER)
#		pragma warning( disable : 4127 ) // "Conditional expression is constant"
#	endif
#endif

inline void parse_float(const std::string& str, float& x) {
	x = std::stof(str);
}
inline void parse_float(const std::string& str, double& x) {
	x = std::stod(str);
}

template <class Float>
static void live_test()
{
	char buffer[41];

	while (true) {
		Float x;
		std::string x_str;
		while (true) {
			std::getline(std::cin, x_str);
			try {
				parse_float(x_str, x);
			}
			catch (...) {
				std::cout << "Not a valid input; input again.\n";
				continue;
			}
			break;
		}

		auto xx = jkj::dragonbox::float_bits<Float>{ x };
		std::cout << "              sign: " << (xx.is_negative() ? "-" : "+") << std::endl;
		std::cout << "     exponent bits: " << "0x" << std::hex << std::setfill('0')
			<< xx.extract_exponent_bits() << std::dec
			<< " (value: " << xx.binary_exponent() << ")\n";
		std::cout << "  significand bits: " << "0x" << std::hex << std::setfill('0')
			<< std::setw(sizeof(Float) * 2)
			<< xx.extract_significand_bits()
			<< " (value: 0x" << xx.binary_significand() << ")\n" << std::dec;

		jkj::dragonbox::to_chars(x, buffer);
		std::cout << "  Dragonbox output: " << buffer << "\n\n";
	}
}

int main()
{
	constexpr enum {
		test_float,
		test_double
	} test = test_double;

	JKJ_IF_CONSTEXPR(test == test_float) {
		std::cout << "[Start live test for float's]\n";
		live_test<float>();
	}
	else JKJ_IF_CONSTEXPR(test == test_double) {
		std::cout << "[Start live test for double's]\n";
		live_test<double>();
	}
}
