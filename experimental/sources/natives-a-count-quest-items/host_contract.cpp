#include <cstring>
static int cell;
static int owner = -1;
static char item[160];
static int result = 3;
extern "C" int amx_GetAddr(void*, int, int** physical) {
    *physical = &cell;
    return 0;
}
extern "C" int amx_GetString(char* destination, int* source) {
    if (source != &cell) return 1;
    std::strcpy(destination, "relic");
    return 0;
}
extern "C" int queryInventoryItem__FPci(char* name, int got_owner) {
    std::strcpy(item, name);
    owner = got_owner;
    return result;
}
extern "C" int amxCountQuestItems__FP5__amxPi(void*, int*);
int main() {
    int params[2] = {4, 1};
    if (amxCountQuestItems__FP5__amxPi(0, params) != 3) return 1;
    if (owner != 0 || std::strcmp(item, "relic") != 0) return 2;
    result = 0;
    if (amxCountQuestItems__FP5__amxPi(0, params) != 0) return 3;
    return 0;
}
