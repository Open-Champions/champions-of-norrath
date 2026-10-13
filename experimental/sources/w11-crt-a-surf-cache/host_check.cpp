// Host contract for SurfCache. Not an EE plan input.
#include <cstdio>
#include "surf_cache.cpp"
int main() {
    if (sizeof(SurfCache) != 152) return 1;
    for (unsigned i = 0; i < 38; ++i) if (SurfCache[i] != 0) return 2;
    if (reinterpret_cast<unsigned char*>(&SurfCache[7]) - reinterpret_cast<unsigned char*>(SurfCache) != 28) return 3;
    std::puts("SurfCache ok");
    return 0;
}
