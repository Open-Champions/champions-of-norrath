// Observed msg_load__3NPCPc at 0x00231978, 320 bytes. Reads six 32-bit words from the buffer
// and returns 24 (the bytes consumed). In order, w0..w2 are string-pool indexes whose texts
// are copied with strcpy to object+960, +992 and +1024; w3 goes to the halfword at +1066; w4 to
// the word at +244; w5 (a float) to +176. Then: the model resource lumpFindResource(+960, +992)
// goes to +1056 and lumpFindResource(+960, +1024) to +1060; the animation state at +288 is
// initialised; if objectFindAnimation(self, "idle") finds a header it is added with flag 1 and
// its time randomised; modelGetActiveChunks(+1056, +244) is stored as a 64-bit value at +552.
// Last, the byte playedStinger (0x004857e0) is cleared. The Creature load routine is not
// called. Nothing here ran on the EE.
extern "C" {
extern char playedStinger;
char* objectSaveGetFromStringPool__Fi(int index);
char* strcpy(char* to, const char* from);
void* lumpFindResource__FPcT0(char* lump, char* name);
void animInitState__FP14AnimationState(void* state);
void* objectFindAnimation__FP10GameObjectPc(void* object, char* name);
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flag);
void animRandomizeTime__FP14AnimationState(void* state);
long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int mask);

int msg_load__3NPCPc(void* self, char* buffer) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    const int* in = reinterpret_cast<const int*>(buffer);
    char* const lump = reinterpret_cast<char*>(o + 960);
    char* const model = reinterpret_cast<char*>(o + 992);
    char* const texture = reinterpret_cast<char*>(o + 1024);
    void* const animation = o + 288;
    strcpy(lump, objectSaveGetFromStringPool__Fi(*in++));
    strcpy(model, objectSaveGetFromStringPool__Fi(*in++));
    strcpy(texture, objectSaveGetFromStringPool__Fi(*in++));
    const int w3 = *in++;
    const int w4 = *in++;
    const float w5 = *reinterpret_cast<const float*>(in++);
    *reinterpret_cast<short*>(o + 1066) = static_cast<short>(w3);
    *reinterpret_cast<float*>(o + 176) = w5;
    *reinterpret_cast<int*>(o + 244) = w4;
    // Pointers are 32-bit on the target; word accesses keep the host contract layout-exact.
    *reinterpret_cast<unsigned*>(o + 1056) = static_cast<unsigned>(reinterpret_cast<unsigned long>(lumpFindResource__FPcT0(lump, model)));
    *reinterpret_cast<unsigned*>(o + 1060) = static_cast<unsigned>(reinterpret_cast<unsigned long>(lumpFindResource__FPcT0(lump, texture)));
    animInitState__FP14AnimationState(animation);
    void* const idle = objectFindAnimation__FP10GameObjectPc(self, const_cast<char*>("idle"));
    if (idle != nullptr) {
        animAdd__FP14AnimationStateP15AnimationHeaderi(animation, idle, 1);
        animRandomizeTime__FP14AnimationState(animation);
    }
    *reinterpret_cast<long long*>(o + 552) =
        modelGetActiveChunks__FP12_modelHeaderi(reinterpret_cast<void*>(static_cast<unsigned long>(*reinterpret_cast<unsigned*>(o + 1056))),
                                                *reinterpret_cast<int*>(o + 244));
    playedStinger = 0;
    return static_cast<int>(reinterpret_cast<const char*>(in) - buffer);
}
}
