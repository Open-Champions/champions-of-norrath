#include "draw_start.cpp"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

extern "C" unsigned char drawRecords[0x10000] = {};
extern "C" unsigned char drawHead[0x40] = {};
extern "C" int writeBuffer = 0;
extern "C" int nmDrawUsed = 0;

static std::string log_text;
static int reload_cause;

extern "C" int DIntr() {
    log_text += "D";
    if (nmDrawUsed != 8) log_text += "!";
    return 0;
}
extern "C" int EIntr() {
    log_text += "E";
    return 0;
}
extern "C" void reloadDMA__Fi(int cause) {
    log_text += "R";
    reload_cause = cause;
}

static unsigned word_at(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) |
           (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) |
           (static_cast<unsigned>(address[3]) << 24);
}

static int check_index(int initial) {
    std::memset(drawRecords, 0x5a, sizeof drawRecords);
    std::memset(drawHead, 0x5a, sizeof drawHead);
    writeBuffer = initial;
    nmDrawUsed = 99;
    log_text.clear();
    drawStart__Fv();
    const int index = initial ^ 1;
    if (writeBuffer != index || nmDrawUsed != 8) return 1;
    if (log_text != "DRE" || reload_cause != -1) return 2;
    for (int slot = 0; slot < 8; ++slot) {
        unsigned char* record = drawRecords + ((static_cast<unsigned>(index) * 2048u + static_cast<unsigned>(slot)) << 4);
        const unsigned expect = static_cast<unsigned>(reinterpret_cast<unsigned long>(record));
        if (word_at(drawHead + index * 32 + slot * 4) != expect) return 3;
        if (word_at(record + 12) != 0u) return 4;
        if (record[11] != 0x5a || record[16] != 0x5a) return 5;
    }
    const int other = index ^ 1;
    if (drawHead[other * 32] != 0x5a) return 6;
    return 0;
}

int main() {
    if (check_index(0) != 0) return 1;
    if (check_index(1) != 0) return 2;
    std::puts("draw start contract passed");
    return 0;
}
