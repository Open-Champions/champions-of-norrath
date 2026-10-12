// Host check only; not an EE build input.
#include "inline_bases.cpp"
#include <cstdio>
int main() {
    unsigned char* nodes[] = {__ti4Prop, __ti9AutoChest, __ti10AutoObject, __ti13AutoContainer};
    const unsigned sizes[] = {sizeof(__ti4Prop), sizeof(__ti9AutoChest), sizeof(__ti10AutoObject), sizeof(__ti13AutoContainer)};
    for (int i = 0; i < 4; ++i) {
        if (sizes[i] != 12) return 1;
        for (unsigned b = 0; b < 12; ++b) if (nodes[i][b] != 0) return 2;  // zero at start
        for (int j = i + 1; j < 4; ++j) if (nodes[i] == nodes[j]) return 3;  // distinct objects
    }
    std::puts("inline bases ok");
    return 0;
}
