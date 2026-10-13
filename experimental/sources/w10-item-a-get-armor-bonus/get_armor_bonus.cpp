// Observed getArmorBonus__4itemPQ24item11description at 0x00103918, 204 bytes.
// The argument is an item record (the same record getValue__4item reads): +0 is a
// pointer to a template whose signed halfword at +24 is the base value, and four
// 8-byte effect entries start at +36, each {signed halfword tag, signed halfword
// v1, v2, v3}. The first entry (by position) whose tag is 13 supplies v1; with
// none, v1 is 0. The result is base + (int)((float)base * ((float)v1 * 0.01f)),
// where 0.01f is the single-precision word 0x3c23d70a at gp-32492 and the final
// conversion truncates; with v1 equal to 0 the result is just base.
// Unknown: what tag 13 and the base halfword mean.
#include "../inventory-items-record/record.hpp"

extern "C" int getArmorBonus__4itemPQ24item11description(const unsigned char* item) {
    using namespace inventory_items;
    const unsigned char* base = resolve<unsigned char>(*reinterpret_cast<const unsigned*>(item));
    int value = *reinterpret_cast<const short*>(base + 24);
    int percent = 0;
    for (int i = 0; i < 4; ++i) {
        const short* entry = reinterpret_cast<const short*>(item + 36 + 8 * i);
        if (entry[0] == 13) {
            percent = entry[1];
            break;
        }
    }
    if (percent != 0) {
        value += static_cast<int>(static_cast<float>(value) * (static_cast<float>(percent) * 0.01f));
    }
    return value;
}
