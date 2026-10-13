#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed setState__5Mummyi at 0x239008, 2196 bytes, emitted here as
// norrath_mummy_set_state (the name the Mummy constructor imports). The
// animation state is at object+288, the word at +36 points to a header table T,
// and the word at +960 is the mummy kind: kinds 0 and 2 share one set of
// animations, kind 4 has a second and every other kind a third (written
// "A / B / C" below). The requested state lives in a stack word.
//  1. setStateProcessing__8CreaturePi(object, &state); nonzero returns at once.
//  2. The word at +1104 becomes 0 and terminateSpell__8CreatureRC9type_info is
//     called with the object and __tf15SpellConeOfFire().
//  3. States of 18 and above, and 2, 4, 7, 11 and 12, only store. Otherwise the
//     jump table at 0x472d90 selects. "t" is animAddTransitionTo, "s" is
//     animAddOneShot (both rates equal); R is an eRand__10GameObject roll.
//     Rates are 0.1 (0x3dcccccc) unless noted.
//     16: word +12 &= ~9; the byte-for-byte setWeapon of this class (kind 0 or
//        2: 44, 1 or 3: 48, 4: 46, 5: 50, others unchanged) is stored at +964;
//        norrath_mummy_init_type(object, 0); objectAddToRunList; bit 32 of the
//        +160 word set; halfwords 50, 128, 255, 150 at +146, +150, +152, +154
//        and 0, 128, 255, 0 at +140, +142, +144, +148; the state becomes 0 and
//        state 0 runs.
//     0: transition A T+84, B T+204, C T+324, flags 3; +256 = +808 + R mod 120.
//     1: resetRouting; getSmartRandomDestination(range 200) copied to +272;
//        transition A T+108, B T+252, C T+340, flags 2051; animGlobalSync.
//     3: resetRouting; when the current halfword is 6 or 14 only the store;
//        else transition A T+100, B T+244, C T+340, flags 2051; animGlobalSync.
//     17: resetRouting; transition A T+92, B T+236, C T+332, flags 2051;
//        animGlobalSync.
//     5: transition A T+84, B T+164, C T+324, flags 3; animGlobalSync; +256 =
//        +808 + R mod 120 + 60.
//     15: +1112 = +808 + 600; transition T+84, flags 3; one-shot T+348, flags
//        2056.
//     6: A: one-shot T+28+8*(R&1); B/C: T+116+8*(R mod 3); flags 2056.
//     14: one-shot T+140+8*(R mod 3), flags 2056.
//     13: transition T+84, flags 3; one-shot T+260+8*(R mod 3), flags 2056; a
//        second roll R2: +256 = (R2 as signed single * 2^-16 * 120.0, truncated)
//        plus 240.
//     8: the state becomes the current signed halfword at +572 when it is 9,
//        or 6 or 14 with bit 8 of the halfword at +294 set; otherwise a
//        one-shot, flags 16: A T+76, B T+196, C T+316.
//     9: transition T+84, 0.3 (0x3e999999), flags 3.
//     10: when bit 0x1000000 of the +160 word is set, fxCreatureToDust(model
//        T+12, object+288, copy of the 12 bytes at +16, signed halfword +56,
//        singles at +224 and +228), word +12 |= 0x4002, bit 41 of the +160 word
//        set, objectAddToDeleteList; else, unless bit 32 of the +160 word is
//        set, dropTreasure__8CreatureG6Point3iffffffPc(object, copy of +16,
//        halfword +56, 36.0, 2.0, 1.6, 0.6, 1.4, 0.25, "mummytongue"); then a
//        roll chooses a transition, flags 2054, 0.3: A T+60+8*(R&1), B
//        T+180+8*(R&1), C T+300+8*(R&1). Then dropWeapon__5Mummy and the virtual
//        slot at byte offset 192.
//  4. Unless returned early, the low 16 bits of the state word go to +572.
// Unknown: the state names, the kinds and the table layout.
extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
const void* __tf15SpellConeOfFire();
void terminateSpell__8CreatureRC9type_info(void* self, const void* type);
void norrath_mummy_init_type(void* self, int flag);
int eRand__10GameObject(void* object);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animGlobalSync__FP14AnimationState(void* state);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
void objectAddToRunList__FP10GameObject(void* object);
void objectAddToDeleteList__FP10GameObject(void* object);
void fxCreatureToDust__FP12_modelHeaderP14AnimationStateG6Point3sff(void* model, void* animation, const void* point, short a, float b, float c);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const void* point, int a, float f12, float f13, float f14, float f15, float f16, float f17, char* name);
void dropWeapon__5Mummy(void* self);
}

namespace {
using namespace rtti_c;
const unsigned fast = 0x3dccccccu;
__attribute__((always_inline)) inline void* header(Address object, unsigned offset) {
    return pointer(read_word(address(pointer(read_word(object + 36u))) + offset));
}
__attribute__((always_inline)) inline void tr(Address object, unsigned offset, unsigned rate, int flags) {
    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(bytes(object + 288u), header(object, offset), single_from_bits(rate), flags);
}
__attribute__((always_inline)) inline void os_at(Address object, void* h, unsigned rate, int flags) {
    const float value = single_from_bits(rate);
    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(bytes(object + 288u), h, value, value, flags);
}
__attribute__((always_inline)) inline bool kind_ab(Address object) {
    const unsigned kind = read_word(object + 960u);
    return kind == 0u || kind == 2u;
}
__attribute__((always_inline)) inline bool kind_b(Address object) { return read_word(object + 960u) == 4u; }
// A / B / C transition selection: kinds 0 and 2, kind 4, anything else.
__attribute__((always_inline)) inline void by_kind(Address object, unsigned a, unsigned b, unsigned c, unsigned rate, int flags) {
    if (kind_ab(object)) tr(object, a, rate, flags);
    else if (kind_b(object)) tr(object, b, rate, flags);
    else tr(object, c, rate, flags);
}
__attribute__((always_inline)) inline unsigned roll(void* self) { return static_cast<unsigned>(eRand__10GameObject(self)); }
__attribute__((always_inline)) inline void state_zero(void* self, Address object) {
    by_kind(object, 84u, 204u, 324u, fast, 3);
    write_word(object + 256u, read_word(object + 808u) + roll(self) % 120u);
}
}

extern "C" void norrath_mummy_set_state(void* self, int requested) {
    const Address object = address(self);
    int state = requested;
    if (setStateProcessing__8CreaturePi(self, &state) != 0) return;
    write_word(object + 1104u, 0u);
    terminateSpell__8CreatureRC9type_info(self, __tf15SpellConeOfFire());
    switch (static_cast<unsigned>(state)) {
    case 16u: {
        write_word(object + 12u, read_word(object + 12u) & 0xfffffff6u);
        switch (read_word(object + 960u)) {
        case 0u:
        case 2u:
            write_word(object + 964u, 44u);
            break;
        case 1u:
        case 3u:
            write_word(object + 964u, 48u);
            break;
        case 4u:
            write_word(object + 964u, 46u);
            break;
        case 5u:
            write_word(object + 964u, 50u);
            break;
        default:
            break;
        }
        norrath_mummy_init_type(self, 0);
        objectAddToRunList__FP10GameObject(self);
        write_u64(object + 160u, read_u64(object + 160u) | (1ull << 32));
        write_u16(object + 146u, 50u);
        write_u16(object + 150u, 128u);
        write_u16(object + 152u, 255u);
        write_u16(object + 154u, 150u);
        state = 0;
        write_u16(object + 140u, 0u);
        write_u16(object + 142u, 128u);
        write_u16(object + 144u, 255u);
        write_u16(object + 148u, 0u);
        state_zero(self, object);
        break;
    }
    case 0u:
        state_zero(self, object);
        break;
    case 1u: {
        resetRouting__8Creature(self);
        unsigned char point[12];
        getSmartRandomDestination__8Creaturei(point, self, 200);
        for (unsigned i = 0; i < 12u; ++i) write_u8(object + 272u + i, point[i]);
        by_kind(object, 108u, 252u, 340u, fast, 2051);
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        break;
    }
    case 3u:
        resetRouting__8Creature(self);
        if (read_i16(object + 572u) != 6 && read_i16(object + 572u) != 14) {
            by_kind(object, 100u, 244u, 340u, fast, 2051);
            animGlobalSync__FP14AnimationState(bytes(object + 288u));
        }
        break;
    case 17u:
        resetRouting__8Creature(self);
        by_kind(object, 92u, 236u, 332u, fast, 2051);
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        break;
    case 5u:
        by_kind(object, 84u, 164u, 324u, fast, 3);
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        write_word(object + 256u, read_word(object + 808u) + roll(self) % 120u + 60u);
        break;
    case 15u:
        write_word(object + 1112u, read_word(object + 808u) + 600u);
        tr(object, 84u, fast, 3);
        os_at(object, header(object, 348u), fast, 2056);
        break;
    case 6u:
        if (kind_ab(object)) {
            const unsigned r = roll(self);
            os_at(object, header(object, 28u + 8u * (r & 1u)), fast, 2056);
        } else {
            const unsigned r = roll(self) % 3u;
            os_at(object, header(object, 116u + 8u * r), fast, 2056);
        }
        break;
    case 14u: {
        const unsigned r = roll(self) % 3u;
        os_at(object, header(object, 140u + 8u * r), fast, 2056);
        break;
    }
    case 13u: {
        tr(object, 84u, fast, 3);
        const unsigned r = roll(self) % 3u;
        os_at(object, header(object, 260u + 8u * r), fast, 2056);
        const float scaled = static_cast<float>(static_cast<int>(roll(self))) * single_from_bits(0x37800000u) * single_from_bits(0x42f00000u);
        write_word(object + 256u, static_cast<unsigned>(static_cast<int>(scaled)) + 240u);
        break;
    }
    case 8u: {
        const int current = read_i16(object + 572u);
        if (current == 9 || ((current == 6 || current == 14) && (read_u16(object + 294u) & 8u) != 0u)) {
            state = current;
        } else if (kind_ab(object)) {
            os_at(object, header(object, 76u), fast, 16);
        } else if (kind_b(object)) {
            os_at(object, header(object, 196u), fast, 16);
        } else {
            os_at(object, header(object, 316u), fast, 16);
        }
        break;
    }
    case 9u:
        tr(object, 84u, 0x3e999999u, 3);
        break;
    case 10u: {
        if ((read_u64(object + 160u) & 0x1000000ull) != 0ull) {
            unsigned char point[12];
            for (unsigned i = 0; i < 12u; ++i) point[i] = static_cast<unsigned char>(read_u8(object + 16u + i));
            fxCreatureToDust__FP12_modelHeaderP14AnimationStateG6Point3sff(
                pointer(read_word(address(pointer(read_word(object + 36u))) + 12u)), bytes(object + 288u), point, static_cast<short>(read_i16(object + 56u)),
                read_single(object + 224u), read_single(object + 228u));
            write_word(object + 12u, read_word(object + 12u) | 0x4002u);
            write_u64(object + 160u, read_u64(object + 160u) | (1ull << 41));
            objectAddToDeleteList__FP10GameObject(self);
        } else {
            if ((read_u64(object + 160u) & (1ull << 32)) == 0ull) {
                unsigned char point[12];
                for (unsigned i = 0; i < 12u; ++i) point[i] = static_cast<unsigned char>(read_u8(object + 16u + i));
                dropTreasure__8CreatureG6Point3iffffffPc(self, point, read_i16(object + 56u), single_from_bits(0x42100000u), single_from_bits(0x40000000u),
                                                         single_from_bits(0x3fcccccc), single_from_bits(0x3f199999u), single_from_bits(0x3fb33333u),
                                                         single_from_bits(0x3e800000u), const_cast<char*>("mummytongue"));
            }
            const unsigned r = roll(self) & 1u;
            if (kind_ab(object)) tr(object, 60u + 8u * r, 0x3e999999u, 2054);
            else if (kind_b(object)) tr(object, 180u + 8u * r, 0x3e999999u, 2054);
            else tr(object, 300u + 8u * r, 0x3e999999u, 2054);
        }
        dropWeapon__5Mummy(self);
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
