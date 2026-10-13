// Host contract for bCrossingWater__8Froglock; not an EE input. Tries every zero/non-zero combination of
// the four bytes (including the sign bit) against the rule stated in b_crossing_water.cpp.
#include "b_crossing_water.cpp"
int main() {
    alignas(16) static signed char object[2048];
    int bad = 0;
    const signed char vals[] = {0, 1, -1};
    for (int a : vals) for (int b : vals) for (int c : vals) for (int d : vals) {
        object[1104] = a; object[1105] = b; object[1106] = c; object[568] = d;
        int want = (a == 0 && d != 0) ? 1 : (c != 0 && b == 0 ? 1 : 0);
        if (bCrossingWater__8Froglock(object) != want) bad = 1;
    }
    return bad;
}
