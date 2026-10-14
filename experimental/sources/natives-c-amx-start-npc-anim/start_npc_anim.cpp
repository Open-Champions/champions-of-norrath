// Observed amxStartNPCAnim__FP5__amxPi at 0x001390c8, 360 bytes. Cells 1, 2,
// and 3 are AMX strings and cell 4 is the mode. Up to 512 objects are
// collected. A null first object returns 0. Each object whose word at byte
// 12 has bit 5 set uses the animation state at byte 288. Mode -1
// initializes it, adds lumpFindResource of cells 2 and 3 with count 6, and
// sets that state to its end. Any other mode adds a transition at the
// .lit4 word 0x3e999999. The native returns 0. amx_GetAddr's stack slot is
// zeroed first; the original slot was left uninitialized when that call failed.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* dest, int* source);
extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** objects, int count);
extern "C" void animInitState__FP14AnimationState(unsigned char* state);
extern "C" void* lumpFindResource__FPcT0(char* group, char* name);
extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(unsigned char* state, void* header, int count);
extern "C" void animSetToEnd__FP14AnimationState(unsigned char* state);
extern "C" void animResetFramestates__FP14AnimationState(unsigned char* state);
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(
    unsigned char* state, void* header, float rate, int mode);

namespace {
int load_int(const unsigned char* bytes) {
    int value;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    out[0] = bytes[0];
    out[1] = bytes[1];
    out[2] = bytes[2];
    out[3] = bytes[3];
    return value;
}

float bits_to_float(unsigned bits) {
    float value;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    out[0] = static_cast<unsigned char>(bits);
    out[1] = static_cast<unsigned char>(bits >> 8);
    out[2] = static_cast<unsigned char>(bits >> 16);
    out[3] = static_cast<unsigned char>(bits >> 24);
    return value;
}
}

extern "C" int amxStartNPCAnim__FP5__amxPi(void* amx, int* params) {
    char name[160];
    char anim[160];
    char third[160];
    void* objects[513];
    int* physical = nullptr;
    amx_GetAddr(amx, params[1], &physical);
    amx_GetString(name, physical);
    amx_GetAddr(amx, params[2], &physical);
    amx_GetString(anim, physical);
    amx_GetAddr(amx, params[3], &physical);
    amx_GetString(third, physical);
    objects[0] = nullptr;
    objectFindObject__FPcPP10GameObjecti(name, objects, 512);
    int index = 0;
    while (objects[index] != nullptr) {
        auto* object = static_cast<unsigned char*>(objects[index]);
        const int flags = load_int(object + 12);
        index += 1;
        if ((flags & 0x20) == 0) continue;
        unsigned char* state = object + 288;
        if (params[4] == -1) {
            animInitState__FP14AnimationState(state);
            void* header = lumpFindResource__FPcT0(anim, third);
            animAdd__FP14AnimationStateP15AnimationHeaderi(state, header, 6);
            animSetToEnd__FP14AnimationState(state);
            animResetFramestates__FP14AnimationState(state);
        } else {
            void* header = lumpFindResource__FPcT0(anim, third);
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(
                state, header, bits_to_float(0x3e999999u), params[4]);
        }
    }
    return 0;
}
