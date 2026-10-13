#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_alert__13FemaleDarkElfifG6Point3P10GameObject at 0x2895c8, 180 bytes.
// Acts only when the kind (second argument) is 2, the word at object+1104 is non-zero, the signed
// halfword at object+572 is 3 or 5, and the source object is non-null. The float and the Point3
// arguments are not used (the original copies the Point3 to its stack and never reads it).
// The heading is iatan2(source.y - object.y, source.x - object.x) from the floats at +20 and +16
// of each. When heading - halfword(object+56) + 9999 is below 19999 as an unsigned value
// (that is, the heading is within 9999 of the object's halfword), virtual slot 23 (byte offset 184
// of the table at object+72) is called with the adjusted object and 14.
// Unknown: what state 14 is and what the halfword at +56 holds besides this heading comparison.
extern "C" int iatan2__Fff(float y, float x);
extern "C" void msg_alert__13FemaleDarkElfifG6Point3P10GameObject(void* self, int kind, float unused,
                                                                   const void* position, void* source) {
    using namespace vslots_b;
    (void)unused;
    (void)position;
    if (kind != 2) return;
    if (read_word(address(self) + 1104u) == 0u) return;
    const int state = read_i16(address(self) + 572u);
    if (state != 3 && state != 5) return;
    if (source == nullptr) return;
    const Address s = address(source);
    const Address o = address(self);
    const int heading = iatan2__Fff(read_single(s + 20u) - read_single(o + 20u), read_single(s + 16u) - read_single(o + 16u));
    if (static_cast<unsigned>(heading - read_i16(o + 56u) + 9999) < 19999u) {
        void* adjusted;
        slot_function<void (*)(void*, int)>(self, 184u, &adjusted)(adjusted, 14);
    }
}
