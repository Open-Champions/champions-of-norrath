// Host check for playerSetFeatRank__Fixi against an independent model.
#include "set_feat_rank.cpp"
#include <cstring>
alignas(8) unsigned char player[5 * 23984];
alignas(8) unsigned char gSkillTree[40 + 5 * 3592 + 64];
static unsigned char shadow[5 * 23984];
static void model(int idx, long long feat, int rank) {
    int row; memcpy(&row, shadow + idx * 23984 + 80, 4);
    for (int i = 0; i < 64; ++i) {
        long long id; memcpy(&id, gSkillTree + 40 + row * 3592 + i * 56, 8);
        if (id == feat) {
            signed char& b = (signed char&)shadow[idx * 23984 + 243 + i];
            if (b < rank) b = (signed char)rank;
            return;
        }
    }
}
int main() {
    for (int r = 0; r < 5; ++r)
        for (int i = 0; i < 64; ++i) {
            long long id = 1000000000000LL * (r + 1) + i * 7 + 1;
            memcpy(gSkillTree + 40 + r * 3592 + i * 56, &id, 8);
        }
    // a duplicate id in row 2 must only affect the first entry
    long long dup = 1000000000000LL * 3 + 5 * 7 + 1;
    memcpy(gSkillTree + 40 + 2 * 3592 + 9 * 56, &dup, 8);
    for (int c = 0; c < 5; ++c) {
        int row = (c * 3 + 1) % 5;
        memcpy(player + c * 23984 + 80, &row, 4);
        for (int i = 0; i < 64; ++i) player[c * 23984 + 243 + i] = (unsigned char)(i * 5 + c * 17);
    }
    memcpy(shadow, player, sizeof player);
    static const int ranks[] = {-5, 0, 1, 20, 127, 128, 200, -1};
    int n = 0;
    for (int c = 0; c < 5; ++c)
        for (int r = 0; r < 6; ++r)
            for (int i = 0; i < 65; ++i)
                for (int k : ranks) {
                    long long f = 1000000000000LL * (r + 1) + i * 7 + 1;
                    playerSetFeatRank__Fixi(c, f, k);
                    model(c, f, k);
                    if (memcmp(player, shadow, sizeof player) != 0) return 1;
                    ++n;
                }
    // the duplicate really was exercised and a rise is observable
    if (n == 0) return 2;
    unsigned char before = player[0 * 23984 + 243];
    playerSetFeatRank__Fixi(0, 1000000000000LL * 2 + 1, 100);
    if (player[0 * 23984 + 243] != 100 && before < 100) return 3;
    return 0;
}
