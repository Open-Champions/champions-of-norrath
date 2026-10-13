// Host contract for the ListHead copy constructor; not an EE input.
#include "list_head_copy.cpp"
#include <cstdio>
int main() {
    unsigned dest[3] = {0xdeadbeef, 0x11111111, 0x22222222};
    unsigned src[2] = {0x12345678, 0x9abcdef0};
    ListHead_copy(dest, src);
    if (dest[0] != 0 || dest[1] != 0x11111111 || dest[2] != 0x22222222 || src[0] != 0x12345678 || src[1] != 0x9abcdef0) {
        std::puts("fail: list head copy");
        return 1;
    }
    std::puts("list head copy ok");
    return 0;
}
