#include <cstring>
static int string_index;
static unsigned short first_text[2] = {1, 0};
static unsigned short second_text[2] = {2, 0};
static int hud_id = 0;
static unsigned short* hud_first;
static unsigned short* hud_second;
static char keys[2][160];
extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int cell;
    *physical = &cell;
    return 0;
}
extern "C" int amx_GetString(char* destination, int*) {
    std::strcpy(destination, string_index == 0 ? "help-a" : "help-b");
    ++string_index;
    return 0;
}
extern "C" unsigned short* textGetString__FPc(char* key) {
    int slot = std::strcmp(key, "help-a") == 0 ? 0 : 1;
    std::strcpy(keys[slot], key);
    return slot == 0 ? first_text : second_text;
}
extern "C" void hudAddHelpMessage__FiPUsT1(int id, unsigned short* first, unsigned short* second) {
    hud_id = id;
    hud_first = first;
    hud_second = second;
}
extern "C" int amxAddHelpMessage__FP5__amxPi(void*, int*);
int main() {
    int params[3] = {8, 3, 4};
    if (amxAddHelpMessage__FP5__amxPi(0, params) != 0) return 1;
    if (std::strcmp(keys[0], "help-a") != 0 || std::strcmp(keys[1], "help-b") != 0) return 2;
    if (hud_id != -1 || hud_first != first_text || hud_second != second_text) return 3;
    return 0;
}
