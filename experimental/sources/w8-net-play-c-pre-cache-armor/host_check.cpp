#include <cstdio>
#include <cstring>
#include <cstdint>
#include <string>
#include <sys/mman.h>
#define strlen unit_strlen
#include "pre_cache_armor.cpp"
#undef strlen
// x86_64 build with -Wl,-pagezero_size,0x10000: item, entry and guard pointers are
// 32-bit words on the target and live at 0x20000000 here.
alignas(8) unsigned char player[2 * 23984];
signed char interfaceOn;
unsigned levelFlags;
int gamePlayersPresent;
alignas(8) unsigned char norrath_player_armor_cache_view_7cb770[3520];
alignas(8) unsigned char _4item$globalItem[32 * 16];

extern "C" unsigned unit_strlen(const char* s) { return (unsigned)strlen(s); }
static std::string trace;
static unsigned char* heap; static unsigned heap_used;
static unsigned lo(unsigned n) { unsigned a = 0x20000000u + heap_used; heap_used += (n + 15) & ~15u; return a; }
static unsigned char* lp(unsigned a) { return (unsigned char*)(unsigned long)a; }
extern "C" void getPicName__4itemPciPQ24item11description(char* b, int cls, void* item) {
    char t[64]; snprintf(t, sizeof t, "p%03d_%d.tga", (int)(((unsigned long)item - 0x20000000u) / 16), cls);
    strcpy(b, t); trace += "pic(" + std::string(t) + ") ";
}
static unsigned model_for(const std::string& n) { unsigned h = 7; for (char c : n) h = h * 31 + c; return (h % 5 == 0) ? 0 : h; }
extern "C" void cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(char* name, void* el, unsigned* m, unsigned* t) {
    long off = (unsigned char*)el - norrath_player_armor_cache_view_7cb770;
    char b[128]; snprintf(b, sizeof b, "cache(%s,%ld) ", name, off); trace += b;
    std::string s = name; *m = model_for(s); *t = model_for(s + "t");
}
extern "C" int getProxyMeshId__4itemPQ24item11description(void* item) {
    int id = (int)(((unsigned long)item - 0x20000000u) / 16) % 20;
    char b[64]; snprintf(b, sizeof b, "proxy(%d) ", id); trace += b; return id;
}
static uint32_t seed = 99;
static uint32_t rnd() { seed = seed * 1664525u + 1013904223u; return seed >> 8; }
static void put32(unsigned char* b, int o, unsigned v) { memcpy(b + o, &v, 4); }

static int fails;

static int model_run(std::string& log, unsigned char* self_) {
    unsigned char* rec = player + *(int*)(self_ + 960) * 23984;
    unsigned m = 0, t = 0; int res = 0;
    static const int nmap[16] = {-1, -1, -1, 9, 0, -1, -1, -1, -1, 2, 1, 3, 4, 5, -1, -1};
    for (int s = 0; s < 16; ++s) {
        unsigned it; memcpy(&it, rec + 21896 + 4 * s, 4);
        if (!it) continue;
        signed char sel = (signed char)rec[23572];
        unsigned g; memcpy(&g, rec + 21960 + 4 * sel, 4);
        if (g && s == 3 && (signed char)lp(*(unsigned*)lp(g))[30] == 4) continue;
        unsigned entry = *(unsigned*)lp(it);
        int kind = (signed char)lp(entry)[28];
        if (kind == 8 || kind == 5 || kind == 6 || kind == 7) continue;
        if (!interfaceOn && (levelFlags & 0x10000000) && (kind == 2 || kind == 0 || kind == 3)) continue;
        int n = nmap[s];
        if (n < 0) {
            int id = (int)(((unsigned long)lp(it) - 0x20000000u) / 16) % 20;
            log += "proxy(" + std::to_string(id) + ") "; log += "proxy(" + std::to_string(id) + ") ";
            memcpy(&m, _4item$globalItem + id * 16 + 8, 4); memcpy(&t, _4item$globalItem + id * 16 + 12, 4);
        } else if (gamePlayersPresent == 0) {
            if (s == 3) m = t = 0;
        } else if (*(unsigned*)(lp(entry) + 48)) {
            char b[64]; int cls; memcpy(&cls, rec + 84, 4);
            snprintf(b, sizeof b, "p%03d_%d", (int)(((unsigned long)lp(it) - 0x20000000u) / 16), cls);
            log += "pic(" + std::string(b) + ".tga) ";
            char c[160]; snprintf(c, sizeof c, "cache(%s,%d) ", b, n * 352 + 88 * *(int*)(self_ + 960)); log += c;
            m = model_for(b); t = model_for(std::string(b) + "t");
        } else if (s == 3) {
            short idx; memcpy(&idx, lp(entry) + 52, 2);
            memcpy(&m, _4item$globalItem + idx * 16 + 8, 4); memcpy(&t, _4item$globalItem + idx * 16 + 12, 4);
        }
        if (!m || !t) res = 1;
    }
    return res;
}

int main() {
    heap = (unsigned char*)mmap((void*)0x20000000, 1 << 20, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON | MAP_FIXED, -1, 0);
    if ((unsigned long)heap != 0x20000000) return 100;
    alignas(8) static unsigned char selfbuf[2048];
    int ones = 0, zeros = 0;
    for (int round = 0; round < 4000; ++round) {
        heap_used = 0; trace.clear();
        memset(player, 0, sizeof player); memset(selfbuf, 0, sizeof selfbuf);
        for (int i = 0; i < 32; ++i) { put32(_4item$globalItem, i * 16 + 8, (rnd() % 4) ? 0x1000 + i : 0); put32(_4item$globalItem, i * 16 + 12, (rnd() % 4) ? 0x2000 + i : 0); }
        int idx = rnd() % 2; put32(selfbuf, 960, idx);
        unsigned char* rec = player + idx * 23984;
        put32(rec, 84, rnd() % 10);
        rec[23572] = (unsigned char)(rnd() % 3);
        for (int k = 0; k < 3; ++k) {
            unsigned entry = lo(64); lp(entry)[30] = (rnd() % 3 == 0) ? 4 : 1;
            unsigned holder = lo(16); put32(lp(holder), 0, entry);
            put32(rec, 21960 + 4 * k, (rnd() % 4) ? holder : 0);
        }
        for (int s = 0; s < 16; ++s) {
            if (rnd() % 5 == 0) continue;
            unsigned entry = lo(64); memset(lp(entry), 0, 64);
            static const int kinds[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 1, 4, 4};
            lp(entry)[28] = (unsigned char)kinds[rnd() % 14];
            put32(lp(entry), 48, (rnd() % 3) ? 0x77 : 0);
            short e52 = (short)(rnd() % 32); memcpy(lp(entry) + 52, &e52, 2);
            unsigned item = lo(16); put32(lp(item), 0, entry);
            put32(rec, 21896 + 4 * s, item);
        }
        interfaceOn = rnd() % 2; levelFlags = (rnd() % 2) ? 0x10000000 : 0; gamePlayersPresent = rnd() % 4 != 0;
        std::string expect;
        int want = model_run(expect, selfbuf);
        int got = preCacheArmor__6Player(selfbuf);
        (got ? ones : zeros)++;
        if (got != want || trace != expect) { printf("FAIL round %d got %d want %d\n got: %s\nwant: %s\n", round, got, want, trace.c_str(), expect.c_str()); ++fails; if (fails > 3) break; }
    }
    if (!ones || !zeros) { printf("FAIL no variety %d %d\n", ones, zeros); return 1; }
    return fails ? 1 : 0;
}
