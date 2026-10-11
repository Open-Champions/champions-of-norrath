#include <cstdio>
#include <cstring>

extern "C" int amxShowCompass__FP5__amxPi(void*, int*);

static const char* texts[] = {"marker", "north"};
static int step;
static unsigned char object[32];
static char looked[160];
static float point[3];
static unsigned short message[] = {2, 0};
static unsigned short* shown;

extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int slot;
    *physical = &slot;
    return 0;
}

extern "C" int amx_GetString(char* dest, int*) {
    std::snprintf(dest, 160, "%s", texts[step]);
    step += 1;
    return 0;
}

extern "C" void* objectFindObject__FPc(char* name) {
    if (std::strcmp(name, "marker") != 0) return nullptr;
    return object;
}

extern "C" unsigned short* textGetString__FPc(char* key) {
    std::snprintf(looked, sizeof looked, "%s", key);
    return message;
}

extern "C" void hudShowCompass__FG6Point3PUs(const float* value, unsigned short* text) {
    std::memcpy(point, value, sizeof point);
    shown = text;
}

int main() {
    float source[3] = {1.5f, -2.f, 4.f};
    std::memcpy(object + 16, source, sizeof source);
    int params[3] = {8, 1, 2};
    if (amxShowCompass__FP5__amxPi(nullptr, params) != 0) return 1;
    if (std::strcmp(looked, "north") != 0 || shown != message) return 2;
    if (point[0] != 1.5f || point[1] != -2.f || point[2] != 4.f) return 3;
    std::printf("show compass passed\n");
    return 0;
}
