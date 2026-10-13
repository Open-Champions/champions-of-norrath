#include "../w10-actor-b-common/fields.hpp"
// Observed msg_poke__9AutoChesti at 0x21f6d8, 448 bytes. The second argument is a player
// index. The word at +232 is set first to frameNm, or, when the pointer word at +244 is
// nonzero, to frameNm plus the signed word at +4 of the record it addresses divided by 3
// (truncating). Then, by the word at +216:
//  zero (not opened yet): a sound is started from the lump "fx.lmp" by the signed byte at
//   +240 (0 "door_wdchst_open.anm", 1 "door_mtlchst_open.anm", 2 "door_stnchst_open.anm",
//   any other value no sound), each with soundPlaySequence(sound, object, 1.0); then the
//   AnimationState at +152 gets animAdd(+152, word at +244, 516); +216 becomes 1; the
//   object is added to the run list; the word at +224 gets the word at +232; and when
//   the signed halfword at +220 is not positive, bit 0x100000 of the flag word at +12
//   is cleared.
//  nonzero (already opened): the 64-bit value at +200 becomes 1; "game_pu_coin.anm" from
//   "fx.lmp" is played the same way; hudGivePlayerGold(player, the three words at
//   +16..+27 copied, the signed halfword at +220) is called, and the halfword at +220
//   becomes zero.
// Inferred: +220 is a gold amount, +244 an open-animation header, +240 the chest kind.
// Unknown: what +224, +232 and +200 mean; the function's result is not set (void).
extern "C" {
extern int frameNm;
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sound, void* object, float volume);
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flags);
void objectAddToRunList__FP10GameObject(void* object);
struct __attribute__((may_alias)) GoldPoint {
    unsigned x, y, z;
};
void hudGivePlayerGold__FiG6Point3i(int player, const GoldPoint* point, int amount);
}

namespace {
typedef unsigned long long DoubleWord __attribute__((may_alias));
void play(void* self, char* name) {
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), name), self, 1.0f);
}
}

extern "C" void msg_poke__9AutoChesti(void* self, int player) {
    using namespace actor_b;
    field<SWord>(self, 232) = frameNm;
    if (field<Word>(self, 244) != 0) {
        field<SWord>(self, 232) = frameNm + field<SWord>(link(self, 244), 4) / 3;
    }
    if (field<Word>(self, 216) == 0) {
        const int kind = field<SByte>(self, 240);
        if (kind == 1) play(self, const_cast<char*>("door_mtlchst_open.anm"));
        else if (kind == 0) play(self, const_cast<char*>("door_wdchst_open.anm"));
        else if (kind == 2) play(self, const_cast<char*>("door_stnchst_open.anm"));
        animAdd__FP14AnimationStateP15AnimationHeaderi(static_cast<unsigned char*>(self) + 152, link(self, 244), 516);
        field<Word>(self, 216) = 1;
        objectAddToRunList__FP10GameObject(self);
        field<Word>(self, 224) = field<Word>(self, 232);
        if (field<SHalf>(self, 220) <= 0) field<Word>(self, 12) &= 0xffefffffu;
    } else {
        field<DoubleWord>(self, 200) = 1;
        play(self, const_cast<char*>("game_pu_coin.anm"));
        const GoldPoint point = *reinterpret_cast<const GoldPoint*>(static_cast<unsigned char*>(self) + 16);
        hudGivePlayerGold__FiG6Point3i(player, &point, field<SHalf>(self, 220));
        field<SHalf>(self, 220) = 0;
    }
}
