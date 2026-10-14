#include "lump_state.cpp"
#include <cstdio>
int main() {
    if (sizeof(lumpLoadLump) != 4u || sizeof(lumpLoadSize) != 4u) return 1;
    if (sizeof(lumpLoadName) != 192u || sizeof(discardable) != 32u) return 2;
    if (lumpLoadCompletionFlag != 0 || lumpLoadLump[0] != 0 || lumpLoadName[0] != 0) return 3;
    if (discardable[0] != 0 || discardable[31] != 0 || lumpLoadSize[3] != 0) return 4;
    std::puts("lump state contract passed");
    return 0;
}
