#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed setState__14WoodElfSoldieri at 0x248690, 2472 bytes, emitted here as
// norrath_wood_elf_soldier_set_state (the name its constructor imports). The
// animation state is at object+288, the word at +36 points to a header table T
// and the word at +964 is the soldier type (0 to 3 are the ones with
// per-type animations). The requested state lives in a stack word.
//  1. setStateProcessing__8CreaturePi(object, &state); nonzero returns at once.
//  2. Bit 0x200 of the 64-bit word at +160 is set for state 13, else cleared.
//  3. States of 16 and above, and 4, 11 and 12, only store. Otherwise the table
//     at 0x4745f0 selects (R = eRand__10GameObject(object) roll; "t" is
//     animAddTransitionTo and "s" animAddOneShot with both rates equal; the
//     rate is 0.3, 0x3e999999, unless noted "fast" = 0.1, 0x3dcccccc):
//     0: +256 = +808 + R mod 120 + 120; for type 1, bNextToEdge__14WoodElfSoldier
//        nonzero replaces +256 with +808 + (second roll) mod 60; then the
//        per-type transition of state 9.
//     9: transition by type, flags 3: type 0 T+148, 1 T+172, 2 T+244, 3 T+252.
//     1: resetRouting; getSmartRandomDestination(range 300) copied to +272;
//        single at +176 = 100.0; per type transitions (0: T+164, 1: T+212,
//        2: T+164, 3: T+268 and +176 = 150.0), flags 3; animGlobalSync;
//        +256 = +808 + R mod 120 + 300.
//     2: resetRouting; +176 = 100.0; per type (0: T+164, 1: T+212, 2: T+164,
//        3: T+300), flags 3; animGlobalSync.
//     7: resetRouting; +176 = 150.0; per type (0: T+156, 1: T+188, 2: T+156,
//        3: T+268), flags 3; +256 = +808 + 120; animGlobalSync.
//     3: resetRouting; per type (0: T+156 then +176 = 150.0, 1: T+212 then
//        100.0, 2: T+156 then 150.0, 3: T+268 then 150.0), flags 3;
//        animGlobalSync.
//     5: fast; type 0: T+148, 2: T+244, 3: T+260, 1: T+172, flags 3.
//     13: transition T+236, flags 3; +256 = +808 + 60.
//     6: fast, per type a transition and a one-shot (flags 8): type 0: T+148 and
//        T+140, 2: T+244 and T+220, 3: T+260 and T+284 (type 1: none); then
//        +256 = R mod 10.
//     15: fast; type 1: transition T+172 and one-shot T+196, type 3: transition
//        T+260 and one-shot T+196, flags 3 and 8; +256 = R mod 10 + 10.
//     14: fast; type 1: transition T+172 and one-shot T+204; +256 = R mod 60 + 60.
//     8: current halfword at +572 equal to 9 -> state 9; equal to 6 with bit 8
//        of the halfword at +294 -> state 6; otherwise type 2: one-shot T+228,
//        fast, flags 16, state kept; other types: state = current halfword.
//     10: commanderValid__14WoodElfSoldier nonzero calls leaveCommander; when
//        bit 2 of the word at +12 is clear dropItem__14WoodElfSoldieri(object,
//        1); transition T+308, flags 6; dropTreasure__8CreatureG6Point3iffffffPc
//        (object, copy of the 12 bytes at +16, signed halfword at +56, 36.0,
//        2.0, 1.6, 0.6, 1.6, 0.25, NULL name); virtual slot at byte offset 192.
//  4. Unless returned early, the low 16 bits of the state word go to +572.
// Unknown: state names, the roles of types, what the table fields are.
extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
int eRand__10GameObject(void* object);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animGlobalSync__FP14AnimationState(void* state);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
int bNextToEdge__14WoodElfSoldier(void* self);
int commanderValid__14WoodElfSoldier(void* self);
void leaveCommander__14WoodElfSoldier(void* self);
void dropItem__14WoodElfSoldieri(void* self, int kind);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const void* point, int a, float f12, float f13, float f14, float f15, float f16, float f17, char* name);
}

namespace {
using namespace rtti_c;
const unsigned slow = 0x3e999999u;
const unsigned fast = 0x3dccccccu;
__attribute__((always_inline)) inline void* header(Address object, unsigned offset) {
    return pointer(read_word(address(pointer(read_word(object + 36u))) + offset));
}
__attribute__((always_inline)) inline void tr(Address object, unsigned offset, unsigned rate, int flags) {
    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(bytes(object + 288u), header(object, offset), single_from_bits(rate), flags);
}
__attribute__((always_inline)) inline void os(Address object, unsigned offset, unsigned rate, int flags) {
    const float value = single_from_bits(rate);
    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(bytes(object + 288u), header(object, offset), value, value, flags);
}
__attribute__((always_inline)) inline unsigned type_of(Address object) { return read_word(object + 964u); }
__attribute__((always_inline)) inline unsigned roll(void* self, unsigned modulus) {
    return static_cast<unsigned>(eRand__10GameObject(self)) % modulus;
}
// The per-type transition shared by states 0 and 9.
__attribute__((always_inline)) inline void typed(Address object) {
    const unsigned type = type_of(object);
    if (type == 0u) tr(object, 148u, slow, 3);
    else if (type == 1u) tr(object, 172u, slow, 3);
    else if (type == 2u) tr(object, 244u, slow, 3);
    else if (type == 3u) tr(object, 252u, slow, 3);
}
__attribute__((always_inline)) inline void destination(void* self, Address object, int range) {
    resetRouting__8Creature(self);
    unsigned char point[12];
    getSmartRandomDestination__8Creaturei(point, self, range);
    for (unsigned i = 0; i < 12u; ++i) write_u8(object + 272u + i, point[i]);
}
}

extern "C" void norrath_wood_elf_soldier_set_state(void* self, int requested) {
    const Address object = address(self);
    int state = requested;
    if (setStateProcessing__8CreaturePi(self, &state) != 0) return;
    if (state == 13) write_u64(object + 160u, read_u64(object + 160u) | 0x200ull);
    else write_u64(object + 160u, read_u64(object + 160u) & ~0x200ull);
    switch (static_cast<unsigned>(state)) {
    case 0u: {
        write_word(object + 256u, read_word(object + 808u) + roll(self, 120u) + 120u);
        if (type_of(object) == 1u && bNextToEdge__14WoodElfSoldier(self) != 0) {
            write_word(object + 256u, read_word(object + 808u) + roll(self, 60u));
        }
        typed(object);
        break;
    }
    case 9u:
        typed(object);
        break;
    case 1u: {
        destination(self, object, 300);
        write_single(object + 176u, single_from_bits(0x42c80000u));
        if (type_of(object) == 0u) tr(object, 164u, slow, 3);
        if (type_of(object) == 1u) tr(object, 212u, slow, 3);
        if (type_of(object) == 2u) tr(object, 164u, slow, 3);
        if (type_of(object) == 3u) {
            write_single(object + 176u, single_from_bits(0x43160000u));
            tr(object, 268u, slow, 3);
        }
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        write_word(object + 256u, read_word(object + 808u) + roll(self, 120u) + 300u);
        break;
    }
    case 2u:
        resetRouting__8Creature(self);
        write_single(object + 176u, single_from_bits(0x42c80000u));
        if (type_of(object) == 0u) tr(object, 164u, slow, 3);
        if (type_of(object) == 1u) tr(object, 212u, slow, 3);
        if (type_of(object) == 2u) tr(object, 164u, slow, 3);
        if (type_of(object) == 3u) tr(object, 300u, slow, 3);
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        break;
    case 7u:
        resetRouting__8Creature(self);
        write_single(object + 176u, single_from_bits(0x43160000u));
        if (type_of(object) == 0u) tr(object, 156u, slow, 3);
        if (type_of(object) == 1u) tr(object, 188u, slow, 3);
        if (type_of(object) == 2u) tr(object, 156u, slow, 3);
        if (type_of(object) == 3u) tr(object, 268u, slow, 3);
        write_word(object + 256u, read_word(object + 808u) + 120u);
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        break;
    case 3u:
        resetRouting__8Creature(self);
        if (type_of(object) == 0u) {
            tr(object, 156u, slow, 3);
            write_single(object + 176u, single_from_bits(0x43160000u));
        }
        if (type_of(object) == 1u) {
            tr(object, 212u, slow, 3);
            write_single(object + 176u, single_from_bits(0x42c80000u));
        }
        if (type_of(object) == 2u) {
            tr(object, 156u, slow, 3);
            write_single(object + 176u, single_from_bits(0x43160000u));
        }
        if (type_of(object) == 3u) {
            tr(object, 268u, slow, 3);
            write_single(object + 176u, single_from_bits(0x43160000u));
        }
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        break;
    case 5u:
        if (type_of(object) == 0u) tr(object, 148u, fast, 3);
        if (type_of(object) == 2u) tr(object, 244u, fast, 3);
        if (type_of(object) == 3u) tr(object, 260u, fast, 3);
        if (type_of(object) == 1u) tr(object, 172u, fast, 3);
        break;
    case 13u:
        tr(object, 236u, slow, 3);
        write_word(object + 256u, read_word(object + 808u) + 60u);
        break;
    case 6u:
        if (type_of(object) == 0u) {
            tr(object, 148u, fast, 3);
            os(object, 140u, fast, 8);
        }
        if (type_of(object) == 2u) {
            tr(object, 244u, fast, 3);
            os(object, 220u, fast, 8);
        }
        if (type_of(object) == 3u) {
            tr(object, 260u, fast, 3);
            os(object, 284u, fast, 8);
        }
        write_word(object + 256u, roll(self, 10u));
        break;
    case 15u:
        if (type_of(object) == 1u) {
            tr(object, 172u, fast, 3);
            os(object, 196u, fast, 8);
        }
        if (type_of(object) == 3u) {
            tr(object, 260u, fast, 3);
            os(object, 196u, fast, 8);
        }
        write_word(object + 256u, roll(self, 10u) + 10u);
        break;
    case 14u:
        if (type_of(object) == 1u) {
            tr(object, 172u, fast, 3);
            os(object, 204u, fast, 8);
        }
        write_word(object + 256u, roll(self, 60u) + 60u);
        break;
    case 8u: {
        const int current = read_i16(object + 572u);
        if (current == 9) {
            state = 9;
        } else if (current == 6 && (read_u16(object + 294u) & 8u) != 0u) {
            state = 6;
        } else if (type_of(object) == 2u) {
            os(object, 228u, fast, 16);
        } else {
            state = current;
        }
        break;
    }
    case 10u: {
        if (commanderValid__14WoodElfSoldier(self) != 0) leaveCommander__14WoodElfSoldier(self);
        if ((read_word(object + 12u) & 2u) == 0u) dropItem__14WoodElfSoldieri(self, 1);
        tr(object, 308u, slow, 6);
        unsigned char point[12];
        for (unsigned i = 0; i < 12u; ++i) point[i] = static_cast<unsigned char>(read_u8(object + 16u + i));
        dropTreasure__8CreatureG6Point3iffffffPc(self, point, read_i16(object + 56u), single_from_bits(0x42100000u), single_from_bits(0x40000000u),
                                                 single_from_bits(0x3fcccccc), single_from_bits(0x3f199999u), single_from_bits(0x3fcccccc),
                                                 single_from_bits(0x3e800000u), nullptr);
        typedef void (*SlotCall)(void*);
        void* adjusted;
        SlotCall call = slot_function<SlotCall>(self, 192u, &adjusted);
        call(adjusted);
        break;
    }
    default:
        break;
    }
    write_u16(object + 572u, static_cast<unsigned>(state));
}
