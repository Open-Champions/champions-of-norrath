// Host check only; not an EE build input. Checks extent and values against the list in the source comment.
#include <cstdio>
extern "C" long long gsm_DLB[4];
int main() {
    const long long want[4] = {6554, 16384, 26214, 32767};
    if (sizeof(gsm_DLB) != sizeof(want)) return 1;
    for (int i = 0; i < 4; ++i) if (gsm_DLB[i] != want[i]) return 2;
    std::puts("gsm_DLB ok");
    return 0;
}
