#include <cstdio>
#include <cstring>

extern "C" int amxStartMusic__FP5__amxPi(void*, int*);

static const char* source_text = "theme";
static char started[160];
static int mode;

extern "C" int amx_GetAddr(void*, int, int** physical) {
    *physical = reinterpret_cast<int*>(const_cast<char*>(source_text));
    return 0;
}

extern "C" int amx_GetString(char* dest, int*) {
    std::snprintf(dest, 160, "%s", source_text);
    return 0;
}

extern "C" void soundStartMusic__FPci(char* name, int value) {
    std::snprintf(started, sizeof started, "%s", name);
    mode = value;
}

int main() {
    int params[2] = {4, 8};
    if (amxStartMusic__FP5__amxPi(nullptr, params) != 0) return 1;
    if (std::strcmp(started, "theme") != 0 || mode != 3) return 2;
    std::printf("start music passed\n");
    return 0;
}
