// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
#include <cstdio>
#include <cstring>
static unsigned char arena[1 << 18];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "insert.cpp"
static int& W(unsigned char* b, int off) { return *reinterpret_cast<int*>(b + off); }
static unsigned char* E(unsigned char* map, int i) { return arena + (*reinterpret_cast<unsigned*>(map + 8) - arena_base) + 104 * i; }
static int fail_add, adds, lefts, rights;
extern "C" int AddNode__t5VIMap2ZiZ17VIEESurfCacheInfoii(unsigned char* map, int parent, int left) {
    ++adds;
    if (fail_add) return -1;
    int i = W(map, 16);
    W(map, 16) = W(E(map, i), 4);
    W(E(map, i), 8) = 0; W(E(map, i), 0) = 1; W(E(map, i), 12) = 0; W(E(map, i), 4) = parent;
    if (parent == 0) W(map, 12) = i; else if (left) W(E(map, parent), 8) = i; else W(E(map, parent), 12) = i;
    W(map, 0) += 1;
    return i;
}
extern "C" void LeftRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi(unsigned char* map, int x) {
    ++lefts;
    int y = W(E(map, x), 12);
    W(E(map, x), 12) = W(E(map, y), 8);
    if (W(E(map, y), 8)) W(E(map, W(E(map, y), 8)), 4) = x;
    W(E(map, y), 4) = W(E(map, x), 4);
    int px = W(E(map, x), 4);
    if (px == 0) W(map, 12) = y; else if (x == W(E(map, px), 8)) W(E(map, px), 8) = y; else W(E(map, px), 12) = y;
    W(E(map, y), 8) = x; W(E(map, x), 4) = y;
}
extern "C" void RightRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi(unsigned char* map, int x) {
    ++rights;
    int y = W(E(map, x), 8);
    W(E(map, x), 8) = W(E(map, y), 12);
    if (W(E(map, y), 12)) W(E(map, W(E(map, y), 12)), 4) = x;
    W(E(map, y), 4) = W(E(map, x), 4);
    int px = W(E(map, x), 4);
    if (px == 0) W(map, 12) = y; else if (x == W(E(map, px), 12)) W(E(map, px), 12) = y; else W(E(map, px), 8) = y;
    W(E(map, y), 12) = x; W(E(map, x), 4) = y;
}
// returns the black height of the subtree or -1 when the red-black rules or the ordering are broken
static int check(unsigned char* map, int i, int lo, int hi, int parent) {
    if (i == 0) return 1;
    unsigned char* e = E(map, i);
    int key = W(e, 16);
    if (key <= lo || key >= hi || W(e, 4) != parent) return -1;
    if (W(e, 0) == 1 && (W(E(map, W(e, 8)), 0) == 1 || W(E(map, W(e, 12)), 0) == 1)) return -1;
    int l = check(map, W(e, 8), lo, key, i), r = check(map, W(e, 12), key, hi, i);
    if (l < 0 || r < 0 || l != r) return -1;
    return l + (W(e, 0) == 0);
}
int main() {
    bool ok = true;
    unsigned char* map = arena;
    const int capacity = 200;
    unsigned char* entries = arena + 256;
    int orders[3] = {1, -1, 7};
    for (int order = 0; order < 3 && ok; ++order) {
        std::memset(arena, 0, sizeof arena);
        W(map, 4) = capacity; *reinterpret_cast<unsigned*>(map + 8) = arena_base + 256; W(map, 12) = 0; W(map, 16) = 1;
        for (int i = 1; i <= capacity; ++i) W(entries + 104 * i, 4) = i < capacity ? i + 1 : -1;
        int inserted = 0;
        for (int j = 0; j < 150; ++j) {
            int key = orders[order] == 1 ? j * 2 : orders[order] == -1 ? 1000 - j * 2 : (j * 37) % 151 * 2;
            EESurfCacheInfo v;
            for (int w = 0; w < 21; ++w) v.words[w] = key * 100 + w;
            int index = Insert__t5VIMap2ZiZ17VIEESurfCacheInfoRCiRC17VIEESurfCacheInfo(map, &key, &v);
            ok = ok && index > 0 && W(E(map, index), 16) == key && W(E(map, index), 20) == key * 100 && W(E(map, index), 20 + 80) == key * 100 + 20;
            int again = Insert__t5VIMap2ZiZ17VIEESurfCacheInfoRCiRC17VIEESurfCacheInfo(map, &key, &v);   // a duplicate returns the same entry
            ok = ok && again == index;
            ++inserted;
            ok = ok && check(map, W(map, 12), -1, 100000, 0) > 0 && W(E(map, W(map, 12)), 0) == 0;
        }
        ok = ok && W(map, 0) == inserted && lefts + rights > 0;
    }
    fail_add = 1; adds = 0;
    int key = 123456; EESurfCacheInfo v = {};
    ok = ok && Insert__t5VIMap2ZiZ17VIEESurfCacheInfoRCiRC17VIEESurfCacheInfo(map, &key, &v) == -1 && adds == 1;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
