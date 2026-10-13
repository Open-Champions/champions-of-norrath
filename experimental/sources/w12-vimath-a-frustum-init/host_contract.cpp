#include "init.cpp"
#include <string.h>

int main() {
    unsigned char block[64];
    memset(block, 0x5a, sizeof block);
    if (Init__9VIFrustum(block) != 0) return 1;
    for (unsigned char b : block) if (b != 0x5a) return 2;
    return 0;
}
