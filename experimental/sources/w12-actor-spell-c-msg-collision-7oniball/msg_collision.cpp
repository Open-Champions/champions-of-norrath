// Observed msg_collision__7OniBallP10GameObject at 0x0021aaf0, 364 bytes. O is the ball (first argument)
// and X the colliding object (second argument). Steps in order:
//  1. Nothing happens when the word at X+12 has bit 0x20000 set or has bit 0x40 clear.
//  2. With p the word at X+960: playerGetSkillRank__Fii(p, 59) is called; a result of zero or less ends it.
//  3. The word at 0x0034b918 + 4p (blockButton) is ANDed with the word at pad + 88p + 8; a zero result ends it.
//  4. The word at X+252 becomes frameNm. With R = playerGetSkillRank__Fii(p, 59) again, the amount
//     A = SkillLifeTap::getAmount(skills[59], R) (getAmount__12SkillLifeTapi, called on the pointer word at skills + 236).
//  5. With P the player record at player + 23984p, by the signed byte at O+116:
//       0: float(A) is added to the float at P+180; if the float at P+176 is below P+172 plus that sum, the
//          float at P+180 becomes P+176 minus P+172; the word at X+928 becomes 30;
//       1: the same on P+204 with limits P+200 and P+196, and the word at X+936 becomes 30;
//       any other value: nothing more.
//  6. The word at O+84 becomes frameNm (on every path that passed step 3).
// Inferred: this is a pickup effect (the skill id 59 is the one the name SkillLifeTap belongs to). Unknown: what
// the P offsets and the byte at O+116 stand for, and what the X words at +252, +928 and +936 hold.
// Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern int frameNm;
extern unsigned skills[];
extern unsigned blockButton[];
extern unsigned char pad[];
extern unsigned char player[];
int playerGetSkillRank__Fii(int player, int skill);
int getAmount__12SkillLifeTapi(void* self, int rank);

void msg_collision__7OniBallP10GameObject(void* self, void* other) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const x = static_cast<unsigned char*>(other);
    const unsigned flags = word(x, 12);
    if ((flags & 0x20000u) != 0u) return;
    if ((flags & 0x40u) == 0u) return;
    const int p = sword(x, 960);
    if (playerGetSkillRank__Fii(p, 59) <= 0) return;
    if ((blockButton[p] & word(pad + p * 88, 8)) == 0u) return;
    word(x, 252) = static_cast<unsigned>(frameNm);
    unsigned char* const record = player + p * 23984;
    const int amount = getAmount__12SkillLifeTapi(from_word(skills[59]), playerGetSkillRank__Fii(p, 59));
    const signed char kind = static_cast<signed char>(o[116]);
    if (kind == 0) {
        const float sum = real(record, 180) + static_cast<float>(amount);
        real(record, 180) = sum;
        if (real(record, 176) < real(record, 172) + sum) real(record, 180) = real(record, 176) - real(record, 172);
        word(x, 928) = 30u;
    } else if (kind == 1) {
        const float sum = real(record, 204) + static_cast<float>(amount);
        real(record, 204) = sum;
        if (real(record, 200) < real(record, 196) + sum) real(record, 204) = real(record, 200) - real(record, 196);
        word(x, 936) = 30u;
    }
    word(o, 84) = static_cast<unsigned>(frameNm);
}
}
