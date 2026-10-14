#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" int amxSound__FP5__amxPi(void*, int*);

static const char* texts[] = {"bank", "hit"};
static int step;
static char group[160];
static char name[160];
static int header_mode;
static void* played;
static const float* point;
static float volume;
static float pitch;
static float unused;
static int plays;

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

extern "C" void* lumpFindResource__FPcT0(char* first, char* second) {
    std::snprintf(group, sizeof group, "%s", first);
    std::snprintf(name, sizeof name, "%s", second);
    return header_mode ? reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x40)) : nullptr;
}

extern "C" void soundPlaySound__FP10_vagHeaderP6Point3fff(void* header, const float* where, float a, float b, float c) {
    played = header;
    point = where;
    volume = a;
    pitch = b;
    unused = c;
    plays += 1;
}

int main() {
    int params[3] = {8, 1, 2};
    header_mode = 0;
    if (amxSound__FP5__amxPi(nullptr, params) != 0) return 1;
    if (plays != 0 || std::strcmp(group, "bank") != 0 || std::strcmp(name, "hit") != 0) return 2;
    step = 0;
    header_mode = 1;
    if (amxSound__FP5__amxPi(nullptr, params) != 0) return 3;
    if (plays != 1 || played == nullptr || point != nullptr) return 4;
    if (volume != 1.f || pitch != 1.f || unused != 0.f) return 5;
    std::printf("sound passed\n");
    return 0;
}
