// Host comparison only. Not an EE build input.
#include <cstdio>
#include <initializer_list>

#define close w8_close_under_test
#include "close.cpp"
#undef close

int main() {
    const int descriptors[] = {-1, 0, 1, 2, 3, 100, 0x7fffffff};
    for (int descriptor : descriptors) {
        if (w8_close_under_test(descriptor) != -1) {
            std::printf("close(%d) did not return -1\n", descriptor);
            return 1;
        }
    }
    std::printf("close cases passed\n");
    return 0;
}
