// Host check only; not an EE build input. Checks extent and values against the list in the source comment.
#include <cstdio>
extern "C" long long gsm_QLB[4];
int main() {
    const long long want[4] = {3277, 11469, 21299, 32767};
    if (sizeof(gsm_QLB) != sizeof(want)) return 1;
    for (int i = 0; i < 4; ++i) if (gsm_QLB[i] != want[i]) return 2;
    std::puts("gsm_QLB ok");
    return 0;
}
