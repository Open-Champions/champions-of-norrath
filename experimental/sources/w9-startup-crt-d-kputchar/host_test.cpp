// Host comparison only. Not an EE build input. The status register is scripted:
// busy for a chosen number of reads, then clear; the data register records writes.
#include <cstdio>

static unsigned status_word;
static unsigned reads_until_clear;
static unsigned char data_byte;
static int writes;
static unsigned reads;
struct Status {
    operator unsigned() const {
        ++reads;
        if (reads_until_clear == 0) return 0x7fffu;      // bit 15 clear, other bits set
        --reads_until_clear;
        return 0x8000u | 0x1u;
    }
};
static Status status_reg;
struct Data {
    Data& operator=(unsigned char v) { data_byte = v; ++writes; return *this; }
};
static Data data_reg;
#define W9_KPUTCHAR_STATUS status_reg
#define W9_KPUTCHAR_DATA data_reg
#include "kputchar.cpp"

int main() {
    int failures = 0;
    reads_until_clear = 3; reads = 0; writes = 0;
    int r = kputchar(0x141);
    if (r != 0x141) { std::printf("must return the argument unchanged\n"); ++failures; }
    if (data_byte != 0x41 || writes != 1) { std::printf("low byte must be stored once\n"); ++failures; }
    if (reads != 4) { std::printf("must poll until clear: %u reads\n", reads); ++failures; }
    reads_until_clear = 0; reads = 0; writes = 0;
    kputchar(-1);
    if (data_byte != 0xff || writes != 1 || reads != 1) { std::printf("immediate path wrong\n"); ++failures; }
    if (failures) return 2;
    std::printf("kputchar cases passed\n");
    return 0;
}
