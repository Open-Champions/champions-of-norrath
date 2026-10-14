// Host check for poisonFleck. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "poisonFleck.cpp"
int main() {
    if (sizeof poisonFleck != 80u) return 1;
    if (reinterpret_cast<std::uintptr_t>(poisonFleck) % 16u != 0u) return 2;
    for (unsigned i = 0; i < sizeof poisonFleck; ++i)
        if (poisonFleck[i] != 0) return 3;
    return 0;
}
