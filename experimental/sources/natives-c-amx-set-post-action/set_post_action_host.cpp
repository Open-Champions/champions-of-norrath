#include <cstdio>
#include <cstring>

extern "C" int amxSetPostAction__FP5__amxPi(void*, int*);

static int seen_address;
static const char* source_text = "after";
static char stored[160];

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

extern "C" void scriptSetPostAction__FPc(char* text) {
    std::snprintf(stored, sizeof stored, "%s", text);
}

int main() {
    int params[2] = {4, 11};
    if (amxSetPostAction__FP5__amxPi(nullptr, params) != 0) return 1;
    if (seen_address != 11 || std::strcmp(stored, "after") != 0) return 2;
    std::printf("set post action passed\n");
    return 0;
}
