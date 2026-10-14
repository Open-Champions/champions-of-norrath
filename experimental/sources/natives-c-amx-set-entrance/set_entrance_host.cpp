#include <cstdio>
#include <cstring>

extern "C" int amxSetEntrance__FP5__amxPi(void*, int*);
extern "C" char startEntranceName[32] = {};

static int seen_address;
static const char* source_text = "gate";

extern "C" int amx_GetAddr(void*, int address, int** physical) {
    seen_address = address;
    *physical = reinterpret_cast<int*>(const_cast<char*>(source_text));
    return 0;
}

extern "C" int amx_GetString(char* dest, int* source) {
    if (source != reinterpret_cast<int*>(const_cast<char*>(source_text))) return 9;
    std::snprintf(dest, 160, "%s", source_text);
    return 0;
}

int main() {
    std::memset(startEntranceName, 0x5a, sizeof startEntranceName);
    int params[2] = {4, 70};
    if (amxSetEntrance__FP5__amxPi(nullptr, params) != 0) return 1;
    if (seen_address != 70) return 2;
    if (std::strcmp(startEntranceName, "gate") != 0) return 3;
    std::printf("set entrance passed\n");
    return 0;
}
