// Host check for getDamageRangeAll__4itemPQ24item11descriptionPiT2i. Not an EE plan input.
#include "get_damage_range_all.cpp"
#include <cstring>
static float multiplier = 1.0f;
static int calls;
extern "C" float getDamageMultiplier__4itemPQ24item11description(void*) { calls++; return multiplier; }
alignas(8) static unsigned char desc[96];
alignas(8) static unsigned char base[32];
static void rec(int k, short id, short a, short b) {
    short* r = reinterpret_cast<short*>(desc + 36 + 8 * k);
    r[0] = id; r[1] = a; r[2] = b;
}
static void clear() {
    memset(desc, 0, sizeof desc);
    *reinterpret_cast<unsigned char**>(desc) = base;
    *reinterpret_cast<short*>(base + 14) = 10;
    *reinterpret_cast<short*>(base + 16) = 20;
    calls = 0; multiplier = 1.0f;
}
int main() {
    int lo = -1, hi = -1;
    clear();
    getDamageRangeAll__4itemPQ24item11descriptionPiT2i(desc, &lo, &hi, 0);
    if (lo != 10 || hi != 20 || calls != 2) return 1;
    // id 35 scales both by (1 + a / 100): 50 -> x1.5
    clear(); rec(2, 35, 50, 999);
    getDamageRangeAll__4itemPQ24item11descriptionPiT2i(desc, &lo, &hi, 0);
    if (lo != 15 || hi != 30) return 2;
    // ids 21, 22 and 24 add only when extra is nonzero; the second value goes to hi
    clear(); rec(0, 21, 3, 4); rec(1, 22, 5, 6); rec(3, 24, 7, 8);
    getDamageRangeAll__4itemPQ24item11descriptionPiT2i(desc, &lo, &hi, 0);
    if (lo != 10 || hi != 20) return 3;
    getDamageRangeAll__4itemPQ24item11descriptionPiT2i(desc, &lo, &hi, 1);
    if (lo != 25 || hi != 38) return 4;
    // a modifier whose first value is 0 is ignored even when its second value is set
    clear(); rec(0, 21, 0, 50);
    getDamageRangeAll__4itemPQ24item11descriptionPiT2i(desc, &lo, &hi, 1);
    if (lo != 10 || hi != 20) return 5;
    // multiplier and rounding: 10 * 1.25 + 0.5 = 13.0 (truncated), 20 * 1.25 + 0.5 = 25.5 -> 25
    clear(); multiplier = 1.25f;
    getDamageRangeAll__4itemPQ24item11descriptionPiT2i(desc, &lo, &hi, 0);
    if (lo != 13 || hi != 25) return 6;
    // only the first of two records with the same id counts
    clear(); rec(0, 35, 100, 0); rec(1, 35, 300, 0);
    getDamageRangeAll__4itemPQ24item11descriptionPiT2i(desc, &lo, &hi, 0);
    if (lo != 20 || hi != 40) return 7;
    return 0;
}
