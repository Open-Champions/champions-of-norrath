// Observed msg_load__8CatapultPc at 0x0028c4d0, 120 bytes.
// Consumes one byte of the buffer. When that byte is nonzero it does what msg_trigger does:
// animInitState on the state at object+92, then animAdd with
// lumpFindResource("catapult.lmp", "catapult.anm") and the integer 1. A zero byte does
// nothing more. Returns 1, the number of bytes consumed. The object is not written directly.
// Unknown: the meaning of the byte (msg_save writes 1 when object+96 > 0, but load never
// stores it back).
extern "C" {
void animInitState__FP14AnimationState(void* state);
void* lumpFindResource__FPcT0(const char* lump, const char* name);
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flag);
}

extern "C" int msg_load__8CatapultPc(void* object_storage, char* buffer) {
    unsigned char* state = static_cast<unsigned char*>(object_storage) + 92;
    char flag = *buffer;
    if (flag != 0) {
        animInitState__FP14AnimationState(state);
        void* header = lumpFindResource__FPcT0("catapult.lmp", "catapult.anm");
        animAdd__FP14AnimationStateP15AnimationHeaderi(state, header, 1);
    }
    return 1;
}
