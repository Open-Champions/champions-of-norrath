#include "give_player_experience.cpp"
#include <cstring>
alignas(8) unsigned char player[2 * 23984];
int experienceTable[51];
static int recompute, localFlag, soundCalls, msgCalls;
static int lastMsgPlayer, lastMsgTime;
static unsigned short textObj;
static char lumpBuf[1];
extern "C" {
void playerRecomputeAbilityModifiers__Fv() { ++recompute; }
int netPlayerIsLocal__Fi(int) { return localFlag; }
char* lumpFindResource__FPcT0(char* lump, char* name) {
    return (std::strcmp(lump, "fx.lmp") == 0 && std::strcmp(name, "menu_lvlup.anm") == 0) ? lumpBuf : 0;
}
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float r) {
    if (h == lumpBuf && o == 0 && r == 1.0f) ++soundCalls;
    return 0;
}
unsigned short* textGetString__FPc(char* key) { return std::strcmp(key, "LevelUpMessage") == 0 ? &textObj : 0; }
void hudSetInfoMessage__FPUsiic(unsigned short* t, int time, int p, char f) {
    if (t == &textObj && f == 0) { ++msgCalls; lastMsgPlayer = p; lastMsgTime = time; }
}
}
int main() {
    for (int i = 0; i < 51; ++i) experienceTable[i] = (i + 1) * 100;
    unsigned char* r = player + 23984;
    float* f = reinterpret_cast<float*>(r);
    *(int*)(r + 92) = 0;
    r[89] = 0;
    // below threshold: no level up
    givePlayerExperience__Fii(1, 99);
    if (r[89] != 0 || recompute != 0 || *(int*)(r + 92) != 99) return 1;
    // 99+1 reaches 100 for level 0; crossing 200 needs another 100 more
    f[43] = 2.0f; f[44] = 7.0f; f[49] = 1.0f; f[50] = 9.0f;
    localFlag = 1;
    givePlayerExperience__Fii(1, 1);
    if (r[89] != 1 || *(int*)(r + 236) != 1 || r[240] != 3 || recompute != 1) return 2;
    if (f[43] != 7.0f || f[49] != 9.0f) return 3;
    if (soundCalls != 1 || msgCalls != 1 || lastMsgPlayer != 1 || lastMsgTime != 180) return 4;
    // large grant: from level 1 (needs 200) to total 2000 crosses until table[n] > 2000 (level 20)
    localFlag = 0;
    givePlayerExperience__Fii(1, 1900);
    if (r[89] != 20 || recompute != 20) return 5;
    // bonus: levels 1..5 -> +1 (5), 6..15 -> +2 (20), 0 -> +1, 16..19 -> +3 (12)
    if (*(int*)(r + 236) != 1 + 5 + 20 + 12 || r[240] != 60) return 6;
    if (soundCalls != 1 || msgCalls != 20) return 7;
    // zero or negative float: untouched
    f[43] = 0.0f; f[44] = 5.0f;
    givePlayerExperience__Fii(1, 100000 - 2000);
    if (f[43] != 0.0f) return 8;
    // a high level uses the +5 band
    r[89] = 40; *(int*)(r + 92) = 0; *(int*)(r + 236) = 0;
    experienceTable[40] = 10;
    givePlayerExperience__Fii(1, 10);
    if (*(int*)(r + 236) != 5 || r[89] != 41) return 9;
    return 0;
}
