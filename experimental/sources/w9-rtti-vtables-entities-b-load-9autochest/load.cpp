// Observed msg_load__9AutoChestPc at 0x224b00, 188 bytes. Reads the stream in this order:
// a word stored at object+200 as a sign-extended 64-bit value (the image uses a doubleword
// store), a halfword stored at +220, a byte stored at +228, a byte stored at +229, and
// a word stored at +216; the same fields msg_save__9AutoChestPc writes. If the last
// word is nonzero, it then calls animAdd(object+152, the word at +244, 516) and
// animSetToEnd(object+152), and, unless the signed halfword at +220 is positive,
// clears bit 20 (0x100000) of the flag word at object+12. Returns 12, the bytes consumed.
// Unknown: what these fields mean, what animAdd's last argument 516 selects, and
// why the stream layout stores only the low word of the first field's 64-bit slot.

extern "C" {
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flags);
void animSetToEnd__FP14AnimationState(void* state);
}

typedef unsigned Word __attribute__((may_alias));
typedef short Half __attribute__((may_alias));
typedef long long Quad __attribute__((may_alias));

extern "C" int msg_load__9AutoChestPc(void* self, const char* in) {
    unsigned char* object = static_cast<unsigned char*>(self);
    int first = *reinterpret_cast<const int*>(in);
    *reinterpret_cast<Quad*>(object + 200) = first;
    *reinterpret_cast<Half*>(object + 220) = *reinterpret_cast<const Half*>(in + 4);
    object[228] = static_cast<unsigned char>(in[6]);
    object[229] = static_cast<unsigned char>(in[7]);
    unsigned last = *reinterpret_cast<const Word*>(in + 8);
    *reinterpret_cast<Word*>(object + 216) = last;
    if (last != 0) {
        animAdd__FP14AnimationStateP15AnimationHeaderi(object + 152, reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<Word*>(object + 244))), 516);
        animSetToEnd__FP14AnimationState(object + 152);
        if (*reinterpret_cast<Half*>(object + 220) <= 0) *reinterpret_cast<Word*>(object + 12) &= 0xffefffffu;
    }
    return 12;
}
