// Host comparison only. Not an EE build input.
#include <cstdio>
#include <initializer_list>

#define lseek w8_lseek_under_test
#include "lseek.cpp"
#undef lseek

int main() {
    const long long offsets[] = {0, 1, -1, 0x100000000ll, -0x100000000ll, 0x7fffffffffffffffll};
    for (int descriptor : {-1, 0, 1, 7}) {
        for (long long offset : offsets) {
            for (int whence = 0; whence < 3; ++whence) {
                if (w8_lseek_under_test(descriptor, offset, whence) != -1ll) {
                    std::printf("lseek(%d, %lld, %d) did not return -1\n", descriptor, offset, whence);
                    return 1;
                }
            }
        }
    }
    std::printf("lseek cases passed\n");
    return 0;
}
