#include <cstdio>
#include <cstring>

extern "C" int amxStartAudio__FP5__amxPi(void*, int*);

static const char* source_text = "line";
static char started[160];
static int channel;
static int mode;

extern "C" int amx_GetAddr(void*, int, int** physical) {
    *physical = reinterpret_cast<int*>(const_cast<char*>(source_text));
    return 0;
}

extern "C" int amx_GetString(char* dest, int*) {
    std::snprintf(dest, 160, "%s", source_text);
    return 0;
}

extern "C" void soundStartDialogStream__FiPci(int slot, char* name, int value) {
    channel = slot;
    mode = value;
    std::snprintf(started, sizeof started, "%s", name);
}

int main() {
    int params[2] = {4, 15};
    if (amxStartAudio__FP5__amxPi(nullptr, params) != 0) return 1;
    if (channel != 2 || mode != 2 || std::strcmp(started, "line") != 0) return 2;
    std::printf("start audio passed\n");
    return 0;
}
