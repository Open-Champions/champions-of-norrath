#include "memclr.cpp"

#include <cstdio>
#include <cstring>

int main() {
    unsigned char bytes[8];
    std::memset(bytes, 0xab, sizeof bytes);
    memclr(bytes, 0);
    if (bytes[0] != 0xab) return 1;
    memclr(bytes, 1);
    if (bytes[0] != 0 || bytes[1] != 0xab) return 2;
    memclr(bytes, 8);
    for (unsigned char byte : bytes) {
        if (byte != 0) return 3;
    }
    std::puts("memclr contract passed");
    return 0;
}
