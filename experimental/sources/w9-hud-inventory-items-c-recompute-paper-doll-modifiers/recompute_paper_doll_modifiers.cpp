// Observed recomputePaperDollModifiers__Fi at 0x001a4a90, 700 bytes.
// Argument: a player number. record is the 23984-byte player record and object is
// the pointer word at record+21872; with a zero object the result is 0 and nothing
// else happens. Otherwise:
//  1. The five floats at object+88..object+104 are cleared.
//  2. held is the word at record+21960+4*b, b being the signed byte at record+23572.
//  3. For index 0..15 over the equipped-word table at record+21896 (see the removal
//     unit): the index is skipped when held is non-zero, the index is 3 and the
//     signed byte at +30 of held's description is 4. Otherwise a zero equipped word
//     is skipped; for a slot, with row = its description:
//       - when row byte +29 is 1, getArmorBonus(slot) is added to the result;
//       - for k = 0..4 the slot's attribute list (four 8-byte pairs starting at
//         slot+36: halfword id, then halfword value) is searched for id k (first
//         match, else 0) and (float)value * 0.01f (single precision; 0x3c23d70a)
//         is added to the float at object+88+4k;
//       - when the row byte +29 (read again through the equipped word) is not 1,
//         the value for id 13 is added to the result;
//       - the value for id 5 is added (as a float) to the float at record+176 and
//         the value for id 6 to the float at record+200.
//  4. The result is the accumulated sum plus (signed word at record+152) / 2,
//     rounded toward zero.
// Unknown: what ids 0..6 and 13 are; what the +29/+30 row bytes mean.
#include "../inventory-items-record/record.hpp"
extern "C" int getArmorBonus__4itemPQ24item11description(const void* slot);

namespace {
unsigned& word(unsigned char* base, unsigned offset) { return *reinterpret_cast<unsigned*>(base + offset); }
float& real(unsigned char* base, unsigned offset) { return *reinterpret_cast<float*>(base + offset); }
// The value paired with `id` in the slot's four attribute pairs, or 0.
int attribute(const unsigned char* slot, int id) {
    for (int pair = 0; pair < 4; ++pair) {
        if (*reinterpret_cast<const short*>(slot + 36 + 8 * pair) == id) return *reinterpret_cast<const short*>(slot + 38 + 8 * pair);
    }
    return 0;
}
signed char row_byte(const unsigned char* slot, unsigned offset) {
    const unsigned char* row = inventory_items::resolve<unsigned char>(*reinterpret_cast<const unsigned*>(slot));
    return static_cast<signed char>(row[offset]);
}
}

extern "C" int recomputePaperDollModifiers__Fi(int owner) {
    using namespace inventory_items;
    unsigned char* record = player_record(owner);
    if (word(record, 21872) == 0) return 0;
    unsigned char* object = const_cast<unsigned char*>(resolve<unsigned char>(word(record, 21872)));
    int result = 0;
    for (int k = 4; k >= 0; --k) real(object, 88 + 4 * k) = 0;
    const int selected = static_cast<signed char>(record[23572]);
    const unsigned held = word(record, 21960 + 4 * selected);
    for (int index = 0; index < 16; ++index) {
        if (held != 0 && index == 3 && row_byte(resolve<unsigned char>(held), 30) == 4) continue;
        const unsigned equipped = word(record, 21896 + 4 * index);
        if (equipped == 0) continue;
        const unsigned char* slot = resolve<unsigned char>(equipped);
        if (row_byte(slot, 29) == 1) {
            result += getArmorBonus__4itemPQ24item11description(slot);
            object = const_cast<unsigned char*>(resolve<unsigned char>(word(record, 21872)));
        }
        for (int k = 0; k < 5; ++k) real(object, 88 + 4 * k) += static_cast<float>(attribute(slot, k)) * 0.01f;
        if (row_byte(resolve<unsigned char>(word(record, 21896 + 4 * index)), 29) != 1) result += attribute(slot, 13);
        real(record, 176) += static_cast<float>(attribute(slot, 5));
        real(record, 200) += static_cast<float>(attribute(slot, 6));
    }
    const int base = static_cast<int>(word(record, 152));
    return result + base / 2;
}
