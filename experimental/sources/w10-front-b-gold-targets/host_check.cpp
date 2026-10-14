#include "gold_targets.cpp"
#include <cstdio>

int main() {
    const int x[4] = {50, 590, 50, 590}, y[4] = {200, 200, 250, 250};
    int bad = sizeof(playerGoldx) != 16 || sizeof(playerGoldy) != 16;
    for (int i = 0; i < 4; ++i) bad |= playerGoldx[i] != x[i] || playerGoldy[i] != y[i];
    std::printf(bad ? "FAIL\n" : "ok: 16-byte arrays with the observed values\n");
    return bad;
}
