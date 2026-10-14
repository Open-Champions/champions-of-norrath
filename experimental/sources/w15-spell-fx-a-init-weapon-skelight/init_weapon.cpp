// Observed: initWeapon__8Skelight at 0x00246598 is 776 bytes in the pinned SLUS-20565 image. Argument: the object o. Nothing is
// returned. D = o+1032 is the base of an item-description-shaped record whose four 8-byte slots start at D+36 (the same layout the
// spell sources build on their stacks). Float constants read through the global pointer, none with a symbol: K = 0.2 (bits
// 0x3e4ccccc, the same value in three adjacent words at 0x00481f34, 0x00481f38 and 0x00481f3c); worldDifficulty is the float
// global at 0x00484cac, and 2^-16 is an immediate.
//  1. The byte at o+156 is cleared and the first halfword of each of the four slots is set to -1.
//  2. The signed word at o+208 selects a case: 1, 2 or 3; any other value leaves the function here (the byte stays zero).
//     The cases differ in the entry of the pointer table skills used (index 13, 14, 12), the kind (21, 22, 24), the pool word used
//     as the factor (the three words above, in that order, all with the same value) and the byte stored at o+156 at the end
//     (2, 3, 1).
//  3. With S = skills[index]: base = the entry at offset 96 of the table that is the word at S+96, called on S (adjusted by the
//     entry's signed halfword) with (0, (int)worldDifficulty). The result is a signed word, base. amount = base + (int)(((float)base
//     * K) * ((float)eRand__10GameObject(o) * 2^-16)).
//  4. The first slot whose first halfword is -1 or the kind receives the kind, then base at +2 (as a halfword), amount at +4 and
//     zero at +6. When all four hold other kinds nothing is written.
//  5. The byte at o+156 gets the case's value (written even when no slot matched).
// Inferred: this prepares the weapon-hit record of a skill-driven light-bearing creature from three skills, with a damage
// amount rolled between the base and a fifth more. Unknown: the meaning of the cases, kinds and the call at table offset 96.
// Nothing here ran on the EE.
#include "../w15-spell-fx-a-common/access.hpp"

extern "C" {
extern unsigned skills[];
extern float worldDifficulty;
int eRand__10GameObject(void* object);

void initWeapon__8Skelight(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const d = o + 1032;
    o[156] = 0;
    for (int entry = 3; entry >= 0; --entry) half(d, 36 + 8 * entry) = -1;
    const int mode = sword(o, 208);
    int index, kind, flag;
    if (mode == 2) { index = 14; kind = 22; flag = 3; }
    else if (mode == 1) { index = 13; kind = 21; flag = 2; }
    else if (mode == 3) { index = 12; kind = 24; flag = 1; }
    else return;
    unsigned char* const skill = from_word(skills[index]);
    unsigned char* const entry = from_word(word(skill, 96)) + 96;
    const unsigned function = *reinterpret_cast<unsigned*>(entry + 4);
#if defined(W12_SPELL_C_HOST)
    const int base = static_cast<int>(w12_spell_c_host_call(function, skill + *reinterpret_cast<short*>(entry), 2, 0,
                                                            static_cast<unsigned long>(static_cast<int>(worldDifficulty))));
#else
    const int base = reinterpret_cast<int (*)(void*, int, int)>(static_cast<__UINTPTR_TYPE__>(function))(
        skill + *reinterpret_cast<short*>(entry), 0, static_cast<int>(worldDifficulty));
#endif
    float roll = static_cast<float>(base) * bits(0x3e4cccccu);
    const int amount = base + static_cast<int>(roll * (static_cast<float>(eRand__10GameObject(self)) * bits(0x37800000u)));
    for (int slot = 0; slot < 4; ++slot) {
        unsigned char* const s = d + 36 + 8 * slot;
        const short first = half(s, 0);
        if (first == -1 || first == kind) {
            half(s, 0) = static_cast<short>(kind);
            half(s, 6) = 0;
            half(s, 2) = static_cast<short>(base);
            half(s, 4) = static_cast<short>(amount);
            break;
        }
    }
    o[156] = static_cast<unsigned char>(flag);
}
}
