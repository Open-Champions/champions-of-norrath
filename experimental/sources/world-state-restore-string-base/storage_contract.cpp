#include "storage.hpp"
#include <cstdio>
int main() {
    static const unsigned char expect[4] = { 0x00, 0x00, 0x00, 0x00 };
    for (unsigned i = 0; i != 4; ++i) {
        if (norrath_restore_string_base_word[i] != expect[i]) return 1;
    }
    norrath_restore_string_base_word[0] = static_cast<unsigned char>(norrath_restore_string_base_word[0] ^ 0x5au);
    if (norrath_restore_string_base_word[0] == expect[0]) return 2;
    norrath_restore_string_base_word[0] = expect[0];
    if (norrath_restore_string_base_word[0] != expect[0]) return 3;
    std::puts("Define the restore string-base word.");
    return 0;
}
