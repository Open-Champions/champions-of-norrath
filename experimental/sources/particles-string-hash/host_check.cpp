// Host contract for stringHash. Not an EE plan input.
#include <cstdio>
extern "C" unsigned stringHash[512];
int main() {
    if (sizeof stringHash != 2048u) return 1;
    for (int i = 0; i < 512; ++i) {
        if (stringHash[i] != 0u) return 1;
    }
    stringHash[0] = 1u;
    if (stringHash[0] != 1u) return 1;
    return 0;
}
