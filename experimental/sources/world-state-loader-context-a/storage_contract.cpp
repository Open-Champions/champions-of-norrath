#include "storage.hpp"
#include <cstdio>
int main() {
    static const unsigned char expect[8] = { 0x67, 0x6f, 0x74, 0x68, 0x69, 0x63, 0x32, 0x00 };
    for (unsigned i = 0; i != 8; ++i) {
        if (norrath_loader_context_a_view[i] != expect[i]) return 1;
    }
    
    std::puts("Define the first loader context key.");
    return 0;
}
