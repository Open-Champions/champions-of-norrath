// Observed initAnimation__Fv at 0x001d26e8, 292 bytes. Nothing happens when the
// pointer dialogCreature is zero. Otherwise state = dialogCreature + 288 and the
// 16-byte row of the table anim (0x007d0dc8) selected by the signed byte
// animUseBuffer is used (row word +0 is a header pointer, word +4 a flag, word
// +12 an integer).
//  * Flag zero: header = objectFindAnimation__FP10GameObjectPc(dialogCreature,
//    "talk"); a zero result ends the call. Otherwise animAddTransitionTo(state,
//    header, 0.3f, 3). If dialogIdleAnimation is nonzero, animAddTransitionTo
//    (state, dialogIdleAnimation, 0.4f, 3) follows, and then the float at
//    state + 32 * (signed byte at state + 4) is set to
//    -((float)row[+12] * k * 60.0f) where k is the single 0x3898eadb. The
//    product is evaluated before the second transition call, as in the image.
//  * Flag nonzero: animInitAnimation(row header), animAddTransitionTo(state,
//    row header, 0.3f, 38), then animAddToEnd(state, dialogIdleAnimation, 3)
//    when dialogIdleAnimation is nonzero.
// The three single constants are given as their file bit patterns: 0x3e999999,
// 0x3ecccccc and 0x3898eadb.
// Unknown: what the integer at row +12 counts, and the state layout beyond the
// byte at +4 and the 32-byte float rows.
extern "C" unsigned char* dialogCreature;
extern "C" void* dialogIdleAnimation;
extern "C" signed char animUseBuffer;
struct AnimRow { void* header; int flag; int unused; int count; };
static_assert(sizeof(void*) != 4 || sizeof(AnimRow) == 16, "row stride");
extern "C" AnimRow anim[];
extern "C" void* objectFindAnimation__FP10GameObjectPc(unsigned char*, const char*);
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void*, void*, float, int);
extern "C" void animInitAnimation__FP15AnimationHeader(void*);
extern "C" void animAddToEnd__FP14AnimationStateP15AnimationHeaderi(void*, void*, int);

extern "C" void initAnimation__Fv() {
    if (dialogCreature == 0) return;
    const float k03 = __builtin_bit_cast(float, 0x3e999999u);
    const float k04 = __builtin_bit_cast(float, 0x3ecccccc);
    const float kScale = __builtin_bit_cast(float, 0x3898eadbu);
    AnimRow* row = &anim[animUseBuffer];
    unsigned char* state = dialogCreature + 288;
    if (row->flag == 0) {
        void* header = objectFindAnimation__FP10GameObjectPc(dialogCreature, "talk");
        if (header == 0) return;
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(state, header, k03, 3);
        void* idle = dialogIdleAnimation;
        if (idle == 0) return;
        float value = static_cast<float>(row->count) * kScale;
        value = value * 60.0f;
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(state, idle, k04, 3);
        signed char index = static_cast<signed char>(dialogCreature[292]);
        *reinterpret_cast<float*>(dialogCreature + 288 + index * 32) = -value;
    } else {
        void* header = row->header;
        animInitAnimation__FP15AnimationHeader(header);
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(state, header, k03, 38);
        void* idle = dialogIdleAnimation;
        if (idle != 0) animAddToEnd__FP14AnimationStateP15AnimationHeaderi(state, idle, 3);
    }
}
