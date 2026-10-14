#pragma once
// Host-only helpers for the lava-monster-through-wraith constructor checks.
// Stubs record each call as text; the test lists the calls it expects, then
// compares the whole object image against an expected image built from the
// stores read off the original instruction sequence.
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
namespace hs {
inline int fails = 0;
inline std::vector<std::string> calls;
inline void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
inline void event(const char* format, ...) {
    char buffer[256];
    va_list list;
    va_start(list, format);
    std::vsnprintf(buffer, sizeof buffer, format, list);
    va_end(list);
    calls.emplace_back(buffer);
}
inline unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
inline unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
inline void put(unsigned char* p, unsigned value, unsigned size) {
    for (unsigned i = 0; i < size; ++i) p[i] = static_cast<unsigned char>(value >> (8 * i));
}
inline unsigned fbits(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
inline void expect_log(const std::vector<std::string>& want, const char* what) {
    bool same = want.size() == calls.size();
    for (std::size_t i = 0; same && i < want.size(); ++i) same = want[i] == calls[i];
    if (!same) {
        std::printf("FAIL %s call sequence\n  want:", what);
        for (const auto& s : want) std::printf(" [%s]", s.c_str());
        std::printf("\n  got: ");
        for (const auto& s : calls) std::printf(" [%s]", s.c_str());
        std::printf("\n");
        ++fails;
    }
}
// Object image with a poison fill, so any unexpected store is caught.
template <unsigned N> struct Image {
    alignas(16) unsigned char b[N];
    Image() { std::memset(b, 0x5a, N); }
    void w(unsigned off, unsigned value, unsigned size = 4) { put(b + off, value, size); }
};
template <unsigned N> void expect_image(const Image<N>& got, const Image<N>& want, const char* what) {
    for (unsigned i = 0; i < N; ++i) {
        if (got.b[i] != want.b[i]) {
            std::printf("FAIL %s byte %u: got %02x want %02x\n", what, i, got.b[i], want.b[i]);
            ++fails;
            return;
        }
    }
}
inline int finish(const char* name) {
    if (fails) return 1;
    std::printf("%s passed\n", name);
    return 0;
}
}
