#include "storage.hpp"
#include <cstdio>
int main() {
    static const unsigned char expect[4] = { 0x00, 0x00, 0x00, 0x00 };
    for (unsigned i = 0; i != 4; ++i) {
        if (norrath_route_free_root_bytes[i] != expect[i]) return 1;
    }
    norrath_route_free_root_bytes[0] = static_cast<unsigned char>(norrath_route_free_root_bytes[0] ^ 0x5au);
    if (norrath_route_free_root_bytes[0] == expect[0]) return 2;
    norrath_route_free_root_bytes[0] = expect[0];
    if (norrath_route_free_root_bytes[0] != expect[0]) return 3;
    std::puts("Define the route free-root word.");
    return 0;
}
