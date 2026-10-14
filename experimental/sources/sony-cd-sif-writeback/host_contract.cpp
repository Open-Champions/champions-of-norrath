#include <cstdio>
#include <vector>
#define NORRATH_HOST_CONTRACT 1
#include "writeback.cpp"

static std::vector<unsigned> lines;
extern "C" void norrath_sony_cd_record_cache_line(unsigned address) {
    lines.push_back(address);
}
static int expect(const std::vector<unsigned>& wanted) {
    if (lines != wanted) return 1;
    lines.clear();
    return 0;
}
int main() {
    sceSifWriteBackDCache(reinterpret_cast<void*>(0x1000u), 0);
    sceSifWriteBackDCache(reinterpret_cast<void*>(0x1000u), -4);
    if (!lines.empty()) return 1;
    sceSifWriteBackDCache(reinterpret_cast<void*>(0x1010u), 1);
    if (expect({0x1000u}) != 0) return 2;
    sceSifWriteBackDCache(reinterpret_cast<void*>(0x1010u), 100);
    if (expect({0x1000u, 0x1040u}) != 0) return 3;
    sceSifWriteBackDCache(reinterpret_cast<void*>(0x2000u), 512);
    if (expect({0x2000u, 0x2040u, 0x2080u, 0x20c0u, 0x2100u, 0x2140u, 0x2180u, 0x21c0u}) != 0) return 4;
    sceSifWriteBackDCache(reinterpret_cast<void*>(0x3000u), 576);
    if (expect({0x3000u, 0x3040u, 0x3080u, 0x30c0u, 0x3100u, 0x3140u, 0x3180u, 0x31c0u, 0x3200u}) != 0) return 5;
    std::puts("sif writeback contract passed");
    return 0;
}
