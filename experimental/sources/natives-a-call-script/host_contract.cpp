#include <cstring>
static void* saw_amx;
static int saw_address;
static int cell = 0x55;
static char started[160];
static int start_result = 12;
extern "C" int amx_GetAddr(void* amx, int address, int** physical) {
    saw_amx = amx;
    saw_address = address;
    *physical = &cell;
    return 0;
}
extern "C" int amx_GetString(char* destination, int* source) {
    if (source != &cell || cell != 0x55) return 1;
    std::strcpy(destination, "intro");
    return 0;
}
extern "C" int scriptStart__FPc(char* name) {
    std::strcpy(started, name);
    return start_result;
}
extern "C" int amxCallScript__FP5__amxPi(void*, int*);
int main() {
    int token = 4;
    int params[2] = {4, 0x20};
    if (amxCallScript__FP5__amxPi(&token, params) != 12) return 1;
    if (saw_amx != &token || saw_address != 0x20) return 2;
    if (std::strcmp(started, "intro") != 0) return 3;
    start_result = -6;
    if (amxCallScript__FP5__amxPi(&token, params) != -6) return 4;
    return 0;
}
