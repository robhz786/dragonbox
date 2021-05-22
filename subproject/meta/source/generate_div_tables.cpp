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

#include "dragonbox/dragonbox.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace jkj::dragonbox::detail;
using namespace jkj::dragonbox::detail::div;

template <class UInt>
UInt modular_inverse(UInt a, unsigned int bit_width = value_bits<UInt>) noexcept {
	// By Euler's theorem, a^phi(2^n) == 1 (mod 2^n),
	// where phi(2^n) = 2^(n-1), so the modular inverse of a is
	// a^(2^(n-1) - 1) = a^(1 + 2 + 2^2 + ... + 2^(n-2)).
	std::common_type_t<UInt, unsigned int> mod_inverse = 1;
	for (unsigned int i = 1; i < bit_width; ++i) {
		mod_inverse = mod_inverse * mod_inverse * a;
	}
	if (bit_width < value_bits<UInt>) {
		auto mask = UInt((UInt(1) << bit_width) - 1);
		return UInt(mod_inverse & mask);
	}
	else {
		return UInt(mod_inverse);
	}
}

template <class UInt>
void fill_inv_and_quotients(std::size_t size, UInt a, UInt* mod_inv, UInt* max_quotients) {
	const auto mod_inverse = modular_inverse<UInt>(a);
	std::common_type_t<UInt, unsigned int> pow_of_mod_inverse = 1;
	UInt pow_of_a = 1;
	for (std::size_t i = 0; i < size; ++i) {
		mod_inv[i] = UInt(pow_of_mod_inverse);
		max_quotients[i] = UInt(std::numeric_limits<UInt>::max() / pow_of_a);

		pow_of_mod_inverse *= mod_inverse;
		pow_of_a *= a;
	}
}

template <typename UInt>
void print_inv_and_quotients(std::ostream& out, const char* uint_name, UInt divisor, std::size_t size) {
	std::vector<UInt> mod_inv(size);
	std::vector<UInt> max_quotients(size);

	fill_inv_and_quotients<UInt>(size, divisor, &mod_inv[0], &max_quotients[0]);

	out << "\tstatic constexpr std::size_t size = " << size << ";\n";
	out << "\tstatic constexpr " << uint_name << " mod_inv[size] = {\n\t\t";
	out << std::hex << std::showbase;
	const char* suffix = sizeof(UInt) == 8 ? "ull" : "ul";
	for (std::size_t i = 0; i < size; ++i) {
		out << mod_inv[i] << suffix;
		if (i != size - 1) {
			out << ", ";
		}
	}
	out << "\n\t}\n";
	out << "\tstatic constexpr " << uint_name << " max_quotients[size] = {\n\t\t";
	for (std::size_t i = 0; i < size; ++i) {
		out << max_quotients[i] << suffix;;
		if (i != size - 1) {
			out << ", ";
		}
	}
	out << "\n\t}\n";
}

int main()
{
	std::ofstream out{ "results/div_tables.txt" };
	//auto& out = std::cout;

	out << "struct div_5_uint32 {\n";
	print_inv_and_quotients<std::uint32_t>(out, "std::uint32_t", 5, 11);
	out << "}\n\n";

	out << "struct div_5_uint64 {\n";
	print_inv_and_quotients<std::uint64_t>(out, "std::uint64_t", 5, 24);
	out << "}\n\n";

	return 0;
}
