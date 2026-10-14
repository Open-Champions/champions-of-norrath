#include "storage.hpp"
#include <cstdio>
int main() {
    static const unsigned char expect[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    for (unsigned i = 0; i != 8; ++i) {
        if (norrath_restore_broadcast_head_view[i] != expect[i]) return 1;
    }
    norrath_restore_broadcast_head_view[0] = static_cast<unsigned char>(norrath_restore_broadcast_head_view[0] ^ 0x5au);
    if (norrath_restore_broadcast_head_view[0] == expect[0]) return 2;
    norrath_restore_broadcast_head_view[0] = expect[0];
    if (norrath_restore_broadcast_head_view[0] != expect[0]) return 3;
    std::puts("Define the restore broadcast-list head.");
    return 0;
}
