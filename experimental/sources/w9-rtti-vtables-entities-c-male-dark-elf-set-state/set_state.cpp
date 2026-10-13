#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed setState__11MaleDarkElfi at 0x288818, 3264 bytes, emitted here as
// norrath_male_dark_elf_set_state (the name the MaleDarkElf constructor
// imports). The animation state is at object+288, the word at +36 points to a
// header table T, and the byte at +960 is the elf kind: "ukind" reads it
// unsigned and "kind" signed. The requested state lives in a stack word.
//  1. setStateProcessing__8CreaturePi(object, &state); nonzero returns at once.
//  2. Bit 0x200 of the 64-bit word at +160 is cleared; states of 17 and above,
//     and 2, 4, 11 and 12, only store. Otherwise the jump table at 0x4776b0
//     selects. "t" is animAddTransitionTo, "s" animAddOneShot (equal rates),
//     all rates 0.1 (0x3dcccccc), R is an eRand__10GameObject roll.
//     0: ukind 4 or 5: t T+76; else kind 3: T+148, 2: T+196, 1: T+252, 0: T+292
//        (flags 3); then +256 = +808 + R mod 60 + 120.
//     1: resetRouting; getSmartRandomDestination(range 200) copied to +272; ukind
//        2 to 4: t T+220; else kind 1: T+268, kind 0: T+300 (flags 2051);
//        animGlobalSync.
//     3: bit 27 of the +160 word cleared, the single at +788 = 1000.0,
//        resetRouting; ukind 2 or 3: T+212; kind 1: T+260; kind 0: T+300; else
//        ukind 4 or 5: T+84 (flags 2051); animGlobalSync.
//     5: kind 2: T+180, 3: T+132, 1: T+252, 5: T+52, 4: T+44 (flags 3).
//     16: the same selection as 5, then bit 0x200 of the +160 word is set and
//        +256 = +808 + 120.
//     7: resetRouting; +256 = +808 + 120; t T+212, flags 2051.
//     9: ukind 4 or 5: T+76; kind 3: T+148, 2: T+196, 1: T+252, 0: T+292 (flags 3).
//     6: kind 2: t T+180 and s T+156+8*(R mod 3); 3: t T+132 and s T+108+8*(R
//        mod 3); 1: t T+252 and s T+228+8*(R&1); 5: t T+52 and s T+28+8*(R&1);
//        4: t T+44 and s T+28+8*(R&1); s flags 2056; then +256 = R mod 20.
//     13, 14, 15: state 15: t T+292 and s T+276; otherwise t T+76 and s T+100
//        (13) or T+92 (14); flags 3 and 2056; then +256 = R mod 15.
//     8: the state becomes the current signed halfword at +572 when it is 9 or
//        6 with bit 8 of the halfword at +294 set; else a one-shot, flags 16:
//        with bit 0x200 of the +160 word set (it was just cleared, so this
//        half is not reached normally): kind 3: T+140, 2: T+188, else T+204;
//        with it clear: kind 1: T+244, 0: T+284, else T+204.
//     10: t T+308+16*(R&3), flags 6; dropTreasure__8CreatureG6Point3iffffffPc
//        (object, copy of +16, halfword +56, 36.0, 2.0, 0x3fe66666, 0x3f199999,
//        2.0, 0.25, NULL). When bit 2 of the word at +12 is clear, for each of
//        the words +964, +968 and +1112 that is nonzero (indexes 0, 1, 2):
//        modelGetCharAttachmentPos(model T+12, copy of +16, halfword +56 minus
//        16384, object+288, index, two point outputs, 1.0), then a 584-byte
//        blockAlloc__Fi and, if it succeeded, the PhProp constructor with the
//        position, the strings T+0, T+8*word and T+8*word+8, 1, a flag word
//        (16385, or for +964 values 52 and 64 8193) and the single at +224; the
//        word is then cleared. After that the virtual slot at byte offset 192.
//  3. Unless returned early, the low 16 bits of the state word go to +572.
// Unknown: the state names, the kinds and what the table fields are.
extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
int eRand__10GameObject(void* object);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animGlobalSync__FP14AnimationState(void* state);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const void* point, int a, float f12, float f13, float f14, float f15, float f16, float f17, char* name);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, const void* point, short a, void* animation, int index, void* out_a, void* out_b, float scale);
void* blockAlloc__Fi(int size);
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, const void* point, char* a, char* b, char* c, long d, int e, void* description, float scale);
}

namespace {
using namespace rtti_c;
const unsigned rate = 0x3dccccccu;
__attribute__((always_inline)) inline void* header(Address object, unsigned offset) {
    return pointer(read_word(address(pointer(read_word(object + 36u))) + offset));
}
__attribute__((always_inline)) inline void tr(Address object, unsigned offset, int flags) {
    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(bytes(object + 288u), header(object, offset), single_from_bits(rate), flags);
}
__attribute__((always_inline)) inline void os(Address object, unsigned offset, int flags) {
    const float value = single_from_bits(rate);
    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(bytes(object + 288u), header(object, offset), value, value, flags);
}
__attribute__((always_inline)) inline unsigned ukind(Address object) { return read_u8(object + 960u); }
__attribute__((always_inline)) inline int skind(Address object) { return read_s8(object + 960u); }
__attribute__((always_inline)) inline unsigned roll(void* self) { return static_cast<unsigned>(eRand__10GameObject(self)); }
// The transition shared by states 0 and 9.
__attribute__((always_inline)) inline void typed_a(Address object) {
    if (ukind(object) - 4u < 2u) { tr(object, 76u, 3); return; }
    const int kind = skind(object);
    if (kind == 3) tr(object, 148u, 3);
    else if (kind == 2) tr(object, 196u, 3);
    else if (kind == 1) tr(object, 252u, 3);
    else if (kind == 0) tr(object, 292u, 3);
}
// The transition shared by states 5 and 16.
__attribute__((always_inline)) inline void typed_b(Address object) {
    const int kind = skind(object);
    if (kind == 2) tr(object, 180u, 3);
    else if (kind == 3) tr(object, 132u, 3);
    else if (kind == 1) tr(object, 252u, 3);
    else if (kind == 5) tr(object, 52u, 3);
    else if (kind == 4) tr(object, 44u, 3);
}
__attribute__((always_inline)) inline void spawn(void* self, Address object, unsigned word_offset, int index) {
    using namespace rtti_c;
    const Address source = object + 16u;
    float position[3], model_point[3], extra[3];
    for (unsigned i = 0; i < 12u; ++i) {
        reinterpret_cast<unsigned char*>(position)[i] = static_cast<unsigned char>(read_u8(source + i));
        reinterpret_cast<unsigned char*>(model_point)[i] = static_cast<unsigned char>(read_u8(source + i));
    }
    modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
        pointer(read_word(address(pointer(read_word(object + 36u))) + 12u)), model_point, static_cast<short>(read_u16(object + 56u) - 16384u),
        bytes(object + 288u), index, position, extra, single_from_bits(0x3f800000u));
    int flags = 16385;
    if (word_offset == 964u) {
        const unsigned type = read_word(object + 964u);
        if (type == 52u || type == 64u) flags = 8193;
    }
    void* block = blockAlloc__Fi(584);
    if (block != nullptr) {
        const Address table = address(pointer(read_word(object + 36u)));
        const unsigned word = read_word(object + word_offset);
        const Address entry = table + word * 8u;
        __6PhPropG6Point3PcN22liPQ24item11descriptionf(block, position, static_cast<char*>(pointer(read_word(table))), static_cast<char*>(pointer(read_word(entry))), static_cast<char*>(pointer(read_word(entry + 8u))),
                                                       1, flags, nullptr, read_single(object + 224u));
    }
    write_word(object + word_offset, 0u);
}
}

extern "C" void norrath_male_dark_elf_set_state(void* self, int requested) {
    const Address object = address(self);
    int state = requested;
    if (setStateProcessing__8CreaturePi(self, &state) != 0) return;
    write_u64(object + 160u, read_u64(object + 160u) & ~0x200ull);
    switch (static_cast<unsigned>(state)) {
    case 0u:
        typed_a(object);
        write_word(object + 256u, read_word(object + 808u) + roll(self) % 60u + 120u);
        break;
    case 1u: {
        resetRouting__8Creature(self);
        unsigned char point[12];
        getSmartRandomDestination__8Creaturei(point, self, 200);
        for (unsigned i = 0; i < 12u; ++i) write_u8(object + 272u + i, point[i]);
        if (ukind(object) - 2u < 3u) tr(object, 220u, 2051);
        else if (skind(object) == 1) tr(object, 268u, 2051);
        else if (skind(object) == 0) tr(object, 300u, 2051);
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        break;
    }
    case 3u:
        write_u64(object + 160u, read_u64(object + 160u) & 0xfffffffff7ffffffull);
        write_single(object + 788u, single_from_bits(0x447a0000u));
        resetRouting__8Creature(self);
        if (ukind(object) - 2u < 2u) tr(object, 212u, 2051);
        else if (skind(object) == 1) tr(object, 260u, 2051);
        else if (skind(object) == 0) tr(object, 300u, 2051);
        else if (((ukind(object) - 4u) & 0xffu) < 2u) tr(object, 84u, 2051);
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        break;
    case 5u:
        typed_b(object);
        break;
    case 16u:
        typed_b(object);
        write_u64(object + 160u, read_u64(object + 160u) | 0x200ull);
        write_word(object + 256u, read_word(object + 808u) + 120u);
        break;
    case 7u:
        resetRouting__8Creature(self);
        write_word(object + 256u, read_word(object + 808u) + 120u);
        tr(object, 212u, 2051);
        break;
    case 9u:
        typed_a(object);
        break;
    case 6u: {
        const int kind = skind(object);
        if (kind == 2) {
            tr(object, 180u, 3);
            os(object, 156u + 8u * (roll(self) % 3u), 2056);
        } else if (kind == 3) {
            tr(object, 132u, 3);
            os(object, 108u + 8u * ((roll(self) % 3u) & 0xffffu), 2056);
        } else if (kind == 1) {
            tr(object, 252u, 3);
            os(object, 228u + 8u * (roll(self) & 1u), 2056);
        } else if (kind == 5) {
            tr(object, 52u, 3);
            os(object, 28u + 8u * (roll(self) & 1u), 2056);
        } else if (kind == 4) {
            tr(object, 44u, 3);
            os(object, 28u + 8u * (roll(self) & 1u), 2056);
        }
        write_word(object + 256u, roll(self) % 20u);
        break;
    }
    case 13u:
    case 14u:
    case 15u:
        if (state == 15) {
            tr(object, 292u, 3);
            os(object, 276u, 2056);
        } else {
            tr(object, 76u, 3);
            if (state == 13) os(object, 100u, 2056);
            else os(object, 92u, 2056);
        }
        write_word(object + 256u, roll(self) % 15u);
        break;
    case 8u: {
        const int current = read_i16(object + 572u);
        if ((current == 6 && (read_u16(object + 294u) & 8u) != 0u) || current == 9) {
            state = current;
        } else if ((read_u64(object + 160u) & 0x200ull) != 0ull) {
            const int kind = skind(object);
            if (kind == 3) os(object, 140u, 16);
            else if (kind == 2) os(object, 188u, 16);
            else os(object, 204u, 16);
        } else {
            const int kind = skind(object);
            if (kind == 1) os(object, 244u, 16);
            else if (kind == 0) os(object, 284u, 16);
            else os(object, 204u, 16);
        }
        break;
    }
    case 10u: {
        const unsigned r = roll(self) & 3u;
        tr(object, 308u + 16u * r, 6);
        unsigned char point[12];
        for (unsigned i = 0; i < 12u; ++i) point[i] = static_cast<unsigned char>(read_u8(object + 16u + i));
        dropTreasure__8CreatureG6Point3iffffffPc(self, point, read_i16(object + 56u), single_from_bits(0x42100000u), single_from_bits(0x40000000u),
                                                 single_from_bits(0x3fe66666u), single_from_bits(0x3f199999u), single_from_bits(0x40000000u),
                                                 single_from_bits(0x3e800000u), nullptr);
        if ((read_word(object + 12u) & 2u) == 0u) {
            if (read_word(object + 964u) != 0u) spawn(self, object, 964u, 0);
            if (read_word(object + 968u) != 0u) spawn(self, object, 968u, 1);
            if (read_word(object + 1112u) != 0u) spawn(self, object, 1112u, 2);
        }
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
