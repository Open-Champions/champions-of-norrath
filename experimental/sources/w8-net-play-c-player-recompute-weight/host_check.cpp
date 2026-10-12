#include <cstring>
#include <cstdint>
#include <sys/mman.h>
#include "player_recompute_weight.cpp"
// The item pointer word is 32 bits on the target, so the host check maps its
// item-kind bytes below 4 GiB (built for x86_64 with -Wl,-pagezero_size,0x10000 so the low mapping is allowed).
alignas(8) unsigned char player[5 * 23984];
static int rank_calls; static long long rank_feat; static int rank_ret; static int rank_index;
static float weights[5][3][64];
extern "C" int playerGetFeatRank__Fix(int index, long long feat) {
    ++rank_calls; rank_feat = feat; rank_index = index; return rank_ret;
}
extern "C" float getWeight__4itemPQ24item11description(void* item) {
    long off = (unsigned char*)item - player;
    int rec = (int)(off / 23984); off -= rec * 23984;
    for (int l = 0; l < 3; ++l)
        if (off >= 328 + l * 4616 && off < 328 + l * 4616 + 64 * 72)
            return weights[rec][l][(off - 328 - l * 4616) / 72];
    return -1e9f;
}
static unsigned char* kinds;  // 256 bytes; slot k at +k*32, byte 28 is the kind
static float reference(int rec, int rank) {
    float total = 0.0f;
    for (int l = 0; l < 3; ++l) {
        int n; memcpy(&n, player + rec * 23984 + 4936 + l * 4616, 4);
        for (int i = 0; i < n; ++i) {
            unsigned w; memcpy(&w, player + rec * 23984 + 328 + l * 4616 + i * 72, 4);
            signed char k = *(signed char*)(unsigned long)(w + 28);
            float scale = (k == 8 && rank) ? 0.5f : 1.0f;
            total += weights[rec][l][i] * scale;
        }
    }
    return total;
}
int main() {
    kinds = (unsigned char*)mmap((void*)0x20000000, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON | MAP_FIXED, -1, 0);
    if ((unsigned long)kinds != 0x20000000) return 100;
    for (int k = 0; k < 64; ++k) kinds[k * 32 + 28] = (k % 4 == 1) ? 8 : (k % 4);
    for (int rec = 0; rec < 5; ++rec) {
        memset(player + rec * 23984, 0xee, 23984);
        int counts[3] = {rec * 7 % 11, 64 - rec, rec == 2 ? 0 : 5 + rec};
        for (int l = 0; l < 3; ++l) {
            memcpy(player + rec * 23984 + 4936 + l * 4616, &counts[l], 4);
            for (int i = 0; i < 64; ++i) {
                unsigned w = 0x20000000u + ((i * 5 + l + rec) % 64) * 32;
                memcpy(player + rec * 23984 + 328 + l * 4616 + i * 72, &w, 4);
                weights[rec][l][i] = 0.1f * (i + 1) + 3.7f * l + 0.013f * rec;
            }
        }
    }
    for (int rank = 0; rank < 2; ++rank)
        for (int rec = 0; rec < 5; ++rec) {
            rank_ret = rank; rank_calls = 0;
            // pre-set the total to garbage: it must be overwritten
            float junk = 1234.5f; memcpy(player + rec * 23984 + 308, &junk, 4);
            unsigned char before[23984]; memcpy(before, player + rec * 23984, 23984);
            playerRecomputeWeight__Fi(rec);
            float got; memcpy(&got, player + rec * 23984 + 308, 4);
            if (got != reference(rec, rank)) return 1;
            if (rank_calls && (rank_feat != 0x10000LL || rank_index != rec)) return 2;
            memcpy(before + 308, &got, 4);
            if (memcmp(before, player + rec * 23984, 23984)) return 3;
        }
    // the feat check only happens for kind 8: count calls for record 0 with rank 0
    rank_ret = 0; rank_calls = 0;
    playerRecomputeWeight__Fi(0);
    int expect_calls = 0;
    for (int l = 0; l < 3; ++l) {
        int n; memcpy(&n, player + 4936 + l * 4616, 4);
        for (int i = 0; i < n; ++i) {
            unsigned w; memcpy(&w, player + 328 + l * 4616 + i * 72, 4);
            if (*(signed char*)(unsigned long)(w + 28) == 8) ++expect_calls;
        }
    }
    if (rank_calls != expect_calls || expect_calls == 0) return 4;
    return 0;
}
