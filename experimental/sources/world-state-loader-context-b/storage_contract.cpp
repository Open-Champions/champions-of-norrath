#include "storage.hpp"
#include <cstdio>
int main() {
    static const unsigned char expect[9] = { 0x72, 0x75, 0x69, 0x6e, 0x69, 0x6e, 0x74, 0x31, 0x00 };
    for (unsigned i = 0; i != 9; ++i) {
        if (norrath_loader_context_b_view[i] != expect[i]) return 1;
    }
    
    std::puts("Define the second loader context key.");
    return 0;
}
