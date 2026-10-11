#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" int amxSoundSequence__FP5__amxPi(void*, int*);

static const char* texts[] = {"seq", "open"};
static int step;
static void* played;
static void* object;
static float rate;
static int marked;

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

extern "C" void* lumpFindResource__FPcT0(char* group, char* name) {
    if (std::strcmp(group, "seq") != 0 || std::strcmp(name, "open") != 0) return nullptr;
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x80));
}

extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* owner, float value) {
    played = header;
    object = owner;
    rate = value;
    return 14;
}

extern "C" void soundMakeSequenceUnpausable__Fi(int sequence) { marked = sequence; }

int main() {
    int params[3] = {8, 1, 2};
    if (amxSoundSequence__FP5__amxPi(nullptr, params) != 0) return 1;
    if (played == nullptr || object != nullptr || rate != 1.f || marked != 14) return 2;
    std::printf("sound sequence passed\n");
    return 0;
}
