// Observed initType__5Mummyb at 0x002371a8, 1196 bytes. Arguments: the Mummy object M and a bool B. Steps in order:
//  1. The byte at +156 is cleared.
//  2. The word at +960 (the type) selects the parameters when it is 0..5 (a jump table); a larger type skips
//     this step. Each type calls setParameters__8Creaturefffffi(M, a, b, c, d, e, !B) with five floats:
//       type 0: (1, 1, 1, 1, 1)             type 1: (1, 1, 1, 1, 1)
//       type 2: (1.2, 1.2, 1.2, 1.2, 1.2)   type 3: (0.8, 1, 1, 1, 1)
//       type 4: (1.5, 1.3, 1.4, 1.2, 1.3)   type 5: (1.3, 1, 1, 1.2, 1.2)
//     then, for types 0, 2 and 4: the floats at +88, +100, +96 become -0.2, 0.5, 0.3; for types 1, 3 and 5: the floats
//     at +88, +100, +96, +104 become -0.2, 0.5, 0.3, 0.2 and the byte at +156 becomes 6.
//     (The 1.2-style constants are floats the image reads from its pool near gp - 24276; the bits written below are
//     those words.)
//  3. Four halfwords at +1068, +1076, +1084 and +1092 are set to -1 (an array of four 8-byte records).
//  4. The word at +208 (the mode) selects a record to add or refresh:
//       1: skill slot 25 (the pointer word at skills + 100), record id 38, byte +156 = 7;
//       2: when the type is 2, skill slot 14 (skills + 56), id 22, byte +156 = 3; otherwise skill slot 13
//          (skills + 52), id 21, byte +156 = 2;
//       3: byte +156 = 8;
//       4: byte +156 = 8 and the word at +212 = 1;
//       anything else: nothing more.
//     For modes 1 and 2: A = the integer returned by slot byte 96 of the table at word (skill + 96) called on the skill
//     with (0, (int)worldDifficulty); x = float(A) * 0.2 (pool bits 0x3e4ccccc) * (float(eRand__10GameObject(M)) *
//     2^-16); the first record whose halfword is -1 or equals the id gets: id, halfword[+2] = A, halfword[+4] = A +
//     (int)x, halfword[+6] = 0 (no record is written when all four hold other ids).
// Inferred: this sets creature multipliers for a mummy variant and registers one timed skill effect; the "type" and
// "mode" names are guesses from how the words select behaviour. Unknown: the float meanings and the record layout
// beyond the four halfwords. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern unsigned skills[];
extern float worldDifficulty;
int eRand__10GameObject(void* self);
void setParameters__8Creaturefffffi(void* self, float a, float b, float c, float d, float e, int flag);

namespace {
void register_effect(unsigned char* m, int slot, short id) {
    using namespace w12c;
    unsigned char* const skill = from_word(skills[slot]);
    const int amount = static_cast<int>(call_entry_from(skill, 96, 96, 0, static_cast<unsigned long>(static_cast<int>(worldDifficulty))));
    float scaled = static_cast<float>(amount) * bits(0x3e4cccccu);
    const float roll = static_cast<float>(eRand__10GameObject(m)) * bits(0x37800000u);
    scaled = scaled * roll;
    const int total = amount + static_cast<int>(scaled);
    short* record = reinterpret_cast<short*>(m + 1068);
    for (int k = 0; k < 4; ++k, record += 4) {
        if (record[0] == -1 || record[0] == id) {
            record[0] = id;
            record[3] = 0;
            record[1] = static_cast<short>(amount);
            record[2] = static_cast<short>(total);
            break;
        }
    }
}
}  // namespace

void initType__5Mummyb(void* self, bool flag) {
    using namespace w12c;
    unsigned char* const m = static_cast<unsigned char*>(self);
    m[156] = 0;
    const unsigned type = word(m, 960);
    if (type < 6u) {
        const int invert = flag ^ 1;
        bool second = false;
        switch (type) {
            case 0:
                setParameters__8Creaturefffffi(m, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, invert);
                break;
            case 1:
                second = true;
                setParameters__8Creaturefffffi(m, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, invert);
                break;
            case 2:
                setParameters__8Creaturefffffi(m, bits(0x3f999999u), bits(0x3f999999u), bits(0x3f999999u), bits(0x3f999999u), bits(0x3f999999u), invert);
                break;
            case 3:
                second = true;
                setParameters__8Creaturefffffi(m, bits(0x3f4cccccu), 1.0f, 1.0f, 1.0f, 1.0f, invert);
                break;
            case 4:
                setParameters__8Creaturefffffi(m, 1.5f, bits(0x3fa66666u), bits(0x3fb33333u), bits(0x3f999999u), bits(0x3fa66666u), invert);
                break;
            default:
                second = true;
                setParameters__8Creaturefffffi(m, bits(0x3fa66666u), 1.0f, 1.0f, bits(0x3f999999u), bits(0x3f999999u), invert);
                break;
        }
        if (!second) {
            real(m, 88) = bits(0xbe4cccccu);
            real(m, 100) = 0.5f;
            real(m, 96) = bits(0x3e999999u);
        } else {
            real(m, 88) = bits(0xbe4cccccu);
            real(m, 100) = 0.5f;
            real(m, 96) = bits(0x3e999999u);
            real(m, 104) = bits(0x3e4cccccu);
            m[156] = 6;
        }
    }
    short* record = reinterpret_cast<short*>(m + 1068);
    for (int k = 3; k >= 0; --k) record[4 * k] = -1;
    switch (sword(m, 208)) {
        case 1:
            register_effect(m, 25, 38);
            m[156] = 7;
            break;
        case 2:
            if (word(m, 960) == 2u) {
                register_effect(m, 14, 22);
                m[156] = 3;
            } else {
                register_effect(m, 13, 21);
                m[156] = 2;
            }
            break;
        case 3:
            m[156] = 8;
            break;
        case 4:
            m[156] = 8;
            word(m, 212) = 1u;
            break;
        default:
            break;
    }
}
}
