// Observed msg_load__5ChestPc at 0x0021dab0, 220 bytes.
// Reads 12 bytes from the buffer in the order msg_save__5ChestPc writes them: the word to
// object+84, the halfword to +76, the bytes to +88 and +89 (read as signed chars), and a word w to
// +132. When w is nonzero: animAdd__FP14AnimationStateP15AnimationHeaderi is called with the
// AnimationState at object+92, the word at (the pointer at object+36)+28 and the int 516, then
// animSetToEnd__FP14AnimationState on the same state; after that, if the signed halfword at +76 is
// not positive, bit 0x100000 of the word at +12 is cleared. In every case the last step calls
// setResource__10GameObjectPQ210GameObject11resourceMap with the object and the entry of
// chestResourceList selected by the signed byte at +89, and the byte count read (12) is returned.
// Inferred: +92 is an embedded AnimationState (it is only passed on) and w means "opened". Unknown:
// the field names, the meaning of 516, and the size of chestResourceList (no bounds check exists).
// Nothing here ran on the EE.
extern "C" {
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flags);
void animSetToEnd__FP14AnimationState(void* state);
void setResource__10GameObjectPQ210GameObject11resourceMap(void* self, void* map);
extern void* chestResourceList[];
}

#if defined(W10_ACTOR_D_HOST)
extern unsigned char* w10_actor_d_host_base;  // host builds keep 32-bit words as offsets from this base
#endif
namespace {
void* from_word(unsigned word) {
#if defined(W10_ACTOR_D_HOST)
    return w10_actor_d_host_base + word;
#else
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
}  // namespace

extern "C" int msg_load__5ChestPc(void* self, char* buffer) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const char* cursor = buffer;
    void* animationState = object + 92;
    *reinterpret_cast<unsigned*>(object + 84) = *reinterpret_cast<const unsigned*>(cursor);
    cursor += 4;
    *reinterpret_cast<short*>(object + 76) = *reinterpret_cast<const short*>(cursor);
    cursor += 2;
    object[88] = static_cast<unsigned char>(*cursor);
    cursor += 1;
    object[89] = static_cast<unsigned char>(*cursor);
    cursor += 1;
    const unsigned w = *reinterpret_cast<const unsigned*>(cursor);
    cursor += 4;
    *reinterpret_cast<unsigned*>(object + 132) = w;
    if (w != 0) {
        unsigned char* block = static_cast<unsigned char*>(from_word(*reinterpret_cast<unsigned*>(object + 36)));
        animAdd__FP14AnimationStateP15AnimationHeaderi(animationState, from_word(*reinterpret_cast<unsigned*>(block + 28)), 516);
        animSetToEnd__FP14AnimationState(animationState);
        if (*reinterpret_cast<short*>(object + 76) <= 0) *reinterpret_cast<unsigned*>(object + 12) &= 0xffefffffu;
    }
    setResource__10GameObjectPQ210GameObject11resourceMap(self, chestResourceList[static_cast<signed char>(object[89])]);
    return static_cast<int>(cursor - buffer);
}
