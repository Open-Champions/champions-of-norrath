// Host check for redSpark. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "redSpark.cpp"
int main() {
    if (sizeof redSpark != 80u) return 1;
    if (reinterpret_cast<std::uintptr_t>(redSpark) % 16u != 0u) return 2;
    for (unsigned i = 0; i < sizeof redSpark; ++i)
        if (redSpark[i] != 0) return 3;
    return 0;
}
