// Observed runAnimation__Fv at 0x001d2810, 304 bytes.
// If dialogCreature (gp-10728) is null it returns. Otherwise the
// AnimationState at creature+288 is searched with
// animFindComponentWithFlags(state, 32); a negative index returns. The
// function then reads soundGetStreamAmountPlayed(2). When it differs from the
// stored previous value (a file-local word, zero in the image), the float at
// creature+320+index*32 is scaled by 0.016666668, 0.07 is subtracted, and the
// result is compared with (amount-1024)*7.2916700e-5. When the two differ by
// less than 0.02 the file-local correction float becomes zero; otherwise it
// becomes +0.1 when the scaled value is below the stream-derived one and -0.1
// when not. The previous-amount word is updated in both cases, and only when
// the amount changed. A nonzero correction is then added to the float at
// state+32+i*32 for each of the signed-byte count at state+4 entries.
// The correction persists between calls and is applied on every call while it
// is nonzero, even when the amount is unchanged.
// The six constants are the bit patterns in the image:
// 3c888888, 3898eadb, 3ca3d70a, 3d8f5c28, 3dcccccc and bdcccccc.
// Inferred: the stream is audio, the float at +320 a position in seconds, and
// the correction a lip-sync drift term. These are guesses from the arithmetic,
// not established. The two file-local values are zero-initialized words in
// the original .sdata (0x00485058 and 0x0048505c); here they are ordinary
// zero-initialized statics, so their section differs.
extern "C" int animFindComponentWithFlags__FP14AnimationStatei(unsigned char* state, int flags);
extern "C" int soundGetStreamAmountPlayed__Fi(int stream);
extern "C" unsigned char* dialogCreature;

namespace {
__attribute__((always_inline)) inline float constant(unsigned bits) {
    union {
        unsigned word;
        float value;
    } u = {bits};
    return u.value;
}
}

extern "C" void runAnimation__Fv() {
    static int lastAmountPlayed;
    static float correction;
    unsigned char* const creature = dialogCreature;
    if (creature == nullptr) return;
    unsigned char* const state = creature + 288;
    const int index = animFindComponentWithFlags__FP14AnimationStatei(state, 32);
    if (index < 0) return;
    const int amount = soundGetStreamAmountPlayed__Fi(2);
    if (amount != lastAmountPlayed) {
        const float scaled = *reinterpret_cast<float*>(creature + 320 + index * 32) *
                             constant(0x3c888888u);
        const float expected = static_cast<float>(amount - 1024) * constant(0x3898eadbu);
        const float target = scaled - constant(0x3d8f5c28u);
        float difference = expected - target;
        if (difference < 0.0f) difference = -difference;
        lastAmountPlayed = amount;
        if (difference < constant(0x3ca3d70au)) {
            correction = 0.0f;
        } else if (target < expected) {
            correction = constant(0x3dccccccu);
        } else {
            correction = constant(0xbdccccccu);
        }
    }
    const float step = correction;
    if (step == 0.0f) return;
    for (int i = 0; i < static_cast<signed char>(state[4]); ++i) {
        float* const entry = reinterpret_cast<float*>(state + 32 + i * 32);
        *entry = *entry + step;
    }
}
