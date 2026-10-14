// Observed msg_collision__5TorchP10GameObject at 0x0022fee8, 328 bytes. Returns without doing
// anything unless all of these hold: bit 0x40 of the flag word at other+12 is set, the signed byte at
// object+138 is neither 2 nor 3, and the signed remainder of frameNm by 12 is zero. Then it builds an
// item description on the stack and calls
//   gameAttackResolver(other, object, 0.0f, 0.0f, 10000000, 0, 0, 0x0100c040, &description, false).
// The description bytes the image writes, offsets from its start: bytes 32 and 33 are 0xff; the
// halfwords at 16..22 and 24..30 (step 2) are zero; four 8-byte slots at 36, 44, 52 and 60 get a
// first halfword of -1. The first slot whose first halfword is -1 or 21 then receives the halfwords
// {21, 1, 1, 0}; since all four were just set to -1, that is always the slot at 36, as in the
// image. All other bytes of the stack object are not written. A null `other` is not tested.
// Unknown: what the description fields, the constant 0x0100c040 and bit 0x40 mean.
// Nothing here ran on the EE.
extern "C" {
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(
    void* target, void* source, float a, float b, int c, int d, int e, int f, void* description, bool flag);
extern int frameNm;
}

typedef unsigned __attribute__((may_alias)) Word;
typedef short __attribute__((may_alias)) Half;

extern "C" void msg_collision__5TorchP10GameObject(void* self, void* other) {
    unsigned char* object = static_cast<unsigned char*>(self);
    if ((*reinterpret_cast<Word*>(static_cast<unsigned char*>(other) + 12) & 0x40u) == 0) return;
    const signed char mode = static_cast<signed char>(object[138]);
    if (mode == 2 || mode == 3) return;
    if (frameNm % 12 != 0) return;
    unsigned char description[72];
    for (int i = 0; i < 4; ++i) *reinterpret_cast<Half*>(description + 36 + 8 * i) = -1;
    description[32] = 0xff;
    description[33] = 0xff;
    for (int i = 0; i < 4; ++i) {
        *reinterpret_cast<Half*>(description + 16 + 2 * i) = 0;
        *reinterpret_cast<Half*>(description + 24 + 2 * i) = 0;
    }
    for (int i = 0; i < 4; ++i) {
        Half* slot = reinterpret_cast<Half*>(description + 36 + 8 * i);
        if (slot[0] == -1 || slot[0] == 21) {
            slot[0] = 21;
            slot[1] = 1;
            slot[2] = 1;
            slot[3] = 0;
            break;
        }
    }
    gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(other, self, 0.0f, 0.0f, 10000000, 0, 0, 0x0100c040, description, false);
}
