// Observed getAttackBonus__4itemPQ24item11description at 0x001039f0, 212 bytes
// (a static item member taking one description pointer).
//
// Observed layout of the record argument: word 0 points to a base record whose
// signed halfword at +26 is the base value; four 8-byte entries start at +36,
// each a signed halfword kind then a signed halfword value. The first entry
// whose kind is 35 supplies the value. If that value is non-zero the result is
// base + value * 0.01 * base, otherwise just base; either way the float is
// truncated to an int.
// Inferred: kind 35 is an attack percentage modifier. Unknown: the name and
// meaning of the base halfword and of the other entry kinds.
struct AttackBase {
    unsigned char pad[26];
    short value;
};

struct AttackEntry {
    short kind;
    short value;
    unsigned char unread[4];
};

struct AttackRecord {
    const AttackBase* base;
    unsigned char pad[32];
    AttackEntry entry[4];
};

extern "C" int getAttackBonus__4itemPQ24item11description(const AttackRecord* record) {
    float result = (float)record->base->value;
    int percent = 0;
    for (int i = 0; i < 4; ++i) {
        if (record->entry[i].kind == 35) {
            percent = record->entry[i].value;
            break;
        }
    }
    if (percent != 0) result = result + (float)percent * 0.01f * result;
    return (int)result;
}
