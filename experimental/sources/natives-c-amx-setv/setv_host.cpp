#include <cstdio>
#include <cstring>

extern "C" int amxSetv__FP5__amxPi(void*, int*);

static const char* source_text = "gold";
static int seen_address;
static char stored_name[160];
static int stored_value;

extern "C" int amx_GetAddr(void*, int address, int** physical) {
    seen_address = address;
    *physical = reinterpret_cast<int*>(const_cast<char*>(source_text));
    return 0;
}

extern "C" int amx_GetString(char* dest, int*) {
    std::snprintf(dest, 160, "%s", source_text);
    return 0;
}

extern "C" void scriptSetGameState__FPci(char* name, int value) {
    std::snprintf(stored_name, sizeof stored_name, "%s", name);
    stored_value = value;
}

int main() {
    int params[3] = {8, 21, 44};
    if (amxSetv__FP5__amxPi(nullptr, params) != 0) return 1;
    if (seen_address != 21) return 2;
    if (std::strcmp(stored_name, "gold") != 0 || stored_value != 44) return 3;
    std::printf("setv passed\n");
    return 0;
}
