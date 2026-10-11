#include "storage.hpp"
#include <cstdio>
int main() {
    static const unsigned char expect[4] = { 0x00, 0x00, 0x00, 0x00 };
    for (unsigned i = 0; i != 4; ++i) {
        if (norrath_list_copy_object_word[i] != expect[i]) return 1;
    }
    norrath_list_copy_object_word[0] = static_cast<unsigned char>(norrath_list_copy_object_word[0] ^ 0x5au);
    if (norrath_list_copy_object_word[0] == expect[0]) return 2;
    norrath_list_copy_object_word[0] = expect[0];
    if (norrath_list_copy_object_word[0] != expect[0]) return 3;
    std::puts("Define the list copy-object word.");
    return 0;
}
