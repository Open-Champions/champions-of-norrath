#include "../w10-actor-m-common/access.hpp"
// Observed msg_alert__8SkeletonifG6Point3P10GameObject at 0x247010, 180 bytes. The first argument is the
// object, the second an int, the float argument is not used, the third is the address of a Point3
// (the image copies it to the stack and never reads the copy) and the fourth another object.
// Nothing is returned. Only int value 2 acts, and only when the word at object+1120 is nonzero and the
// signed halfword at +572 is 3 or 5 and the other object is not null. Then iatan2__Fff is called with
// (other[20] - object[20], other[16] - object[16]) as floats at those byte offsets; if the result minus
// the signed halfword at object+56, plus 9999, is below 19999 as an unsigned 32-bit number, table entry
// 184 (the setState slot of the Skeleton table view at 0x44fee8) runs on the adjusted object with 15.
// Unknown: the meaning of the int, of +1120, of the states 3 and 5, and which axes +16 and +20 are.
extern "C" int iatan2__Fff(float a, float b);

extern "C" void msg_alert__8SkeletonifG6Point3P10GameObject(void* self, int kind, float, const void*, void* other) {
    unsigned char* o = static_cast<unsigned char*>(self);
    unsigned char* t = static_cast<unsigned char*>(other);
    if (kind != 2) return;
    if (w10m::word(o, 1120) == 0) return;
    const short state = w10m::half(o, 572);
    if (state != 3 && state != 5) return;
    if (t == nullptr) return;
    const int angle = iatan2__Fff(w10m::real(t, 20) - w10m::real(o, 20), w10m::real(t, 16) - w10m::real(o, 16));
    const unsigned delta = static_cast<unsigned>(angle) - static_cast<unsigned>(static_cast<int>(w10m::half(o, 56))) + 9999u;
    if (delta < 19999u) w10m::call_entry(o, 184, 15);
}
