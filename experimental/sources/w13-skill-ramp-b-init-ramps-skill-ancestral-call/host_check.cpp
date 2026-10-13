// Host check. Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp init_ramps.cpp
// Fills a guarded buffer with two different patterns, calls the function, and compares every byte with the
// expected little-endian image of each store (offset, size, bytes) below; all other bytes must be unchanged.
#include <cstdio>
#include <cstring>

extern "C" void init_ramps(void*) asm("initRamps__18SkillAncestralCall");

struct Store { unsigned offset; unsigned size; unsigned char bytes[4]; };
static const Store stores[] = {
    {24, 2, {0x7d, 0x00}},
    {52, 4, {0xa6, 0x07, 0x80, 0x40}},
    {36, 4, {0xdc, 0xff, 0x9f, 0x40}},
    {26, 2, {0x6e, 0x00}},
    {40, 4, {0x2d, 0x20, 0x00, 0x40}},
    {28, 2, {0x14, 0x00}},
    {44, 4, {0x56, 0x41, 0x80, 0x3f}},
    {30, 2, {0x04, 0x00}},
    {48, 4, {0xd1, 0xca, 0x01, 0x3f}},
    {22, 2, {0x01, 0x00}},
    {32, 2, {0x55, 0x00}},
    {14, 2, {0x01, 0x00}},
    {16, 2, {0x01, 0x00}},
    {18, 2, {0x01, 0x00}},
    {20, 2, {0x01, 0x00}},
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
