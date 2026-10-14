// Host check. Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp init_ramps.cpp
// Fills a guarded buffer with two different patterns, calls the function, and compares every byte with the
// expected little-endian image of each store (offset, size, bytes) below; all other bytes must be unchanged.
#include <cstdio>
#include <cstring>

extern "C" void init_ramps(void*) asm("initRamps__18SkillHammerOfWrath");

struct Store { unsigned offset; unsigned size; unsigned char bytes[4]; };
static const Store stores[] = {
    {44, 4, {0x00, 0x00, 0x20, 0x40}},
    {24, 2, {0x28, 0x00}},
    {26, 2, {0x0f, 0x00}},
    {40, 4, {0x00, 0x00, 0x00, 0x40}},
    {18, 2, {0x01, 0x00}},
    {28, 2, {0x05, 0x00}},
    {14, 2, {0x01, 0x00}},
    {36, 4, {0x00, 0x00, 0x00, 0x40}},
    {16, 2, {0x01, 0x00}},
};

int main() {
    static const unsigned char fills[2] = {0xa5, 0x5a};
    for (unsigned char fill : fills) {
        alignas(8) unsigned char buffer[160];
        unsigned char expected[160];
        std::memset(buffer, fill, sizeof buffer);
        std::memset(expected, fill, sizeof expected);
        for (const Store& s : stores) std::memcpy(expected + s.offset, s.bytes, s.size);
        init_ramps(buffer + 0);
        for (unsigned i = 0; i < sizeof buffer; ++i) {
            if (buffer[i] != expected[i]) {
                std::printf("byte %u: got %02x want %02x\n", i, buffer[i], expected[i]);
                return 1;
            }
        }
    }
    std::puts("ok");
    return 0;
}
