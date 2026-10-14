// Host check, not a plan input.
#include <cstdio>
#include <cstring>
#include <vector>
#define strlen norrath_host_strlen
#define __builtin_alloca(n) host_alloca(n)
static unsigned char host_pool[4096]; static unsigned alloc_size;
static void* host_alloca(unsigned n) { alloc_size = n; return host_pool; }
#include "vsnprintf16.cpp"
#undef strlen
static unsigned seen_units; static const char* seen_src; static void* seen_args; static int seen_size; static unsigned short* seen_buf; static const unsigned short* seen_fmt;
extern "C" unsigned norrath_host_strlen(const char* s) { return std::strlen(s); }
extern "C" unsigned short* str8to16__8VIStringPUsUiPCc(unsigned short* d, unsigned size, const char* s) {
    seen_units = size; seen_src = s;
    unsigned k = 0; while (s[k] && 2 * (k + 1) < size) { d[k] = static_cast<unsigned char>(s[k]); ++k; }
    d[k] = 0; return d;
}
extern "C" void vsnprintf16__8VIStringPUsiPCUsPv(unsigned short* b, int n, const unsigned short* f, void* a) { seen_buf = b; seen_size = n; seen_fmt = f; seen_args = a; }
int main() {
    unsigned short out[4]; int args;
    const char* cases[] = {"", "a", "abc %d", "0123456789abcdef"};
    for (const char* c : cases) {
        const unsigned n = std::strlen(c);
        vsnprintf16__8VIStringPUsiPCcPv(out, 9, c, &args);
        if (seen_units != 2 * n + 2 || seen_src != c || seen_buf != out || seen_size != 9 || seen_args != &args) return 1;
        if (alloc_size != ((4 * n + 4 + 15) & ~15u)) return 2;
        if (seen_fmt != reinterpret_cast<unsigned short*>(host_pool)) return 3;
        for (unsigned i = 0; i < n; ++i) if (seen_fmt[i] != static_cast<unsigned char>(c[i])) return 4;
        if (seen_fmt[n] != 0) return 5;
    }
    std::puts("vsnprintf16 narrow check passed");
    return 0;
}
