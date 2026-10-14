#include <cstring>
static int which;
static int addresses[2];
static char title[160];
static char body[160];
extern "C" int amx_GetAddr(void*, int address, int** physical) {
    addresses[which] = address;
    *physical = &which;
    return 0;
}
extern "C" int amx_GetString(char* destination, int*) {
    std::strcpy(destination, which == 0 ? "title" : "body");
    ++which;
    return 0;
}
extern "C" void addQuestToLog__FPcT0(char* got_title, char* got_body) {
    std::strcpy(title, got_title);
    std::strcpy(body, got_body);
}
extern "C" int amxAddQuest__FP5__amxPi(void*, int*);
int main() {
    int params[3] = {8, 11, 22};
    if (amxAddQuest__FP5__amxPi(0, params) != 0) return 1;
    if (addresses[0] != 11 || addresses[1] != 22) return 2;
    if (std::strcmp(title, "title") != 0 || std::strcmp(body, "body") != 0) return 3;
    return 0;
}
