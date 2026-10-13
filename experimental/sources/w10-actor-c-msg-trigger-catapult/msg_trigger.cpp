// Observed msg_trigger__8Catapulti at 0x0028c460, 76 bytes. The int argument is not read.
// Restarts the animation state embedded at object+92: animInitState on it, then animAdd with
// the resource lumpFindResource("catapult.lmp", "catapult.anm") and the integer 1.
// Returns nothing. Unknown: what the animation is for, and the meaning of the final 1.
extern "C" {
void animInitState__FP14AnimationState(void* state);
void* lumpFindResource__FPcT0(const char* lump, const char* name);
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flag);
}

extern "C" void msg_trigger__8Catapulti(void* object_storage, int) {
    unsigned char* state = static_cast<unsigned char*>(object_storage) + 92;
    animInitState__FP14AnimationState(state);
    void* header = lumpFindResource__FPcT0("catapult.lmp", "catapult.anm");
    animAdd__FP14AnimationStateP15AnimationHeaderi(state, header, 1);
}
