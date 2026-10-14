// Host check only; not an EE build input. Checks extent and values against the list in the source comment.
#include <cstdio>
extern "C" long long gsm_H[11];
int main() {
    const long long want[11] = {-134, -374, 0, 2054, 5741, 8192, 5741, 2054, 0, -374, -134};
    if (sizeof(gsm_H) != sizeof(want)) return 1;
    for (int i = 0; i < 11; ++i) if (gsm_H[i] != want[i]) return 2;
    std::puts("gsm_H ok");
    return 0;
}
