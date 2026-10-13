// Host contract for jumpGateLoad__FPc. Not an EE plan input. Build: c++ -std=c++17 host_check.cpp
#include <cstdio>
#include <cstring>
signed char jumpGateDestCount = 99;
char jumpGateEntries[480 + 8];
static const void* last_src; static unsigned last_n; static void* last_dst;
#define memcpy norrath_host_memcpy
extern "C" void* norrath_host_memcpy(void* d, const void* s, __SIZE_TYPE__ n) {
    last_dst = d; last_src = s; last_n = n;
    unsigned char* a = static_cast<unsigned char*>(d); const unsigned char* b = static_cast<const unsigned char*>(s);
    for (__SIZE_TYPE__ i = 0; i != n; ++i) a[i] = b[i];
    return d;
}
#include "jump_gate_load.cpp"
#undef memcpy
int main() {
    char buf[600];
    for (int i = 0; i != 600; ++i) buf[i] = char(i * 3 + 1);
    int count = 0x1234fe03;  // only the low byte is kept
    std::memcpy(buf, &count, 4);
    std::memset(jumpGateEntries + 480, 0x7e, 8);
    if (jumpGateLoad__FPc(buf) != 484) return 1;
    if (jumpGateDestCount != 3) return 2;
    if (last_src != buf + 4 || last_n != 480 || last_dst != jumpGateEntries) return 3;
    if (std::memcmp(jumpGateEntries, buf + 4, 480) != 0) return 4;
    if (static_cast<unsigned char>(jumpGateEntries[480]) != 0x7e) return 5;
    count = 0xfe; std::memcpy(buf, &count, 4);
    jumpGateLoad__FPc(buf);
    if (jumpGateDestCount != -2) return 6;  // stored as a byte, read back signed
    std::puts("jumpGateLoad check passed");
    return 0;
}
