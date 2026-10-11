#include <cstdio>
#include <cstring>

extern "C" int amxRemoveQuest__FP5__amxPi(void*, int*);

static int seen_address;
static const char* source_text = "horn";
static char removed[160];
static int calls;

extern "C" int amx_GetAddr(void*, int address, int** physical) {
    seen_address = address;
    *physical = reinterpret_cast<int*>(const_cast<char*>(source_text));
    return 0;
}

extern "C" int amx_GetString(char* dest, int* source) {
    if (source != reinterpret_cast<int*>(const_cast<char*>(source_text))) return 9;
    std::snprintf(dest, 160, "%s", source_text);
    return 0;
}

extern "C" void removeQuestFromLog__FPc(char* name) {
    std::snprintf(removed, sizeof removed, "%s", name);
    calls += 1;
}

int main() {
    int params[2] = {4, 42};
    if (amxRemoveQuest__FP5__amxPi(nullptr, params) != 0) return 1;
    if (seen_address != 42 || calls != 1) return 2;
    if (std::strcmp(removed, "horn") != 0) return 3;
    std::printf("remove quest passed\n");
    return 0;
}
