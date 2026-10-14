// Host check for skeletonResourceMap. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "skeletonResourceMap.cpp"
int main() {
    if (sizeof skeletonResourceMap != 256u) return 1;
    if (reinterpret_cast<std::uintptr_t>(&skeletonResourceMap) % 16u != 0u) return 2;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&skeletonResourceMap);
    for (unsigned i = 0; i < sizeof skeletonResourceMap; ++i)
        if (bytes[i] != 0) return 3;
    return 0;
}
