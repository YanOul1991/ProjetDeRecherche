
#pragma once

#include "Core/OptimEngine.h"
#include <memory>

struct SGuid {
	uint8 data[16];

	inline void print() const {
		for (int i = 0; i < 16; i++) {
			if (i == 4 || i == 6 || i == 8 || i == 10) {
				printf("-");
			}
			printf("%02X", (int)data[i]);
		}
	}
};

inline bool operator==(const SGuid& left, const SGuid& right) {
	return memcmp(left.data, right.data, 16) == 0;
}

namespace std
{

template<> 
struct hash<SGuid> {
	size_t operator()(const SGuid& guid) const noexcept {
		const uint64* p = reinterpret_cast<const uint64*>(guid.data);
		uint64 h = p[0] ^ (p[1] * 0x9E3779B97F4A7C15ull);

		h ^= (h >> 33); 
		h *= 0xff51afd7ed558ccdULL; 
		h ^= (h >> 33); 
		h *= 0xc4ceb9fe1a85ec53ULL; 
		h ^= (h >> 33);
		
		return static_cast<size_t>(h);
	}
};
} // namespace std

namespace Optim::Random {

SGuid CORE_API getGetGuid();

} // namespace Optim::Random