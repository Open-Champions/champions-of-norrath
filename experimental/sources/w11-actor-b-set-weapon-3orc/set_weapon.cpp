// Observed setWeapon__3Orc at 0x00226fb8, 256 bytes. Draws r = eRand__10GameObject(object) and stores a
// value in the word at object+972 chosen by the type word at +1116:
//   type 0: r&8 -> 1; else r&0x16 -> 4; else 3.
//   type 4: word +1124 = 1 and +972 = 2 (r unused).
//   type 1: r&0x16 -> 5; else r&0x32 -> 7; else 14.
//   type 2 or 3: a second eRand r2; 14 + r2 % 3 (unsigned remainder).
//   any other type (5, or above): nothing is written.
// The first eRand call happens before the type is looked at, for every type. Unknown: what the values
// in +972 select (setState code in w10-actor-k reads it). Nothing here ran on the EE.
#include "../w11-actor-b-common/access.hpp"

extern "C" {
unsigned eRand__10GameObject(void* self);

void setWeapon__3Orc(void* self) {
    using namespace w11b;
    unsigned char* o = static_cast<unsigned char*>(self);
    const unsigned r = eRand__10GameObject(self);
    const unsigned type = word(o, 1116);
    if (type == 0 || type == 4) {
        if (type == 4) {
            word(o, 1124) = 1;
            word(o, 972) = 2;
        } else if (r & 8u) {
            word(o, 972) = 1;
        } else if (r & 0x16u) {
            word(o, 972) = 4;
        } else {
            word(o, 972) = 3;
        }
    } else if (type == 1) {
        if (r & 0x16u) word(o, 972) = 5;
        else if (r & 0x32u) word(o, 972) = 7;
        else word(o, 972) = 14;
    } else if (type - 2u < 2u) {
        const unsigned r2 = eRand__10GameObject(self);
        word(o, 972) = 14 + r2 % 3u;
    }
}
}
