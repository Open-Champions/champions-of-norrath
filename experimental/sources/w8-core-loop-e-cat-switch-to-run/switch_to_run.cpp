// Observed switchToRun__3Cat at 0x001eec00, 396 bytes. The argument is the
// Cat object; its layout is only known at the offsets used here.
// The AnimationState at object+288 is reset with animResetFramestates. The
// 32 bytes at object+296 are saved, the byte at +292 is zeroed, and four
// animations are added with animAddHairy(state, header, a, b, c, d, e, 513).
// Each header is a pointer read from the table pointed to by the word at
// object+36, re-read before each call: +116 with floats (0,1,0,1,0), then
// +92, +100 and +108, each with (0,0,0,1,0). The saved 32 bytes are then
// written to object+424, the floats at +432 and +440 become 1.0, the float at
// +436 becomes -1/30 (the bit pattern 0xbd088888 is the constant in the
// image), and the byte at +292 becomes 5.
// Inferred: the +296 and +424 blocks are corresponding slots of the state.
// Unknown: what the five float arguments and flag 513 mean, and what the
// byte value 5 selects.
struct AnimationState;
struct AnimationHeader;
extern "C" void animResetFramestates__FP14AnimationState(AnimationState* state);
extern "C" void animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi(
    AnimationState* state, AnimationHeader* header, float a, float b, float c,
    float d, float e, int flags);

namespace {
struct Block32 {
    unsigned char bytes[32];
} __attribute__((packed));

AnimationHeader* header_at(unsigned char* object, unsigned offset) {
    unsigned char* const table = *reinterpret_cast<unsigned char**>(object + 36);
    return *reinterpret_cast<AnimationHeader**>(table + offset);
}

float negative_thirtieth() {
    union {
        unsigned bits;
        float value;
    } u = {0xbd088888u};
    return u.value;
}
}

extern "C" void switchToRun__3Cat(unsigned char* object) {
    AnimationState* const state = reinterpret_cast<AnimationState*>(object + 288);
    animResetFramestates__FP14AnimationState(state);
    const Block32 saved = *reinterpret_cast<Block32*>(object + 296);
    object[292] = 0;
    animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi(
        state, header_at(object, 116), 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 513);
    animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi(
        state, header_at(object, 92), 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 513);
    animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi(
        state, header_at(object, 100), 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 513);
    animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi(
        state, header_at(object, 108), 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 513);
    *reinterpret_cast<Block32*>(object + 424) = saved;
    *reinterpret_cast<float*>(object + 432) = 1.0f;
    *reinterpret_cast<float*>(object + 440) = 1.0f;
    object[292] = 5;
    *reinterpret_cast<float*>(object + 436) = negative_thirtieth();
}
