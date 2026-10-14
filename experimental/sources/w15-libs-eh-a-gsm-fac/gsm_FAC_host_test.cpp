// Host check only; not an EE build input. Checks extent and values against the list in the source comment.
#include <cstdio>
extern "C" long long gsm_FAC[8];
int main() {
    const long long want[8] = {18431, 20479, 22527, 24575, 26623, 28671, 30719, 32767};
    if (sizeof(gsm_FAC) != sizeof(want)) return 1;
    for (int i = 0; i < 8; ++i) if (gsm_FAC[i] != want[i]) return 2;
    std::puts("gsm_FAC ok");
    return 0;
}
