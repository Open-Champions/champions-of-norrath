#include "../w8-actor-virtuals-c-bytes/bytes.hpp"
// Observed setState__8AutoPropi at 0x220f30, 272 bytes.
// Nothing happens unless the signed state is below the signed word at object+288.
// Then: word 376 = state, word 380 = 0, and the 64-bit value at 384 is built.
// It is 0 for state 0 or a negative state; otherwise it is the OR of
// (1 << (i & 31)) for i in 0..state, each 32-bit term sign-extended to 64 bits
// (the image uses 32-bit variable shifts). So states below 31 give a low-bit
// mask of state+1 bits, and state 31 or above gives all 64 bits set.
// modelGetActiveChunks(model at +144, low word of 384) is called; its 64-bit
// result goes to +200 after the byte at +156 is cleared. The state table is
// 8-byte pairs from +296: a header pointer, then a flag word (+300 + 8*state).
// A nonzero header is added to the animation state at object+152 with count 6
// (count 3 when bit 0x2 of the flag word is set) and the state is set to its end.
// Last, the virtual slot at byte offset 152 of the table at object+72 is called
// with the adjusted object and bit 0x1 of the flag word. Word 376 is re-read
// for the table index after the animation calls.
// Inferred: animSetToEnd is the already defined norrath_animation_set_to_end.
// Unknown: the meaning of the state fields, flag bits and slot 152.
extern "C" unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int which);
extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int count);
extern "C" void norrath_animation_set_to_end(void* state);

namespace {
typedef void (*SlotFunction)(void*, int);
}

extern "C" void setState__8AutoPropi(void* self, int state) {
    using namespace w8c;
    const Address base = address(self);
    if (!(state < signed_word(read_word(base + 288u)))) return;
    write_word(base + 376u, static_cast<unsigned>(state));
    write_word(base + 380u, 0u);
    write_u64(base + 384u, 0ull);
    if (state != 0) {
        const int count = state + 1;
        if (count > 0) {
            unsigned long long mask = 0ull;
            for (int i = 0; i < count; ++i) {
                const unsigned bit = 1u << (i & 31);
                mask |= static_cast<unsigned long long>(
                    static_cast<long long>(signed_word(bit)));
            }
            write_u64(base + 384u, mask);
        }
    }
    const unsigned long long chunks = modelGetActiveChunks__FP12_modelHeaderi(
        pointer(read_word(base + 144u)), signed_word(read_word(base + 384u)));
    write_u8(base + 156u, 0u);
    write_u64(base + 200u, chunks);
    unsigned index = read_word(base + 376u);
    if (read_word(base + 296u + 8u * index) != 0u) {
        void* const animation = static_cast<unsigned char*>(self) + 152;
        animAdd__FP14AnimationStateP15AnimationHeaderi(
            animation, pointer(read_word(base + 296u + 8u * index)),
            (read_word(base + 300u + 8u * index) & 2u) != 0u ? 3 : 6);
        norrath_animation_set_to_end(animation);
        index = read_word(base + 376u);
    }
    void* adjusted;
    SlotFunction function = slot_function<SlotFunction>(self, 152u, &adjusted);
    function(adjusted, static_cast<int>(read_word(base + 300u + 8u * index) & 1u));
}
