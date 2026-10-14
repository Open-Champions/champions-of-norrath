#include <cstdio>
#include <cstring>

extern "C" int amxShowBust__FP5__amxPi(void*, int*);

static const char* texts[] = {"bust", "caption"};
static int step;
static int* issued;
static int bad_source;
static int addresses[2];
static char looked[160];
static char shown_name[160];
static unsigned short message[] = {4, 0};
static unsigned short* shown_text;
static int x;
static int y;

extern "C" int amx_GetAddr(void*, int address, int** physical) {
    static int slot;
    addresses[step] = address;
    slot = step;
    issued = &slot;
    *physical = issued;
    return 0;
}

extern "C" int amx_GetString(char* dest, int* source) {
    if (source != issued) bad_source = 1;
    std::snprintf(dest, 160, "%s", texts[step]);
    step += 1;
    return 0;
}

extern "C" unsigned short* textGetString__FPc(char* key) {
    std::snprintf(looked, sizeof looked, "%s", key);
    return message;
}

extern "C" void hudShowBust__FPcPUsii(char* name, unsigned short* text, int column, int row) {
    std::snprintf(shown_name, sizeof shown_name, "%s", name);
    shown_text = text;
    x = column;
    y = row;
}

int main() {
    int params[5] = {16, 3, 9, 20, 30};
    if (amxShowBust__FP5__amxPi(nullptr, params) != 0) return 1;
    if (bad_source || addresses[0] != 3 || addresses[1] != 9) return 2;
    if (std::strcmp(looked, "caption") != 0 || std::strcmp(shown_name, "bust") != 0) return 3;
    if (shown_text != message || x != 20 || y != 30) return 4;
    std::printf("show bust passed\n");
    return 0;
}
