#include <cstdio>
#include <cstring>
extern "C" int amxPrint__FP5__amxPi(void*, int*);
static void* seen_amx;
static int seen_address = -1;
static void* seen_cells;
static int string_calls = 0;
static char packed[4];

extern "C" int amx_GetAddr(void* amx, int address, void** physical) {
    seen_amx = amx;
    seen_address = address;
    *physical = packed;
    return 7;
}
extern "C" int amx_GetString(char* destination, void* cells) {
    string_calls += 1;
    seen_cells = cells;
    std::strcpy(destination, "hi");
    return 3;
}

int main() {
    int marker = 0;
    int cells[] = {8, 55, 99};
    if (amxPrint__FP5__amxPi(&marker, cells) != 0) return 1;
    if (seen_amx != &marker || seen_address != 55) return 2;
    if (string_calls != 1 || seen_cells != packed) return 3;
    std::puts("amxPrint passed");
    return 0;
}
