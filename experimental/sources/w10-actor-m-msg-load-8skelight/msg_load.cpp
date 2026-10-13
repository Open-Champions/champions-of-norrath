#include "../w10-actor-m-common/access.hpp"
// Observed msg_load__8SkelightPc at 0x247310, 216 bytes. The base routine msg_load__8CreaturePc runs
// first and returns a byte count n. From input+n it reads, in order: a word whose low byte goes to
// object+85, then words into +1104, +960 and +208, then one byte; when that byte is nonzero the bit
// 0x00400000 is set in the 64-bit word at +160. Then initWeapon__8Skelight runs (emitted by the authored
// sources as norrath_skelight_init_weapon), and setParameters__8Creaturefffffi(object, 1, 1, 1, 1, 1, 0)
// runs while the float global worldDifficulty (0x484cac, a symbol in the image) is temporarily
// replaced by the word at object+1104 converted from int to float; the old value is restored
// afterwards. The result is n + 17. Unknown: the meaning of the fields, and why the load borrows
// worldDifficulty for the call (the +1104 word looks like a level; that is a guess).
extern "C" {
int msg_load__8CreaturePc(void* self, char* in);
void norrath_skelight_init_weapon(void* self);
void setParameters__8Creaturefffffi(void* self, float a, float b, float c, float d, float e, int flag);
extern float worldDifficulty;
}

extern "C" int msg_load__8SkelightPc(void* self, char* in) {
    unsigned char* o = static_cast<unsigned char*>(self);
    const int count = msg_load__8CreaturePc(self, in);
    unsigned char* p = reinterpret_cast<unsigned char*>(in) + count;
    o[85] = static_cast<unsigned char>(w10m::word(p, 0));
    w10m::word(o, 1104) = w10m::word(p, 4);
    w10m::word(o, 960) = w10m::word(p, 8);
    w10m::word(o, 208) = w10m::word(p, 12);
    if (static_cast<signed char>(p[16]) != 0) w10m::dword(o, 160) |= 0x00400000ull;
    norrath_skelight_init_weapon(self);
    const float saved = worldDifficulty;
    worldDifficulty = static_cast<float>(static_cast<int>(w10m::word(o, 1104)));
    setParameters__8Creaturefffffi(self, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0);
    worldDifficulty = saved;
    return count + 17;
}
