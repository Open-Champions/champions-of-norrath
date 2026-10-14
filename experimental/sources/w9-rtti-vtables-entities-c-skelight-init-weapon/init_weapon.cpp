#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed initWeapon__8Skelight at 0x246598, 776 bytes, emitted here as
// norrath_skelight_init_weapon (the name the Skelight constructor imports).
// Clears the byte at object+156 and fills the four halfwords at +1068, +1076,
// +1084 and +1092 with -1; these are the type halfwords of four 8-byte slots
// starting at +1068. Then, by the word at object+208:
//   1, 2 or 3 picks a row: 1 uses the pointer word at 0x34b474 (skills[13],
//   slot type 21, final byte 2), 2 uses 0x34b478 (skills[14], type 22, final
//   byte 3), 3 uses 0x34b470 (skills[12], type 24, final byte 1). The factor is
//   0x3e4ccccc, just under 0.2, in all three rows.
//   Any other value stops there with the byte at +156 left 0.
// For a picked row, the global pointer P is an object whose table pointer is
// the word at P+96; the call is the virtual function at byte offset 96 of that
// table (adjustment halfword, function word) with arguments (P adjusted, 0, 1)
// where 1 is the truncated 1.0 of the small-data constant at gp-11588. Its
// result s is converted to single and scaled by the factor; one
// eRand__10GameObject(object) roll, as a signed single times 2^-16, scales that
// again; a2 is s plus the truncated product. The first of the four slots whose
// type is -1 or already equals the row's type receives: type, s at slot+2, a2
// at slot+4, 0 at slot+6; if no slot qualifies nothing is written. The byte at
// +156 then becomes the row's final byte (also when no slot qualified).
// Inferred: the slots are weapon effect records. Unknown: what skills[12..14]
// are, and the meaning of the call's arguments.
extern "C" {
extern unsigned char skills[];
int eRand__10GameObject(void* object);
}

namespace {
using namespace rtti_c;
struct Row { unsigned skill_word; unsigned type; unsigned final_byte; };
__attribute__((always_inline)) inline Row row_for(unsigned variant) {
    switch (variant) {
    case 1u: return Row{13u, 21u, 2u};
    case 2u: return Row{14u, 22u, 3u};
    default: return Row{12u, 24u, 1u};
    }
}
}

extern "C" void norrath_skelight_init_weapon(void* self) {
    const Address object = address(self);
    write_u8(object + 156u, 0u);
    for (unsigned slot = 0; slot < 4u; ++slot) write_u16(object + 1068u + 8u * slot, 0xffffu);
    const unsigned variant = read_word(object + 208u);
    if (variant - 1u >= 3u) return;
    const Row row = row_for(variant);
    void* const owner = pointer(read_word(address(skills) + 4u * row.skill_word));
    const Address table = address(pointer(read_word(address(owner) + 96u))) + 96u;
    typedef int (*Call)(void*, int, int);
    Call call = reinterpret_cast<Call>(pointer(read_word(table + 4u)));
    const int amount = call(static_cast<unsigned char*>(owner) + read_i16(table), 0, 1);
    float scaled = static_cast<float>(amount) * single_from_bits(0x3e4ccccc);
    const int roll = eRand__10GameObject(self);
    scaled = scaled * (static_cast<float>(roll) * single_from_bits(0x37800000u));
    const int total = amount + static_cast<int>(scaled);
    for (unsigned slot = 0; slot < 4u; ++slot) {
        const Address entry = object + 1068u + 8u * slot;
        const unsigned type = read_u16(entry);
        if (type == 0xffffu || type == row.type) {
            write_u16(entry, row.type);
            write_u16(entry + 6u, 0u);
            write_u16(entry + 2u, static_cast<unsigned>(amount));
            write_u16(entry + 4u, static_cast<unsigned>(total));
            break;
        }
    }
    write_u8(object + 156u, row.final_byte);
}
