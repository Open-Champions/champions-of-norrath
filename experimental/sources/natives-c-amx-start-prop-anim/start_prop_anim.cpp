// Observed amxStartPropAnim__FP5__amxPi at 0x00139230, 288 bytes. Cells 1
// and 2 are AMX strings and cell 3 is the mode. The object is found by the
// single-object lookup. Its animation state is byte 84. Mode -1 initializes
// that state, finds the "lump" tag, adds the resource with count 6, and
// sets the state to its end. Any other mode adds a transition at the .lit4
// word 0x3e999999. The native returns 0. amx_GetAddr's stack slot is zeroed
// first; the original slot was left uninitialized when that call failed.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* dest, int* source);
extern "C" void* objectFindObject__FPc(char* name);
extern "C" void animInitState__FP14AnimationState(unsigned char* state);
extern "C" char* findTagString__10GameObjectPci(void* object, char* tag, int index);
extern "C" void* lumpFindResource__FPcT0(char* group, char* name);
extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(unsigned char* state, void* header, int count);
extern "C" void animSetToEnd__FP14AnimationState(unsigned char* state);
extern "C" void animResetFramestates__FP14AnimationState(unsigned char* state);
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(
    unsigned char* state, void* header, float rate, int mode);

namespace {
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

extern "C" int amxStartPropAnim__FP5__amxPi(void* amx, int* params) {
    char name[160];
    char anim[160];
    int* physical = nullptr;
    amx_GetAddr(amx, params[1], &physical);
    amx_GetString(name, physical);
    auto* object = static_cast<unsigned char*>(objectFindObject__FPc(name));
    unsigned char* state = object + 84;
    amx_GetAddr(amx, params[2], &physical);
    amx_GetString(anim, physical);
    if (params[3] == -1) {
        animInitState__FP14AnimationState(state);
        char tag[] = "lump";
        void* header = lumpFindResource__FPcT0(findTagString__10GameObjectPci(object, tag, 0), anim);
        animAdd__FP14AnimationStateP15AnimationHeaderi(state, header, 6);
        animSetToEnd__FP14AnimationState(state);
        animResetFramestates__FP14AnimationState(state);
        return 0;
    }
    char tag[] = "lump";
    void* header = lumpFindResource__FPcT0(findTagString__10GameObjectPci(object, tag, 0), anim);
    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(state, header, bits_to_float(0x3e999999u), params[3]);
    return 0;
}
