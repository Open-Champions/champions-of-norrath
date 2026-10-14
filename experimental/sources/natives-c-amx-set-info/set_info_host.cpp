#include <cstdio>
#include <cstring>

extern "C" int amxSetInfo__FP5__amxPi(void*, int*);

static const char* source_text = "hello";
static char looked[160];
static unsigned short message[] = {1, 0};
static unsigned short* shown;
static int duration;
static int color;
static int channel;

extern "C" int amx_GetAddr(void*, int, int** physical) {
    *physical = reinterpret_cast<int*>(const_cast<char*>(source_text));
    return 0;
}

extern "C" int amx_GetString(char* dest, int*) {
    std::snprintf(dest, 160, "%s", source_text);
    return 0;
}

extern "C" unsigned short* textGetString__FPc(char* key) {
    std::snprintf(looked, sizeof looked, "%s", key);
    return message;
}

extern "C" void hudSetInfoMessage__FPUsiic(unsigned short* text, int hold, int tint, int slot) {
    shown = text;
    duration = hold;
    color = tint;
    channel = slot;
}

int main() {
    int params[2] = {4, 3};
    if (amxSetInfo__FP5__amxPi(nullptr, params) != 0) return 1;
    if (std::strcmp(looked, "hello") != 0) return 2;
    if (shown != message || duration != 180 || color != -1 || channel != 0) return 3;
    std::printf("set info passed\n");
    return 0;
}
