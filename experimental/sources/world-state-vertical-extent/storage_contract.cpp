#include "storage.hpp"
#include <cstdio>
int main() {
    static const unsigned char expect[4] = { 0x00, 0x50, 0xc3, 0x47 };
    for (unsigned i = 0; i != 4; ++i) {
        if (norrath_grid_vertical_extent_view[i] != expect[i]) return 1;
    }
    
    std::puts("Define the grid vertical-extent constant.");
    return 0;
}
