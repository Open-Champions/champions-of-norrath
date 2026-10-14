// Host check for skelightResourceMap. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "skelightResourceMap.cpp"
int main() {
    if (sizeof skelightResourceMap != 80u) return 1;
    if (reinterpret_cast<std::uintptr_t>(&skelightResourceMap) % 16u != 0u) return 2;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&skelightResourceMap);
    for (unsigned i = 0; i < sizeof skelightResourceMap; ++i)
        if (bytes[i] != 0) return 3;
    return 0;
}
