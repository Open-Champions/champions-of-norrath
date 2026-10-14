// Host check for pnums.cpp; not an EE plan input.
#include "pnums.cpp"

int main() {
    if (pnums[0][0] != 0 || pnums[3][7] != 0) return 1;
    pnums[3][7] = 1;
    if (pnums[3][7] != 1) return 2;
    return 0;
}
