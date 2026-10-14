#include <cstring>
extern "C" {
unsigned char player[23984 * 4 + 512];
unsigned char* skills[8];
void inventoryAdjustSkill__Ficx(int, char, long long);
}

static long long log_[64];
static int n;
static int featRank, skillRank;
static int skillRankAfterGive;

extern "C" int playerGetFeatRank__Fix(int p, long long x) { log_[n++] = 1; log_[n++] = p; log_[n++] = x; return featRank; }
extern "C" void playerSetFeatRank__Fixi(int p, long long x, int r) { log_[n++] = 2; log_[n++] = p; log_[n++] = x; log_[n++] = r; }
extern "C" int playerGetSkillRank__Fii(int p, int s) { log_[n++] = 3; log_[n++] = p; log_[n++] = s; return skillRank; }
extern "C" void playerSetSkillRank__Fiii(int p, int s, int r) { log_[n++] = 4; log_[n++] = p; log_[n++] = s; log_[n++] = r; }
extern "C" void givePlayerFeat__Fxii(long long x, int p, int v) { log_[n++] = 5; log_[n++] = x; log_[n++] = p; log_[n++] = v; }
extern "C" void givePlayerSkill__Fii(int s, int p) { log_[n++] = 6; log_[n++] = s; log_[n++] = p; }

static int& pool(int p) { return *reinterpret_cast<int*>(player + p * 23984 + 236); }
static unsigned char skillA[16], skillB[16];

int main() {
    int three = 3, zero = 0;
    std::memcpy(skillA + 8, &three, 4);
    std::memcpy(skillB + 8, &zero, 4);
    skills[5] = skillA; skills[6] = skillB;
    const long long big = 0x100000005LL;

    // no points: nothing at all
    n = 0; pool(2) = 0; featRank = 0; skillRank = 0;
    inventoryAdjustSkill__Ficx(2, 0, 7);
    inventoryAdjustSkill__Ficx(2, 1, 5);
    if (n != 0 || pool(2) != 0) return 1;
    pool(2) = -3; inventoryAdjustSkill__Ficx(2, 0, 7);
    if (n != 0 || pool(2) != -3) return 2;

    // feat: rank below 20 -> pool--, give, then set rank+1 (x passed whole)
    n = 0; pool(1) = 4; featRank = 7;
    inventoryAdjustSkill__Ficx(1, 0, big);
    long long expectFeat[] = {1, 1, big, 5, big, 1, -1, 1, 1, big, 2, 1, big, 8};
    if (n != 14 || pool(1) != 3) return 3;
    for (int i = 0; i < 14; ++i) if (log_[i] != expectFeat[i]) return 4;
    // feat at rank 20: no change
    n = 0; pool(1) = 4; featRank = 20;
    inventoryAdjustSkill__Ficx(1, 0, 9);
    if (n != 3 || pool(1) != 4) return 5;

    // skill with rank > 0: set rank+1, x truncated to 32 bits
    n = 0; pool(3) = 2; skillRank = 4;
    inventoryAdjustSkill__Ficx(3, 1, big);
    long long expectSkill[] = {3, 3, 5, 3, 3, 5, 3, 3, 5, 4, 3, 5, 5};
    if (n != 13 || pool(3) != 1) return 6;
    for (int i = 0; i < 13; ++i) if (log_[i] != expectSkill[i]) return 7;
    // skill at rank 20: only the one query
    n = 0; pool(3) = 2; skillRank = 20;
    inventoryAdjustSkill__Ficx(3, 1, 5);
    if (n != 3 || pool(3) != 2) return 8;
    // rank 0, skills[5] has 3 at +8: two rank queries, then set rank 1
    n = 0; pool(0) = 1; skillRank = 0;
    inventoryAdjustSkill__Ficx(0, 9, 5);
    long long expectSet[] = {3, 0, 5, 3, 0, 5, 4, 0, 5, 1};
    if (n != 10 || pool(0) != 0) return 9;
    for (int i = 0; i < 10; ++i) if (log_[i] != expectSet[i]) return 10;
    // rank below 0, skills[6] has another value: give the skill as (skill, player)
    n = 0; pool(0) = 1; skillRank = -1;
    inventoryAdjustSkill__Ficx(0, 1, 6);
    long long expectGive[] = {3, 0, 6, 3, 0, 6, 6, 6, 0};
    if (n != 9 || pool(0) != 0) return 11;
    for (int i = 0; i < 9; ++i) if (log_[i] != expectGive[i]) return 12;
    return 0;
}
