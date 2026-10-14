#include "storage.hpp"
#include <cstdio>
int main() {
    static const unsigned char expect[13] = { 0x76, 0x61, 0x6d, 0x70, 0x6c, 0x6f, 0x72, 0x64, 0x2e, 0x6c, 0x6d, 0x70, 0x00 };
    for (unsigned i = 0; i != 13; ++i) {
        if (norrath_loader_name_a_view[i] != expect[i]) return 1;
    }
    
    std::puts("Define the first loader name key.");
    return 0;
}
