// Observed initType__3Orcb at 0x002270b8, 1408 bytes. flag is the boolean argument. In order:
//   1. byte +156 = 0; the doubleword at +160 loses bits 33, 34 and 35 (stored at once).
//   2. By the word at +1116 (t): t = 0 or 4: setParameters__8Creaturefffffi(object, 1.1, 0.8, 1.1, 0.8, 0.8,
//      flag ^ 1) and bit 33 set in +160; t = 1: (2.0, 2.0, 1.3, 2.0, 2.0) and bit 35; t = 2: (1.8, 1.2, 1.3,
//      1.2, 1.2) and bit 35; t = 3: float +28 = 22.0, float +224 = 1.3, then (2.6, 1.2, 1.3, 1.2, 1.4) and bit 35;
//      t = 5: (1.3, 1.0, 1.3, 1.0, 1.0), bit 34 and byte +156 = 6; any other t: nothing more. Every number
//      is the single read from the image (for example 1.1 is 0x3f8ccccc, one ulp below the nearest literal);
//      bits are set on the doubleword read again after the call.
//   3. The halfwords at +1076, +1084, +1092 and +1100 (four 8-byte slots) become -1.
//   4. By the word at +208 (u): u = 1: if t is 4 byte +156 = 3, else slot row (skills word 12, type 24) and
//      byte 1; u = 2: if t is 4 byte +156 = 2, else row (14, 22) and byte 3; u = 3: if t is 4 byte +156 = 2,
//      else row (13, 21) and byte 2; u = 4: row (26, 23) and byte 4; anything else nothing.
//   A slot row: owner = the skills word named by the first number; its table is the word at owner+96 and the
//   entry at byte 96 of that table (adjustment halfword, function word) is called with (owner adjusted, 0,
//   (int)worldDifficulty) and returns an amount s; total = s + (int)(s * 0.2 * (eRand__10GameObject(object) *
//   2^-16)), the 0.2 being the single 0x3e4ccccc; the first of the four slots whose type halfword is -1 or
//   already equal to the row's type receives type, s at +2, total at +4 and 0 at +6.
// Unknown: what the flag word bits, the numbers and +208, +1116 select, and what skills[] holds.
// Nothing here ran on the EE.
#include "../w11-actor-b-common/access.hpp"

extern "C" {
extern unsigned char skills[];
extern float worldDifficulty;
int eRand__10GameObject(void* object);
void setParameters__8Creaturefffffi(void* self, float a, float b, float c, float d, float e, int flag);
}

namespace {
__attribute__((always_inline)) inline void add_slot(unsigned char* o, unsigned skillsWord, short type) {
    using namespace w11b;
    unsigned char* owner = from_word(word(skills, 4 * skillsWord));
    const int amount = static_cast<int>(call_entry_from(owner, 96, 96, 0, static_cast<unsigned long>(static_cast<int>(worldDifficulty))));
    float scaled = static_cast<float>(amount) * bits(0x3e4ccccc);
    const int roll = eRand__10GameObject(o);
    scaled = scaled * (static_cast<float>(roll) * bits(0x37800000));
    const int total = amount + static_cast<int>(scaled);
    for (int slot = 0; slot < 4; ++slot) {
        short* entry = reinterpret_cast<short*>(o + 1076 + 8 * slot);
        if (entry[0] == -1 || entry[0] == type) {
            entry[0] = type;
            entry[3] = 0;
            entry[1] = static_cast<short>(amount);
            entry[2] = static_cast<short>(total);
            break;
        }
    }
}
}  // namespace

extern "C" void initType__3Orcb(void* self, int flag) {
    using namespace w11b;
    unsigned char* o = static_cast<unsigned char*>(self);
    *(o + 156) = 0;
    dword(o, 160) &= 0xfffffff1ffffffffull;
    const unsigned t = word(o, 1116);
    const int inverse = flag ^ 1;
    if (t == 0 || t == 4) {
        setParameters__8Creaturefffffi(o, bits(0x3f8ccccc), bits(0x3f4ccccc), bits(0x3f8ccccc), bits(0x3f4ccccc), bits(0x3f4ccccc), inverse);
        dword(o, 160) |= 1ull << 33;
    } else if (t == 1) {
        setParameters__8Creaturefffffi(o, 2.0f, 2.0f, bits(0x3fa66666), 2.0f, 2.0f, inverse);
        dword(o, 160) |= 1ull << 35;
    } else if (t - 2u < 2u) {
        if (t == 3) {
            real(o, 28) = 22.0f;
            real(o, 224) = bits(0x3fa66666);
            setParameters__8Creaturefffffi(o, bits(0x40266666), bits(0x3f999999), bits(0x3fa66666), bits(0x3f999999), bits(0x3fb33333), inverse);
        } else {
            setParameters__8Creaturefffffi(o, bits(0x3fe66666), bits(0x3f999999), bits(0x3fa66666), bits(0x3f999999), bits(0x3f999999), inverse);
        }
        dword(o, 160) |= 1ull << 35;
    } else if (t == 5) {
        setParameters__8Creaturefffffi(o, bits(0x3fa66666), 1.0f, bits(0x3fa66666), 1.0f, 1.0f, inverse);
        dword(o, 160) |= 1ull << 34;
        *(o + 156) = 6;
    }
    half(o, 1076) = -1;
    half(o, 1084) = -1;
    half(o, 1092) = -1;
    half(o, 1100) = -1;
    switch (word(o, 208)) {
    case 1:
        if (t == 4) {
            *(o + 156) = 3;
        } else {
            add_slot(o, 12, 24);
            *(o + 156) = 1;
        }
        break;
    case 2:
        if (t == 4) {
            *(o + 156) = 2;
        } else {
            add_slot(o, 14, 22);
            *(o + 156) = 3;
        }
        break;
    case 3:
        if (t == 4) {
            *(o + 156) = 2;
        } else {
            add_slot(o, 13, 21);
            *(o + 156) = 2;
        }
        break;
    case 4:
        add_slot(o, 26, 23);
        *(o + 156) = 4;
        break;
    default:
        break;
    }
}
