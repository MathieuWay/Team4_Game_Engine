#pragma once
#include <cstdint>
#include <array>
#include <algorithm>

namespace team4_game_engine::serialization {
	enum class PlateformEndianess {
		BigEndiean,
		LittleEndian
	};

	constexpr PlateformEndianess DetectEndianess() {
		union {
			uint32_t i;
			char c[4];
		} testEndian = {0x01020304};
		
		return (testEndian.c[0] == 0x01) ? PlateformEndianess::BigEndiean : PlateformEndianess::LittleEndian;
	}

	template<typename T>
	T SwapEndian(const T& val) {
		union {
			T val;
			std::array<std::uint8_t, sizeof(T)> raw;
		}src, dst;

		src.val = val;
		std::reverse_copy(src.raw.begin(), src.raw.end(), dst.raw.begin());
		return dst.val;
	}
}