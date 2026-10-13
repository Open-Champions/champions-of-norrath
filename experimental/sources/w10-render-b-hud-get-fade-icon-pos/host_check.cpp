// Host contract for hudGetFadeIconPos__Fii. Not an EE plan input.
#include <cstdio>
extern "C" {
int fadeTimer[3][7];
int hudGetFadeIconPos__Fii(int, int);
}
static int reference(int row, int target) {
    int n = 0;
    for (int i = 0; i < 7; ++i) {
        bool live = fadeTimer[row][i] != 0;
        if (i == 5 && fadeTimer[row][2]) live = false;
        if (i == 4 && fadeTimer[row][1]) live = false;
        if (i == 6 && fadeTimer[row][3]) live = false;
        if (live && i != target) ++n;
    }
    return n;
}
int main() {
    int failures = 0;
    for (int mask = 0; mask < 128; ++mask) {
        for (int i = 0; i < 7; ++i) fadeTimer[1][i] = (mask >> i) & 1 ? 10 + i : 0;
        for (int i = 0; i < 7; ++i) { fadeTimer[0][i] = 77; fadeTimer[2][i] = 88; }
        for (int target = -1; target < 8; ++target)
            if (hudGetFadeIconPos__Fii(1, target) != reference(1, target)) { ++failures; }
    }
    if (failures) { std::fprintf(stderr, "fail: %d\n", failures); return 1; }
    std::printf("hudGetFadeIconPos contract passed\n");
    return 0;
}
