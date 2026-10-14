#include <cstdio>
#include <cstring>

extern "C" int amxSetHitPoints__FP5__amxPi(void*, int*);

static const char* source_text = "orc";
static unsigned char object[576];

extern "C" int amx_GetAddr(void*, int, int** physical) {
    *physical = reinterpret_cast<int*>(const_cast<char*>(source_text));
    return 0;
}

extern "C" int amx_GetString(char* dest, int*) {
    std::snprintf(dest, 160, "%s", source_text);
    return 0;
}

extern "C" void* objectFindObject__FPc(char* name) {
    if (std::strcmp(name, "orc") != 0) return nullptr;
    return object;
}

static float stored() {
    float value = 0.f;
    std::memcpy(&value, object + 560, 4);
    return value;
}

int main() {
    std::memset(object, 0, sizeof object);
    int flags = 0x20;
    std::memcpy(object + 12, &flags, 4);
    int params[3] = {8, 1, 25};
    if (amxSetHitPoints__FP5__amxPi(nullptr, params) != 0) return 1;
    if (stored() != 25.f) return 2;
    flags = 0;
    std::memcpy(object + 12, &flags, 4);
    params[2] = 9;
    if (amxSetHitPoints__FP5__amxPi(nullptr, params) != 0) return 3;
    if (stored() != 25.f) return 4;
    flags = 0x21;
    std::memcpy(object + 12, &flags, 4);
    params[2] = -2;
    if (amxSetHitPoints__FP5__amxPi(nullptr, params) != 0 || stored() != -2.f) return 5;
    std::printf("set hit points passed\n");
    return 0;
}
