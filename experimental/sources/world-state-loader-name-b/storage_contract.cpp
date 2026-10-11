#include "storage.hpp"
#include <cstdio>
int main() {
    static const unsigned char expect[13] = { 0x73, 0x75, 0x70, 0x65, 0x72, 0x6f, 0x72, 0x63, 0x2e, 0x6c, 0x6d, 0x70, 0x00 };
    for (unsigned i = 0; i != 13; ++i) {
        if (norrath_loader_name_b_view[i] != expect[i]) return 1;
    }
    
    std::puts("Define the second loader name key.");
    return 0;
}
