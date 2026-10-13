#include "../w10-actor-m-common/access.hpp"
// Observed msg_load__8SkeletonPc at 0x246f80, 144 bytes. The base routine msg_load__8CreaturePc runs
// first and returns a byte count n. Four words are then read from input+n, in order, into the words at
// object+968, +976, +1120 and +208 (the last store happens just before the next call). Then
// initType__8Skeletonb runs with (object, 1); it is the routine the authored sources emit as
// norrath_skeleton_init_type. When the signed halfword at object+572 equals 10 the word at +1124 is
// cleared. The result is n + 16. Unknown: the meaning of the fields; the value 10 is a state number.
extern "C" {
int msg_load__8CreaturePc(void* self, char* in);
void norrath_skeleton_init_type(void* self, int flag);
}

extern "C" int msg_load__8SkeletonPc(void* self, char* in) {
    unsigned char* o = static_cast<unsigned char*>(self);
    const int count = msg_load__8CreaturePc(self, in);
    unsigned char* p = reinterpret_cast<unsigned char*>(in) + count;
    w10m::word(o, 968) = w10m::word(p, 0);
    w10m::word(o, 976) = w10m::word(p, 4);
    w10m::word(o, 1120) = w10m::word(p, 8);
    w10m::word(o, 208) = w10m::word(p, 12);
    norrath_skeleton_init_type(self, 1);
    if (w10m::half(o, 572) == 10) w10m::word(o, 1124) = 0;
    return count + 16;
}
