#include "../w10-actor-m-common/access.hpp"
// Observed msg_save__8SkelightPc at 0x247288, 128 bytes. The base routine msg_save__8CreaturePc runs
// first and returns a byte count n. At output+n it then writes, in order: the signed byte at
// object+85 widened to a word, the word at +1104, the word at +960, the word at +208, and one byte that is
// 1 when the 64-bit word at +160 has any bit of 0x00c00000 set and 0 otherwise. The result is n + 17.
// Unknown: what the fields mean; the weapon routine in this class reads the word at +208.
extern "C" int msg_save__8CreaturePc(void* self, char* out);

extern "C" int msg_save__8SkelightPc(void* self, char* out) {
    unsigned char* o = static_cast<unsigned char*>(self);
    const int count = msg_save__8CreaturePc(self, out);
    unsigned char* p = reinterpret_cast<unsigned char*>(out) + count;
    w10m::word(p, 0) = static_cast<unsigned>(static_cast<int>(static_cast<signed char>(o[85])));
    w10m::word(p, 4) = w10m::word(o, 1104);
    w10m::word(p, 8) = w10m::word(o, 960);
    w10m::word(p, 12) = w10m::word(o, 208);
    p[16] = (w10m::dword(o, 160) & 0x00c00000ull) != 0 ? 1 : 0;
    return count + 17;
}
