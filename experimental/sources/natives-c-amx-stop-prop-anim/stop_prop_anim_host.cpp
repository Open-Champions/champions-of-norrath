#include <cstdio>
#include <cstring>

extern "C" int amxStopPropAnim__FP5__amxPi(void*, int*);

static const char* source_text = "door";
static char looked[160];
static unsigned char object[96];

extern "C" int amx_GetAddr(void*, int, int** physical) {
    *physical = reinterpret_cast<int*>(const_cast<char*>(source_text));
    return 0;
}

extern "C" int amx_GetString(char* dest, int*) {
    std::snprintf(dest, 160, "%s", source_text);
    return 0;
}

extern "C" void* objectFindObject__FPc(char* name) {
    std::snprintf(looked, sizeof looked, "%s", name);
    return object;
}

int main() {
    std::memset(object, 0x7e, sizeof object);
    int params[2] = {4, 5};
    if (amxStopPropAnim__FP5__amxPi(nullptr, params) != 0) return 1;
    if (std::strcmp(looked, "door") != 0) return 2;
    if (object[88] != 0) return 3;
    if (object[87] != 0x7e || object[89] != 0x7e) return 4;
    std::printf("stop prop anim passed\n");
    return 0;
}
