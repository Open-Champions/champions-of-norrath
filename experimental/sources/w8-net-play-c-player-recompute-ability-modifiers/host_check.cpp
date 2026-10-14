#include <cstring>
#include <cstdint>
#include <cstdio>
#include <sys/mman.h>
#include "player_recompute_ability_modifiers.cpp"
// Built for x86_64 with -Wl,-pagezero_size,0x10000: the record pointer words are
// 32 bits on the target, so objects and items live below 4 GiB (mapped at 0x20000000).
alignas(8) unsigned char player[5 * 23984];
int nmPlayers;
int cheatMode;
static int rank_calls;
static int rank_args[64]; static long long rank_feats[64];
static int paper_calls; static int paper_args[8];
extern "C" int playerGetFeatRank__Fix(int index, long long feat) {
    rank_args[rank_calls] = index; rank_feats[rank_calls] = feat; ++rank_calls;
    return (index * 7 + (int)(feat & 0xff) / 16 + rank_calls) % 9;
}
extern "C" int recomputePaperDollModifiers__Fi(int index) {
    paper_args[paper_calls++] = index; return 0x12340 + index * 3 - 7;
}
static uint32_t seed = 12345;
static uint32_t rnd() { seed = seed * 1664525u + 1013904223u; return seed >> 8; }
static unsigned char* heap; static unsigned heap_used;
static unsigned lo_alloc(unsigned n) { unsigned a = 0x20000000u + heap_used; heap_used += (n + 15) & ~15u; return a; }
static unsigned make_item() {
    unsigned a = lo_alloc(128); unsigned char* b = (unsigned char*)(unsigned long)a;
    memset(b, 0, 128);
    static const short ids[] = {7, 8, 9, 10, 30, 11, 7, 0};
    for (int j = 0; j < 4; ++j) {
        short id = ids[rnd() % 8]; short v = (short)((int)(rnd() % 61) - 30);
        memcpy(b + 36 + 8 * j, &id, 2); memcpy(b + 38 + 8 * j, &v, 2);
    }
    return a;
}
static unsigned make_obj(int idx) {
    unsigned a = lo_alloc(1300); unsigned char* b = (unsigned char*)(unsigned long)a;
    memset(b, 0xa5, 1300); memcpy(b + 960, &idx, 4);
    return a;
}
// independent model working on a copy of the memory
static int mod(const unsigned char* item, int id) {
    int v = 0, found = 0;
    for (int j = 0; j < 4 && !found; ++j) {
        short sid; memcpy(&sid, item + 36 + 8 * j, 2);
        if (sid == id) { short sv; memcpy(&sv, item + 38 + 8 * j, 2); v = sv; found = 1; }
    }
    return v;
}
template <class T> static T get(const unsigned char* b, int o) { T t; memcpy(&t, b + o, sizeof t); return t; }
template <class T> static void put(unsigned char* b, int o, T t) { memcpy(b + o, &t, sizeof t); }
static int model_calls;
static int model_rank(int index, long long feat) { return (index * 7 + (int)(feat & 0xff) / 16 + (++model_calls)) % 9; }
static void model(unsigned char* rec, unsigned char* objs[], int p) {
    unsigned char* R = rec;
    unsigned objp = get<unsigned>(R, 21872);
    unsigned char* O = (unsigned char*)(unsigned long)objp;
    for (int i = 0; i < 4; ++i) put<int>(R, 144 + 4 * i, get<int>(R, 120 + 4 * i));
    put<float>(R, 224, 0.0f);
    int st[4] = {get<int>(R, 144), get<int>(R, 148), get<int>(R, 152), get<int>(R, 156)};
    float acc = 0.0f;
    for (int k = 0; k < 16; ++k) {
        unsigned ip = get<unsigned>(R, 21896 + 4 * k);
        if (!ip) continue;
        const unsigned char* it = (const unsigned char*)(unsigned long)ip;
        for (int s = 0; s < 4; ++s) st[s] += mod(it, 7 + s);
        acc += (float)mod(it, 30) * 0.01f;
    }
    for (int s = 0; s < 4; ++s) put<int>(R, 144 + 4 * s, st[s]);
    put<float>(R, 224, acc);
    for (int j = 0; j < 4; ++j) {
        int v = st[j] - ((j & 1) ? 5 : 30);
        signed char b = (signed char)v; if (b < 0) b = 0; O[1124 + j] = (unsigned char)b;
    }
    int cls = get<int>(R, 80);
    static const int kb[5] = {18, 16, 14, 14, 12}; static const float kf[5] = {4.0f, 7.0f, 5.0f, 6.0f, 10.0f};
    if (cls >= 0 && cls < 5) { R[192] = (unsigned char)kb[cls]; put<float>(R, 200, kf[cls]); }
    int lvl = (signed char)R[89];
    float hp = (float)((signed char)R[192] * (lvl + 1));
    int n = model_rank(p, 128);
    hp = hp + (float)(3 * n); hp = hp + (float)(st[3] / 2);
    put<float>(R, 176, hp);
    static const int mt[5] = {6, 10, 8, 9, 14};
    int m = (cls >= 0 && cls < 5) ? mt[cls] : 0;
    float mp = (float)(m * lvl);
    n = model_rank(get<int>(O, 960), 256);
    mp = mp + (float)(5 * n); mp = mp + (float)(st[1] / 2);
    put<float>(R, 200, mp);
    float cap = (float)(st[0] + 125);
    n = model_rank(get<int>(O, 960), 64);
    cap = cap + (float)n * 15.0f;
    if (cheatMode) cap = cap + 5000.0f;
    if (9999.0f < cap) cap = 9999.0f;
    put<float>(R, 312, cap);
    put<short>(O, 232, (short)(0x12340 + p * 3 - 7));
    if (mp < get<float>(R, 196)) put<float>(R, 196, mp);
    if (hp < get<float>(R, 172)) put<float>(R, 172, hp);
    (void)objs;
}
int main() {
    heap = (unsigned char*)mmap((void*)0x20000000, 1 << 20, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON | MAP_FIXED, -1, 0);
    if ((unsigned long)heap != 0x20000000) return 100;
    int checked = 0;
    for (int round = 0; round < 60; ++round) {
        heap_used = 0;
        int count = round % 4;  // 0..3 players walked
        for (int p = 0; p < 5; ++p) {
            unsigned char* R = player + p * 23984;
            for (int i = 0; i < 23984; ++i) R[i] = (unsigned char)rnd();
            int cls = (round / 4 + p) % 8 - 1;  // -1 .. 6: includes out-of-range classes
            put<int>(R, 80, cls);
            put<signed char>(R, 89, (signed char)((rnd() % 30) + 1));
            for (int i = 0; i < 4; ++i) put<int>(R, 120 + 4 * i, (int)(rnd() % 60) - 10);
            for (int k = 0; k < 16; ++k) put<unsigned>(R, 21896 + 4 * k, (rnd() % 3) ? make_item() : 0u);
            put<unsigned>(R, 21872, (p == 1 && round % 3 == 0) ? 0u : make_obj(p + 10 * round % 7));
            put<float>(R, 172, 1e6f); put<float>(R, 196, (round % 2) ? 1e6f : -1.0f);
            if (round % 5 == 0) put<float>(R, 200, 100.0f);
        }
        nmPlayers = count; cheatMode = round & 1;
        // snapshot memory and objects for the model
        static unsigned char copy[sizeof player];
        memcpy(copy, player, sizeof player);
        static unsigned char objs_backup[1 << 16]; memcpy(objs_backup, heap, heap_used);
        rank_calls = 0; paper_calls = 0;
        playerRecomputeAbilityModifiers__Fv();
        // run the model on the pre-call copies; objects must be restored first
        static unsigned char objs_after[1 << 16]; memcpy(objs_after, heap, heap_used);
        memcpy(heap, objs_backup, heap_used);
        model_calls = 0;
        for (int p = 0; p < count; ++p)
            if (get<unsigned>(copy + p * 23984, 21872)) model(copy + p * 23984, 0, p);
        if (memcmp(copy, player, sizeof player)) { printf("record mismatch round %d\n", round); return 1; }
        if (memcmp(heap, objs_after, heap_used)) { printf("object mismatch round %d\n", round); return 2; }
        if (rank_calls != model_calls) return 3;
        ++checked;
    }
    // call order for one player: ranks use p, then the object index, then the object index
    nmPlayers = 1; rank_calls = 0;
    unsigned char* R = player;
    put<unsigned>(R, 21872, make_obj(77));
    playerRecomputeAbilityModifiers__Fv();
    if (rank_calls != 3 || rank_args[0] != 0 || rank_feats[0] != 128 || rank_args[1] != 77 || rank_feats[1] != 256 ||
        rank_args[2] != 77 || rank_feats[2] != 64) return 4;
    // a player with no object pointer is skipped completely
    memset(player, 0, sizeof player); nmPlayers = 1; rank_calls = 0; paper_calls = 0;
    put<int>(player, 120, 5);
    playerRecomputeAbilityModifiers__Fv();
    if (rank_calls || paper_calls || get<int>(player, 144)) return 5;
    return checked == 60 ? 0 : 6;
}
