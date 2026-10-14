// Host checks only; not a build input. A 32-bit guest address space for the window sources:
// guest addresses are offsets from BASE into one host arena, so the 32-bit pointer words that
// the sources read from objects translate to host pointers through norrath_host_pointer.
#pragma once
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

namespace hw {
constexpr unsigned BASE = 0x10000;
alignas(16) inline unsigned char arena[0x40000];

inline unsigned char* at(unsigned guest) { return guest ? arena + (guest - BASE) : nullptr; }
inline unsigned guest(const void* host) {
    return host ? BASE + static_cast<unsigned>(static_cast<const unsigned char*>(host) - arena) : 0u;
}
inline unsigned& w(unsigned guest_address) { return *reinterpret_cast<unsigned*>(at(guest_address)); }
inline unsigned& w(const void* host, unsigned offset) {
    return *reinterpret_cast<unsigned*>(const_cast<unsigned char*>(static_cast<const unsigned char*>(host)) + offset);
}
inline void clear() { std::memset(arena, 0, sizeof arena); }

// Deterministic generator so every run sees the same scenarios.
struct Rng {
    unsigned long long state;
    explicit Rng(unsigned long long seed) : state(seed) {}
    unsigned next() {
        state = state * 6364136223846793005ull + 1442695040888963407ull;
        return static_cast<unsigned>(state >> 33);
    }
};

inline int failures = 0;
inline void check(bool condition, const char* what, long a = 0, long b = 0) {
    if (!condition) {
        ++failures;
        std::printf("FAIL %s %ld %ld\n", what, a, b);
    }
}
inline int finish() {
    std::puts(failures ? "FAIL" : "ok");
    return failures != 0;
}
}  // namespace hw

extern "C" inline unsigned char* norrath_host_pointer(unsigned address) { return hw::at(address); }
