#include <cstring>
static int cell;
static int current = 5;
static int stored = -1;
static char got[160];
static char set_name[160];
extern "C" int amx_GetAddr(void*, int, int** physical) {
    *physical = &cell;
    return 0;
}
extern "C" int amx_GetString(char* destination, int* source) {
    if (source != &cell) return 1;
    std::strcpy(destination, "score");
    return 0;
}
extern "C" int scriptGetGameState__FPc(char* name) {
    std::strcpy(got, name);
    return current;
}
extern "C" void scriptSetGameState__FPci(char* name, int value) {
    std::strcpy(set_name, name);
    stored = value;
}
extern "C" int amxAddv__FP5__amxPi(void*, int*);
int main() {
    int params[3] = {8, 1, 7};
    if (amxAddv__FP5__amxPi(0, params) != 12 || stored != 12) return 1;
    if (std::strcmp(got, "score") != 0 || std::strcmp(set_name, "score") != 0) return 2;
    current = 1;
    params[2] = -1;
    if (amxAddv__FP5__amxPi(0, params) != 0 || stored != 0) return 3;
    return 0;
}
