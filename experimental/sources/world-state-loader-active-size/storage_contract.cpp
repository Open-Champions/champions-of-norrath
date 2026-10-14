#include "storage.hpp"
#include <cstdio>
int main() {
    static const unsigned char expect[4] = { 0x00, 0x00, 0x00, 0x00 };
    for (unsigned i = 0; i != 4; ++i) {
        if (norrath_loader_active_size_view[i] != expect[i]) return 1;
    }
    norrath_loader_active_size_view[0] = static_cast<unsigned char>(norrath_loader_active_size_view[0] ^ 0x5au);
    if (norrath_loader_active_size_view[0] == expect[0]) return 2;
    norrath_loader_active_size_view[0] = expect[0];
    if (norrath_loader_active_size_view[0] != expect[0]) return 3;
    std::puts("Define the loader active-size word.");
    return 0;
}
