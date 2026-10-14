// Observed inventoryAdjustSkill__Ficx at 0x001aeb00, 376 bytes (mangled
// inventoryAdjustSkill__Ficx: int, char, 64-bit integer).
// Arguments: a player index, a kind byte and a 64-bit value `x`. Player records
// are 23984 bytes apart at `player` (0x00408600); the signed word at record+236
// is a pool of points to spend (name inferred). Nothing happens unless it is
// positive.
//   kind == 0: `x` is passed whole (64 bits) to playerGetFeatRank__Fix. If the
//     rank is below 20 the pool is decremented, givePlayerFeat__Fxii(x, player,
//     -1) is called, and the rank is read again and stored one higher with
//     playerSetFeatRank__Fixi.
//   kind != 0: `x` is truncated to a 32-bit value (a skill index). If
//     playerGetSkillRank__Fii is below 20 the pool is decremented, then the
//     rank is read again. If it is positive the rank is read once more and set
//     one higher with playerSetSkillRank__Fiii. If it is zero or less: when the
//     word at offset 8 of the structure pointed to by skills[index] equals 3 the
//     rank is set to 1, otherwise givePlayerSkill__Fii(index, player) is called.
// Whether the kind byte means feat vs skill is inferred from which callees it
// reaches. The meaning of the value 3 at offset 8 is not established. Indices
// are not range checked. The callees and objects are not defined here.
extern "C" {
extern unsigned char player[];
extern unsigned char* skills[];
int playerGetFeatRank__Fix(int, long long);
void playerSetFeatRank__Fixi(int, long long, int);
int playerGetSkillRank__Fii(int, int);
void playerSetSkillRank__Fiii(int, int, int);
void givePlayerFeat__Fxii(long long, int, int);
void givePlayerSkill__Fii(int, int);
}

extern "C" void inventoryAdjustSkill__Ficx(int playerIndex, char kind, long long x) {
    int* points = reinterpret_cast<int*>(player + playerIndex * 23984 + 236);
    if (*points <= 0) return;
    if (kind == 0) {
        if (playerGetFeatRank__Fix(playerIndex, x) < 20) {
            *points = *points - 1;
            givePlayerFeat__Fxii(x, playerIndex, -1);
            playerSetFeatRank__Fixi(playerIndex, x, playerGetFeatRank__Fix(playerIndex, x) + 1);
        }
        return;
    }
    int skill = static_cast<int>(x);
    if (playerGetSkillRank__Fii(playerIndex, skill) < 20) {
        *points = *points - 1;
        if (playerGetSkillRank__Fii(playerIndex, skill) > 0) {
            playerSetSkillRank__Fiii(playerIndex, skill, playerGetSkillRank__Fii(playerIndex, skill) + 1);
        } else if (*reinterpret_cast<int*>(skills[skill] + 8) == 3) {
            playerSetSkillRank__Fiii(playerIndex, skill, 1);
        } else {
            givePlayerSkill__Fii(skill, playerIndex);
        }
    }
}
