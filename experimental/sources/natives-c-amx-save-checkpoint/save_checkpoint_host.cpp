#include <cstdio>
#include <cstring>

extern "C" int amxSaveCheckPoint__FP5__amxPi(void*, int*);

static char printed[64];
static int first;
static int second;
static int third;
static int prints;

extern "C" int scePrintf(const char* text) {
    std::snprintf(printed, sizeof printed, "%s", text);
    prints += 1;
    return 0;
}

extern "C" void MC_SaveCheckPoint__Fiii(int a, int b, int c) {
    first = a;
    second = b;
    third = c;
}

int main() {
    int params[2] = {4, 99};
    if (amxSaveCheckPoint__FP5__amxPi(nullptr, params) != 0) return 1;
    if (prints != 1 || std::strcmp(printed, "amxSaveCheckPoint\n") != 0) return 2;
    if (first != 0 || second != 0 || third != 1) return 3;
    std::printf("save checkpoint passed\n");
    return 0;
}
