#include "../combat-reactions-bytes/bytes.hpp"
// Observed setState__11SmallSpideri at 0x209a10, 524 bytes.
// The argument is kept in a stack word. setStateProcessing is called with the
// object and the address of that word; a nonzero result returns without
// storing anything. Otherwise the word, which the call may have changed,
// selects a path through a jump table of eleven words at 0x46ff50; words at or
// above 11 take the store path. Every path ends by storing the low 16 bits of
// the word at object+572.
// The word at object+36 is a table pointer. The animation state is at
// object+288. The transition rate is the single 0x3e999999 (three .lit4 words
// hold the same pattern).
// 0: eRand modulo 300 unsigned plus 120 plus the word at object+808 is stored
//    at object+256; a transition to the header at table+60 is added, flags 3.
// 1: resetRouting runs; getSmartRandomDestination returns a 12-byte point for
//    the object and range 200, which is stored at object+272; a transition to
//    the header at table+76 is added, flags 3.
// 5, 8, 9: the word becomes the signed halfword already at object+572, so the
//    state does not change.
// 10: bits 0x9 are ORed into the word at object+12. eRand is called twice and
//    its first result, as a signed halfword, is the angle; the second result
//    modulo 10 plus 10, as a float, is the extent. floorAdd gets a 12-byte copy
//    of the point at object+16, the angle, the extent, 6 and 1. Then
//    eRand and 3 selects the header at table + index*8 + 28, a transition to it
//    is added with flags 6, and when bit 0x2 of the word at +12 is set the
//    animation state is set to its end. Finally virtual slot byte 192 is called
//    with this adjusted by that slot's signed halfword and no other argument;
//    the SmallSpider vtable names it death__8Creature.
// 2, 3, 4, 6, 7 and values from 11 up: only the store.
// Inferred: the first eRand in path 10 feeds the angle because the image
// shifts the register in the delay slot of the second call. norrath_actor_random
// is eRand, norrath_animation_add_transition is animAddTransitionTo and
// norrath_animation_set_to_end is animSetToEnd. The point result of
// getSmartRandomDestination comes back through a hidden first pointer.
// Unknown: what the states mean, and the fields at +12 bits 0x9, +256, +272.
extern "C" int setStateProcessing__8CreaturePi(void* self, int* state);
extern "C" unsigned norrath_actor_random(void* object);
extern "C" void norrath_animation_add_transition(void* animation_state, void* animation_header, float rate, int flags);
extern "C" void norrath_animation_set_to_end(void* state);
extern "C" void resetRouting__8Creature(void* self);
extern "C" void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
extern "C" void floorAdd__FG6Point3sfii(const unsigned char* point, int angle, float extent, int kind, int extra);

extern "C" void setState__11SmallSpideri(void* self, int state) {
    using namespace combat_reactions;
    const Address base = address(self);
    unsigned char* animation = static_cast<unsigned char*>(self) + 288u;
    int word = state;
    if (setStateProcessing__8CreaturePi(self, &word) != 0) return;
    switch (static_cast<unsigned>(word)) {
    case 0u: {
        const unsigned wait = norrath_actor_random(self) % 300u;
        const Address table = address(pointer(read_word(base + 36u)));
        write_word(base + 256u, read_word(base + 808u) + wait + 120u);
        norrath_animation_add_transition(animation, pointer(read_word(table + 60u)), single_from_bits(0x3e999999u), 3);
        break;
    }
    case 1u: {
        resetRouting__8Creature(self);
        unsigned char point[12];
        getSmartRandomDestination__8Creaturei(point, self, 200);
        for (unsigned i = 0u; i < 12u; ++i) bytes(base + 272u)[i] = point[i];
        const Address table = address(pointer(read_word(base + 36u)));
        norrath_animation_add_transition(animation, pointer(read_word(table + 76u)), single_from_bits(0x3e999999u), 3);
        break;
    }
    case 5u:
    case 8u:
    case 9u:
        word = read_i16(base + 572u);
        break;
    case 10u: {
        write_word(base + 12u, read_word(base + 12u) | 0x9u);
        const unsigned first = norrath_actor_random(self);
        const unsigned second = norrath_actor_random(self);
        unsigned char point[12];
        for (unsigned i = 0u; i < 12u; ++i) point[i] = bytes(base + 16u)[i];
        const unsigned angle_bits = first & 0xffffu;
        const int angle = angle_bits < 0x8000u ? static_cast<int>(angle_bits) : static_cast<int>(angle_bits) - 65536;
        floorAdd__FG6Point3sfii(point, angle, static_cast<float>(static_cast<int>(second % 10u + 10u)), 6, 1);
        const unsigned index = norrath_actor_random(self) & 3u;
        const Address table = address(pointer(read_word(base + 36u)));
        norrath_animation_add_transition(animation, pointer(read_word(table + index * 8u + 28u)), single_from_bits(0x3e999999u), 6);
        if ((read_word(base + 12u) & 0x2u) != 0u) norrath_animation_set_to_end(animation);
        void* adjusted = nullptr;
        using SlotCall = void (*)(void*);
        SlotCall call = slot_function<SlotCall>(self, 192u, &adjusted);
        call(adjusted);
        break;
    }
    default:
        break;
    }
    write_u16(base + 572u, static_cast<unsigned>(word));
}
