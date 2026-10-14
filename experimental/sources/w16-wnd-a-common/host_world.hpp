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

// Function words: a guest word that stands for a host function (class-table entries).
constexpr unsigned FUNCTION_BASE = 0x7000000;
inline void* functions[64];
inline unsigned function_count = 0;
inline unsigned function_word(void* host_function) {
    for (unsigned i = 0; i < function_count; ++i)
        if (functions[i] == host_function) return FUNCTION_BASE + i * 4;
    functions[function_count] = host_function;
    return FUNCTION_BASE + function_count++ * 4;
}
// Writes a class-table entry (halfword delta, halfword index, function word) at a guest address.
inline void put_entry(unsigned guest_address, short delta, short index, void* host_function) {
    std::memcpy(at(guest_address), &delta, 2);
    std::memcpy(at(guest_address) + 2, &index, 2);
    w(guest_address + 4) = function_word(host_function);
}

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
extern "C" inline void* norrath_host_function(unsigned address) {
    return hw::functions[(address - hw::FUNCTION_BASE) / 4];
}
