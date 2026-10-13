#include "../w10-actor-m-common/access.hpp"
// Observed msg_save__8SkeletonPc at 0x246f10, 108 bytes. The base routine msg_save__8CreaturePc runs
// first and returns a byte count n. Four words follow at output+n, in this order: the words at
// object+968, +976, +1120 and +208. The result is n + 16. Inferred: the output pointer is the
// second argument, from the base routine's signature. Unknown: what the four fields mean.
extern "C" int msg_save__8CreaturePc(void* self, char* out);

extern "C" int msg_save__8SkeletonPc(void* self, char* out) {
    unsigned char* o = static_cast<unsigned char*>(self);
    const int count = msg_save__8CreaturePc(self, out);
    unsigned char* p = reinterpret_cast<unsigned char*>(out) + count;
    w10m::word(p, 0) = w10m::word(o, 968);
    w10m::word(p, 4) = w10m::word(o, 976);
    w10m::word(p, 8) = w10m::word(o, 1120);
    w10m::word(p, 12) = w10m::word(o, 208);
    return count + 16;
}
