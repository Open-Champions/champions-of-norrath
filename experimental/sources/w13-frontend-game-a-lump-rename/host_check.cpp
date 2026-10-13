// Host check for lumpRename__FPcT0. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin host_check.cpp
#include "lump_rename.cpp"

#include <cstdio>
#include <cstring>
#include <initializer_list>

extern "C" {
int nmLumps;
unsigned char lumps[40 * 8];
}

static void setName(int i, const char* s) {
    std::memset(lumps + 40 * i, 0x5a, 40);
    std::strcpy(reinterpret_cast<char*>(lumps + 40 * i), s);
}
static const char* nameOf(int i) { return reinterpret_cast<const char*>(lumps + 40 * i); }

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

int main() {
    char from[] = "town.lmp";
    char to[] = "x.lmp";
    setName(0, "town.lmp"); setName(1, "other.lmp"); setName(2, "town.lmp"); setName(3, "town.lmp"); setName(4, "town.lmp");
    nmLumps = 4;
    unsigned char before[40 * 8];
    std::memcpy(before, lumps, sizeof before);
    lumpRename__FPcT0(from, to);
    // every match among the first nmLumps records is renamed, not only the first
    CHECK(std::strcmp(nameOf(0), "x.lmp") == 0 && std::strcmp(nameOf(2), "x.lmp") == 0 && std::strcmp(nameOf(3), "x.lmp") == 0);
    CHECK(std::strcmp(nameOf(1), "other.lmp") == 0);
    // the record after the count is untouched, and so is everything after a renamed name's terminator
    CHECK(std::memcmp(lumps + 40 * 4, before + 40 * 4, 40 * 4) == 0);
    for (int i : {0, 2, 3})
        CHECK(std::memcmp(lumps + 40 * i + 6, before + 40 * i + 6, 40 - 6) == 0);
    CHECK(std::memcmp(lumps + 40, before + 40, 40) == 0);
    // a name that matches only by prefix is not renamed
    setName(0, "town.lmp2"); setName(1, "town.lm"); nmLumps = 2;
    lumpRename__FPcT0(from, to);
    CHECK(std::strcmp(nameOf(0), "town.lmp2") == 0 && std::strcmp(nameOf(1), "town.lm") == 0);
    // no records
    setName(0, "town.lmp"); nmLumps = 0;
    lumpRename__FPcT0(from, to);
    CHECK(std::strcmp(nameOf(0), "town.lmp") == 0);
    nmLumps = -3;
    lumpRename__FPcT0(from, to);
    CHECK(std::strcmp(nameOf(0), "town.lmp") == 0);
    // a replacement longer than the record runs into the next one (no bound is applied)
    setName(0, "a"); nmLumps = 1;
    char longer[] = "0123456789012345678901234567890123456789ab";
    char one[] = "a";
    lumpRename__FPcT0(one, longer);
    CHECK(std::strcmp(nameOf(0), longer) == 0 && lumps[40] == 'a' && lumps[41] == 'b' && lumps[42] == 0 && lumps[43] == 'n');
    std::puts("lump rename passed");
    return 0;
}
