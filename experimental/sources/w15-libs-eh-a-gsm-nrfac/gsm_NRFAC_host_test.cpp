// Host check only; not an EE build input. Checks extent and values against the list in the source comment.
#include <cstdio>
extern "C" long long gsm_NRFAC[8];
int main() {
    const long long want[8] = {29128, 26215, 23832, 21846, 20165, 18725, 17476, 16384};
    if (sizeof(gsm_NRFAC) != sizeof(want)) return 1;
    for (int i = 0; i < 8; ++i) if (gsm_NRFAC[i] != want[i]) return 2;
    std::puts("gsm_NRFAC ok");
    return 0;
}
