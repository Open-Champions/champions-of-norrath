#include "storage.hpp"
#include <cstdio>
int main() {
    static const unsigned char expect[36] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    for (unsigned i = 0; i != 36; ++i) {
        if (norrath_restore_defaults_view[i] != expect[i]) return 1;
    }
    norrath_restore_defaults_view[0] = static_cast<unsigned char>(norrath_restore_defaults_view[0] ^ 0x5au);
    if (norrath_restore_defaults_view[0] == expect[0]) return 2;
    norrath_restore_defaults_view[0] = expect[0];
    if (norrath_restore_defaults_view[0] != expect[0]) return 3;
    std::puts("Define the restore defaults object.");
    return 0;
}
