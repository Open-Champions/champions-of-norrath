// Host check for skelightName. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "skelightName.cpp"
int main() {
    if (sizeof skelightName != 4u * sizeof(void*)) return 1;
    if (reinterpret_cast<std::uintptr_t>(&skelightName) % 4u != 0u) return 2;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&skelightName);
    for (unsigned i = 0; i < sizeof skelightName; ++i)
        if (bytes[i] != 0) return 3;
    return 0;
}
