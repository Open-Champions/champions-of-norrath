// Host check for skelightParams. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "skelightParams.cpp"
int main() {
    if (sizeof skelightParams != 32u) return 1;
    if (reinterpret_cast<std::uintptr_t>(&skelightParams) % 8u != 0u) return 2;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&skelightParams);
    for (unsigned i = 0; i < sizeof skelightParams; ++i)
        if (bytes[i] != 0) return 3;
    return 0;
}
