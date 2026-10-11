#include <cstdio>
#include <cstring>

extern "C" int amxStartNPCAnim__FP5__amxPi(void*, int*);

static const char* texts[] = {"guard", "wave", "body"};
static int step;
static unsigned char flagged[320];
static unsigned char plain[320];
static int inits;
static int adds;
static int transitions;
static float rate;
static int mode;
static char group[32];
static char name[32];
static int asked;

extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int slot;
    *physical = &slot;
    return 0;
}

extern "C" int amx_GetString(char* dest, int*) {
    std::snprintf(dest, 160, "%s", texts[step % 3]);
    step += 1;
    return 0;
}

extern "C" void objectFindObject__FPcPP10GameObjecti(char* object_name, void** objects, int count) {
    asked = std::strcmp(object_name, "guard") == 0 ? count : -1;
    objects[0] = asked == 512 ? flagged : nullptr;
    objects[1] = asked == 512 ? plain : nullptr;
    objects[2] = nullptr;
}

extern "C" void animInitState__FP14AnimationState(unsigned char* state) {
    if (state == flagged + 288) inits += 1;
}

extern "C" void* lumpFindResource__FPcT0(char* first, char* second) {
    std::snprintf(group, sizeof group, "%s", first);
    std::snprintf(name, sizeof name, "%s", second);
    return flagged;
}

extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(unsigned char* state, void* header, int count) {
    if (state == flagged + 288 && header == flagged && count == 6) adds += 1;
}

extern "C" void animSetToEnd__FP14AnimationState(unsigned char*) {}
extern "C" void animResetFramestates__FP14AnimationState(unsigned char*) {}

extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(
    unsigned char* state, void*, float value, int which) {
    if (state == flagged + 288) {
        rate = value;
        mode = which;
        transitions += 1;
    }
}

int main() {
    int flags = 0x20;
    int none = 0;
    std::memcpy(flagged + 12, &flags, 4);
    std::memcpy(plain + 12, &none, 4);
    int params[5] = {16, 1, 2, 3, -1};
    if (amxStartNPCAnim__FP5__amxPi(nullptr, params) != 0) return 1;
    if (asked != 512 || inits != 1 || adds != 1 || transitions != 0) return 2;
    if (std::strcmp(group, "wave") != 0 || std::strcmp(name, "body") != 0) return 3;
    step = 0;
    params[4] = 5;
    if (amxStartNPCAnim__FP5__amxPi(nullptr, params) != 0) return 4;
    unsigned bits = 0;
    std::memcpy(&bits, &rate, 4);
    if (transitions != 1 || bits != 0x3e999999u || mode != 5 || inits != 1) return 5;
    std::printf("start npc anim passed\n");
    return 0;
}
