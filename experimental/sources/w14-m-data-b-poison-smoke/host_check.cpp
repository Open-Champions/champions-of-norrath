// Host check for poisonSmoke. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "poisonSmoke.cpp"
int main() {
    if (sizeof poisonSmoke != 80u) return 1;
    if (reinterpret_cast<std::uintptr_t>(poisonSmoke) % 16u != 0u) return 2;
    for (unsigned i = 0; i < sizeof poisonSmoke; ++i)
        if (poisonSmoke[i] != 0) return 3;
    return 0;
}
