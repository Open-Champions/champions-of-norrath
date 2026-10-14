#include "../save-and-quests-bytes/bytes.hpp"
// Observed msg_load__8CreaturePc at 0x00111b90, 608 bytes; it reads what
// msg_save__8CreaturePc wrote. Fields keep their observed byte offsets and are
// not named. The vtable pointer is the word at object+72; a virtual call
// loads a signed halfword adjustment and the function pointer four bytes later.
// Buffer order and effects:
//   word -> object+244; word, converted from integer to single -> object+560;
//   word -> string pool lookup (objectSaveGetFromStringPool) -> object+892.
//   Then the virtual slot at vtable byte 200 is called on the object.
//   If the single at object+560 (just stored from an integer) is at most zero:
//     object+12 gains bits 0x2009, the halfword 10 goes to object+572, one
//     word is read and the object is taken off the run list; that word is a
//     pool lookup. A non-null result is searched in the table at the pointer
//     in object+36 (8-byte entries, text pointer then header pointer), from
//     entry 1 until a zero text pointer, with strcmp. On a match (entry text
//     still nonzero) the animation state at object+288 is initialised, the
//     entry's header added with the value 4, set to its end, and the virtual
//     slot at vtable byte 192 is called.
//     Then, for every such object, the dynamic type is read through the
//     function at vtable byte 4 (the type-info slot); a null object calls
//     __throw_bad_typeid. When that type-info equals the one from __tf3Orc,
//     byte 70 goes to object+40.
//   If bit 0 of the 64-bit flags at object+160 is set: a word as a single to
//   object+564 and a word's low byte to object+76.
//   Then a byte to object+570, a mask byte, and two skipped bytes. The mask
//   sets flag bit 24 (mask 1), bit 32 (2), bit 22 (4) and bit 23 (8).
// The return is the number of bytes consumed.
extern "C" {
const char* objectSaveGetFromStringPool__Fi(int index);
void objectRemoveFromRunList__FP10GameObject(void* object);
void animInitState__FP14AnimationState(void* state);
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int count);
void animSetToEnd__FP14AnimationState(void* state);
int strcmp(const char* left, const char* right);
const void* __tf3Orc();
int __eq__C9type_infoRC9type_info(const void* left, const void* right);
void __throw_bad_typeid();
}

namespace {
void call_slot(unsigned char* object, unsigned slot_byte_offset) {
    using namespace sq;
    const unsigned char* slot = static_cast<const unsigned char*>(pointer(read_word(object + 72))) + slot_byte_offset;
    const int adjustment = static_cast<short>(static_cast<unsigned short>(slot[0] | (slot[1] << 8)));
    reinterpret_cast<void (*)(void*)>(pointer(read_word(slot + 4)))(object + adjustment);
}
}

extern "C" int msg_load__8CreaturePc(void* self, char* buffer) {
    using namespace sq;
    unsigned char* object = static_cast<unsigned char*>(self);
    const unsigned char* start = reinterpret_cast<const unsigned char*>(buffer);
    const unsigned char* in = start;
    write_word(object + 244, read_word(in));
    in += 4;
    write_single(object + 560, static_cast<float>(signed_word(read_word(in))));
    in += 4;
    const char* name_text = objectSaveGetFromStringPool__Fi(signed_word(read_word(in)));
    in += 4;
    write_word(object + 892, pointer_bits(name_text));
    call_slot(object, 200);
    if (!(read_single(object + 560) > 0.0f)) {
        write_word(object + 12, read_word(object + 12) | 0x2009u);
        object[572] = 10;
        object[573] = 0;
        in += 4;
        objectRemoveFromRunList__FP10GameObject(self);
        const char* wanted = objectSaveGetFromStringPool__Fi(signed_word(read_word(in - 4)));
        if (wanted != nullptr) {
            for (unsigned index = 1;; ++index) {
                const unsigned char* table = static_cast<const unsigned char*>(pointer(read_word(object + 36)));
                const unsigned text = read_word(table + index * 8);
                if (text == 0) break;
                if (strcmp(static_cast<const char*>(pointer(text)), wanted) != 0) continue;
                const unsigned char* again = static_cast<const unsigned char*>(pointer(read_word(object + 36)));
                if (read_word(again + index * 8) == 0) break;
                animInitState__FP14AnimationState(object + 288);
                table = static_cast<const unsigned char*>(pointer(read_word(object + 36)));
                animAdd__FP14AnimationStateP15AnimationHeaderi(object + 288, pointer(read_word(table + index * 8 + 4)), 4);
                animSetToEnd__FP14AnimationState(object + 288);
                call_slot(object, 192);
                break;
            }
        }
        if (self == nullptr) __throw_bad_typeid();
        const unsigned char* vtable = static_cast<const unsigned char*>(pointer(read_word(object + 72)));
        const void* dynamic_type = reinterpret_cast<const void* (*)()>(pointer(read_word(vtable + 4)))();
        if (__eq__C9type_infoRC9type_info(dynamic_type, __tf3Orc()) != 0) object[40] = 70;
    }
    unsigned long long flags = read_u64(object + 160);
    if ((flags & 1u) != 0u) {
        write_single(object + 564, static_cast<float>(signed_word(read_word(in))));
        in += 4;
        object[76] = static_cast<unsigned char>(read_word(in));
        in += 4;
    }
    object[570] = in[0];
    const unsigned mask = in[1];
    in += 2;
    if ((mask & 1u) != 0u) flags |= 0x01000000ull;
    if ((mask & 2u) != 0u) flags |= 0x100000000ull;
    if ((mask & 4u) != 0u) flags |= 0x400000ull;
    if ((mask & 8u) != 0u) flags |= 0x800000ull;
    write_word(object + 160, static_cast<unsigned>(flags));
    write_word(object + 164, static_cast<unsigned>(flags >> 32));
    return static_cast<int>(in + 2 - start);
}
