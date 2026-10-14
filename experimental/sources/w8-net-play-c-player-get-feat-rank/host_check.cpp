#include "player_get_feat_rank.cpp"
#include <cstring>
#include <cstdint>
alignas(8) unsigned char player[5 * 23984];
alignas(8) unsigned char gSkillTree[40 + 5 * 3592 + 64];
static void set_feat(int row, int i, long long id) {
    memcpy(gSkillTree + 40 + row * 3592 + i * 56, &id, 8);
}
static int ref(int idx, long long feat) {
    if (idx < 0 || idx >= 5) return 0;
    int row; memcpy(&row, player + idx * 23984 + 80, 4);
    for (int i = 0; i < 64; ++i) {
        long long id; memcpy(&id, gSkillTree + 40 + row * 3592 + i * 56, 8);
        if (id == feat) { signed char b = (signed char)player[idx * 23984 + 243 + i]; return b < 0 ? 0 : b; }
    }
    return 0;
}
int main() {
    for (int r = 0; r < 5; ++r)
        for (int i = 0; i < 64; ++i) set_feat(r, i, 1000000000000LL * (r + 1) + i * 7 + 1);
    for (int c = 0; c < 5; ++c) {
        int row = (c * 3 + 1) % 5;
        memcpy(player + c * 23984 + 80, &row, 4);
        for (int i = 0; i < 64; ++i) player[c * 23984 + 243 + i] = (unsigned char)(i * 5 + c * 17);
    }
    int checked = 0;
    for (int c = -2; c < 8; ++c)
        for (int r = 0; r < 6; ++r)
            for (int i = 0; i < 65; ++i) {
                long long f = 1000000000000LL * (r + 1) + i * 7 + 1;
                if (playerGetFeatRank__Fix(c, f) != ref(c, f)) return 1;
                ++checked;
            }
    // ids that differ only in the high word must not match
    if (playerGetFeatRank__Fix(0, 1000000000000LL + 1 + (1LL << 40)) != 0) return 2;
    // first of two equal ids wins; negative rank byte reports 0
    set_feat(1, 9, 1000000000000LL * 2 + 1);
    player[0 * 23984 + 243 + 0] = 0x80;
    memset(player + 80, 0, 4);
    int row = 1; memcpy(player + 80, &row, 4);
    if (playerGetFeatRank__Fix(0, 1000000000000LL * 2 + 1) != 0) return 3;
    player[243 + 0] = 9;
    if (playerGetFeatRank__Fix(0, 1000000000000LL * 2 + 1) != 9) return 4;
    // last entry
    if (playerGetFeatRank__Fix(0, 1000000000000LL * 2 + 63 * 7 + 1) != player[243 + 63]) return 5;
    return checked > 0 ? 0 : 6;
}
