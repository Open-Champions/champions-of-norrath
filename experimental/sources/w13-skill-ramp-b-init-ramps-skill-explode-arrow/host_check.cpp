// Host check. Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp init_ramps.cpp
// Fills a guarded buffer with two different patterns, calls the function, and compares every byte with the
// expected little-endian image of each store (offset, size, bytes) below; all other bytes must be unchanged.
#include <cstdio>
#include <cstring>

extern "C" void init_ramps(void*) asm("initRamps__17SkillExplodeArrow");

struct Store { unsigned offset; unsigned size; unsigned char bytes[4]; };
static const Store stores[] = {
    {84, 4, {0x00, 0x00, 0x80, 0x3f}},
    {24, 2, {0x7d, 0x00}},
    {36, 4, {0xed, 0xff, 0x9f, 0x40}},
    {26, 2, {0xa1, 0x01}},
    {40, 4, {0xd5, 0x8c, 0x4d, 0x41}},
    {18, 2, {0x05, 0x00}},
    {28, 2, {0x32, 0x00}},
    {44, 4, {0x08, 0xfd, 0x3f, 0x40}},
    {32, 2, {0x44, 0x00}},
    {52, 4, {0x75, 0xff, 0x3f, 0x40}},
    {66, 2, {0x02, 0x00}},
    {76, 4, {0x00, 0x00, 0x00, 0x40}},
    {68, 2, {0x90, 0x01}},
    {80, 4, {0x00, 0x00, 0xa0, 0x40}},
    {60, 2, {0x01, 0x00}},
    {70, 2, {0x2d, 0x00}},
    {14, 2, {0x01, 0x00}},
    {16, 2, {0x01, 0x00}},
    {22, 2, {0x01, 0x00}},
    {56, 2, {0x01, 0x00}},
    {58, 2, {0x01, 0x00}},
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
