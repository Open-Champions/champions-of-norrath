// Host contract for lifeTimes. Not an EE plan input.
#include <cstdio>
extern "C" unsigned lifeTimes[4];
int main() {
    const unsigned want[4] = {11u, 30u, 60u, 90u};
    for (int i = 0; i < 4; ++i) {
        if (lifeTimes[i] != want[i]) { std::fprintf(stderr, "fail: word %d\n", i); return 1; }
    }
    if (sizeof lifeTimes != 16u) return 1;
    return 0;
}
