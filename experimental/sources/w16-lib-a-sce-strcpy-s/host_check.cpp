// Host check, not a plan input.
#include <cstdio>
#include <cstring>
#define strcpy norrath_host_strcpy
#include "strcpy_s.cpp"
#undef strcpy
static const char* seen_src; static char* seen_dst; static int calls;
extern "C" char* norrath_host_strcpy(char* d, const char* s) { ++calls; seen_dst = d; seen_src = s; return d; }
int main() {
    int block[8]; char out[8];
    if (StrcpyS(nullptr, out, block, 5) != 0 || calls != 1 || seen_dst != out || seen_src != reinterpret_cast<const char*>(block + 5)) return 1;
    if (StrcpyS(block, nullptr, block, 2) != 0 || calls != 1) return 2;
    std::puts("StrcpyS check passed");
    return 0;
}
