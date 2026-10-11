#include <cstdio>

extern "C" unsigned char norrath_enable_malloc_wrapper_byte_view;

int main() {
    if (sizeof norrath_enable_malloc_wrapper_byte_view != 1) return 1;
    if (norrath_enable_malloc_wrapper_byte_view != 0) return 2;
    // The wrapper treats every nonzero byte, including 0x80, as enabled.
    norrath_enable_malloc_wrapper_byte_view = 0x80;
    if (norrath_enable_malloc_wrapper_byte_view == 0) return 3;
    norrath_enable_malloc_wrapper_byte_view = 0;
    std::printf("malloc wrapper byte checks passed\n");
    return 0;
}
