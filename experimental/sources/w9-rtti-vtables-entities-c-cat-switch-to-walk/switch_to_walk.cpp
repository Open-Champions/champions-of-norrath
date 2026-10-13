#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed switchToWalk__3Cat at 0x1eea70, 396 bytes. With the animation state
// at object+288:
//   1. animResetFramestates(state).
//   2. The 32 bytes at object+296 are copied aside (before any animation call).
//   3. The byte at +292 becomes 0.
//   4. animAddHairy(state, header, 0, 1, 0, 1, 0, 513) with the header word at
//      table+116, then animAddHairy(state, header, 0, 0, 0, 1, 0, 513) for the
//      header words at table+156, +164 and +172. The table is the word at
//      object+36, read again before each call.
//   5. The saved 32 bytes are written to object+424; then the singles at +432
//      and +440 become 1.0, the single at +436 becomes the small-data constant
//      at gp-27972 (0x480cac, bit pattern 0xbd088888, the value that
//      subTurnAnimation__3Cat stores), and the byte at +292 becomes 5.
// Inferred: this selects the walk animations. Unknown: the roles of the five
// float arguments of animAddHairy and of the 32-byte block.
extern "C" {
void animResetFramestates__FP14AnimationState(void* state);
void animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi(void* state, void* header, float a, float b, float c, float d, float e, int flags);
}

extern "C" void switchToWalk__3Cat(void* self) {
    using namespace rtti_c;
    const Address object = address(self);
    void* const state = bytes(object + 288u);
    animResetFramestates__FP14AnimationState(state);
    unsigned long long block[4];
    for (unsigned i = 0; i < 4u; ++i) block[i] = read_u64(object + 296u + 8u * i);
    write_u8(object + 292u, 0u);
    animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi(
        state, pointer(read_word(address(pointer(read_word(object + 36u))) + 116u)), 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 513);
    animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi(
        state, pointer(read_word(address(pointer(read_word(object + 36u))) + 156u)), 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 513);
    animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi(
        state, pointer(read_word(address(pointer(read_word(object + 36u))) + 164u)), 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 513);
    animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi(
        state, pointer(read_word(address(pointer(read_word(object + 36u))) + 172u)), 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 513);
    for (unsigned i = 0; i < 4u; ++i) write_u64(object + 424u + 8u * i, block[i]);
    write_single(object + 432u, 1.0f);
    write_single(object + 440u, 1.0f);
    write_u8(object + 292u, 5u);
    write_single(object + 436u, single_from_bits(0xbd088888u));
}
