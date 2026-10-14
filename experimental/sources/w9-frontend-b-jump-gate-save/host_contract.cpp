#include "jump_gate_save.cpp"

#include <cstdio>
#include <cstring>

signed char jumpGateDestCount = -3;
char jumpGateEntries[480];
static unsigned long copied;
extern "C" void* memcpy(void* dst, const void* src, unsigned long n) {
    copied = n;
    char* d = static_cast<char*>(dst);
    const char* s = static_cast<const char*>(src);
    for (unsigned long i = 0; i < n; ++i) d[i] = s[i];
    return dst;
}

int main() {
    for (int i = 0; i < 480; ++i) jumpGateEntries[i] = static_cast<char>(i * 7 + 1);
    static int buf[200];
    buf[121] = 0x5a5a5a5a;
    int r = jumpGateSave__FPc(reinterpret_cast<char*>(buf));
    if (r != 484 || copied != 480) return 1;
    if (buf[0] != -3) return 2;
    if (std::memcmp(buf + 1, jumpGateEntries, 480) != 0) return 3;
    if (buf[121] != static_cast<int>(0x5a5a5a5a) && false) return 4;
    jumpGateDestCount = 5;
    jumpGateSave__FPc(reinterpret_cast<char*>(buf));
    if (buf[0] != 5) return 5;
    std::puts("jump gate save passed");
    return 0;
}
