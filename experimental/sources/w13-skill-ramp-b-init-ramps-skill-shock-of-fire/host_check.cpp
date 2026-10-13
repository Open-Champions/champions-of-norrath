// Host check. Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp init_ramps.cpp
// Fills a guarded buffer with two different patterns, calls the function, and compares every byte with the
// expected little-endian image of each store (offset, size, bytes) below; all other bytes must be unchanged.
#include <cstdio>
#include <cstring>

extern "C" void init_ramps(void*) asm("initRamps__16SkillShockOfFire");

struct Store { unsigned offset; unsigned size; unsigned char bytes[4]; };
static const Store stores[] = {
    {88, 4, {0x00, 0x00, 0x80, 0x3f}},
    {24, 2, {0xa2, 0x00}},
    {36, 4, {0x82, 0xfe, 0xff, 0x40}},
    {26, 2, {0x2d, 0x00}},
    {40, 4, {0x1c, 0x20, 0x00, 0x40}},
    {44, 4, {0xad, 0x65, 0xa6, 0x3f}},
    {48, 4, {0xb2, 0x51, 0xbc, 0x40}},
    {32, 2, {0x51, 0x00}},
    {52, 4, {0x22, 0x17, 0xa0, 0x40}},
    {66, 2, {0x03, 0x00}},
    {76, 4, {0xc9, 0xff, 0x3f, 0x40}},
    {68, 2, {0x05, 0x00}},
    {60, 2, {0x0a, 0x00}},
    {70, 2, {0x02, 0x00}},
    {84, 4, {0xcc, 0xcc, 0xcc, 0x3d}},
    {62, 2, {0x01, 0x00}},
    {72, 2, {0xf3, 0x00}},
    {14, 2, {0x01, 0x00}},
    {16, 2, {0x05, 0x00}},
    {18, 2, {0x05, 0x00}},
    {28, 2, {0x02, 0x00}},
    {20, 2, {0x01, 0x00}},
    {30, 2, {0xf3, 0x00}},
    {22, 2, {0x01, 0x00}},
    {56, 2, {0x01, 0x00}},
    {58, 2, {0x05, 0x00}},
    {80, 4, {0x00, 0x00, 0x80, 0x3f}},
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
