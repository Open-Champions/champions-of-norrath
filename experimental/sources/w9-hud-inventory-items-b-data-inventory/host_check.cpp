#include <cstddef>
extern "C" unsigned char inventory[464];
int main() {
    if (sizeof(inventory) != 464) return 1;
    for (std::size_t i = 0; i < sizeof(inventory); ++i) if (inventory[i] != 0) return 2;
    return 0;
}
