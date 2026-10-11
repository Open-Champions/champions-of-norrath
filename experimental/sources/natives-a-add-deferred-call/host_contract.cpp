#include <cstring>
static int saw_address;
static int cell;
static char queued[160];
static int calls;
extern "C" int amx_GetAddr(void*, int address, int** physical) {
    saw_address = address;
    *physical = &cell;
    return 0;
}
extern "C" int amx_GetString(char* destination, int* source) {
    if (source != &cell) return 1;
    std::strcpy(destination, "later");
    return 0;
}
extern "C" void scriptAddDeferedCall__FPc(char* name) {
    ++calls;
    std::strcpy(queued, name);
}
extern "C" int amxAddDeferedCall__FP5__amxPi(void*, int*);
int main() {
    int params[2] = {4, 0x30};
    if (amxAddDeferedCall__FP5__amxPi(0, params) != 0) return 1;
    if (saw_address != 0x30 || calls != 1) return 2;
    if (std::strcmp(queued, "later") != 0) return 3;
    return 0;
}
