// Host contract for objectHash. Not an EE plan input.
#include <cstdio>
extern "C" unsigned objectHash[512];
int main() {
    if (sizeof objectHash != 2048u) return 1;
    for (int i = 0; i < 512; ++i) {
        if (objectHash[i] != 0u) return 1;
    }
    objectHash[0] = 1u;
    if (objectHash[0] != 1u) return 1;
    return 0;
}
