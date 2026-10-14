#include <cstdio>
#include <cstring>

extern "C" int amxStartPropAnim__FP5__amxPi(void*, int*);

static const char* texts[] = {"door", "open"};
static int step;
static unsigned char object[128];
static char tag_key[16];
static int tag_index;
static char resource_group[32];
static char resource_name[32];
static int inits;
static int adds;
static int ends;
static int resets;
static int transitions;
static float rate;
static int mode;
static void* header_token = object;

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
    return std::strcmp(name, "door") == 0 ? object : nullptr;
}

extern "C" void animInitState__FP14AnimationState(unsigned char* state) {
    if (state == object + 84) inits += 1;
}

extern "C" char* findTagString__10GameObjectPci(void* value, char* tag, int index) {
    if (value != object) return nullptr;
    std::snprintf(tag_key, sizeof tag_key, "%s", tag);
    tag_index = index;
    return const_cast<char*>("props");
}

extern "C" void* lumpFindResource__FPcT0(char* group, char* name) {
    std::snprintf(resource_group, sizeof resource_group, "%s", group);
    std::snprintf(resource_name, sizeof resource_name, "%s", name);
    return header_token;
}

extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(unsigned char* state, void* header, int count) {
    if (state == object + 84 && header == header_token && count == 6) adds += 1;
}

extern "C" void animSetToEnd__FP14AnimationState(unsigned char* state) {
    if (state == object + 84) ends += 1;
}

extern "C" void animResetFramestates__FP14AnimationState(unsigned char* state) {
    if (state == object + 84) resets += 1;
}

extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(
    unsigned char* state, void* header, float value, int which) {
    if (state == object + 84 && header == header_token) {
        rate = value;
        mode = which;
        transitions += 1;
    }
}

int main() {
    int params[4] = {12, 1, 2, -1};
    if (amxStartPropAnim__FP5__amxPi(nullptr, params) != 0) return 1;
    if (inits != 1 || adds != 1 || ends != 1 || resets != 1 || transitions != 0) return 2;
    if (std::strcmp(tag_key, "lump") != 0 || tag_index != 0) return 3;
    if (std::strcmp(resource_group, "props") != 0 || std::strcmp(resource_name, "open") != 0) return 4;
    step = 0;
    params[3] = 4;
    if (amxStartPropAnim__FP5__amxPi(nullptr, params) != 0) return 5;
    if (transitions != 1 || inits != 1) return 6;
    unsigned bits = 0;
    std::memcpy(&bits, &rate, 4);
    if (bits != 0x3e999999u || mode != 4) return 7;
    std::printf("start prop anim passed\n");
    return 0;
}
