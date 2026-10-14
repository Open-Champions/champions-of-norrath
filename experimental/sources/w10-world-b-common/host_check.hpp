// Host-only helper (not a build-plan input): independent little-endian accessors and a
// whole-buffer comparison, so a test fails on any stray store.
#pragma once
#include <cstdio>
#include <cstring>
inline unsigned hw(const unsigned char* p) { return p[0] | (p[1] << 8) | (p[2] << 16) | (unsigned(p[3]) << 24); }
inline void sw_(unsigned char* p, unsigned v) { p[0] = v; p[1] = v >> 8; p[2] = v >> 16; p[3] = v >> 24; }
inline void sh_(unsigned char* p, unsigned v) { p[0] = v; p[1] = v >> 8; }
inline void sf_(unsigned char* p, float f) { unsigned v; std::memcpy(&v, &f, 4); sw_(p, v); }
inline float lf_(const unsigned char* p) { unsigned v = hw(p); float f; std::memcpy(&f, &v, 4); return f; }
inline int same(const char* what, const unsigned char* got, const unsigned char* want, unsigned n) {
    for (unsigned i = 0; i != n; ++i)
        if (got[i] != want[i]) {
            std::printf("%s: byte %u got %02x want %02x\n", what, i, got[i], want[i]);
            return 1;
        }
    return 0;
}
