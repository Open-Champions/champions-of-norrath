// Host check, not a plan input. Pointer slots are offsets into a small arena (the target uses 32-bit pointers); numbers go
// through the host sprintf. Floating-point conversions are covered by the paired runs, not here.
#include <cstdarg>
#include <cstdio>
#include <cstring>
#define NORRATH_HOST_CONTRACT 1
static unsigned char arena[65536];
extern "C" void* norrath_host_buffer(unsigned address) { return address == 0 ? nullptr : arena + address; }
#define sprintf norrath_host_sprintf
#include "vsnprintf.cpp"
#undef sprintf
extern "C" const unsigned short _8VIString$ConversionTable1252[32] = {0xfffe, 0xfffe, 0x20a1, 0x0192, 0x201e, 0x2026};
extern "C" int norrath_conversion_table_size __asm__("ConversionTableSize.180") = 32;
const char* __VIAssertf_file; int __VIAssertf_line;
static int asserts;
extern "C" void _VIAssert(const char* e, const char* f, int l) { ++asserts; if (std::strcmp(e, "DstCount > 0") || std::strcmp(f, "../core/vistring.cpp") || l != 662) std::puts("bad assert"); }
extern "C" void _VIAssertf(int, const char*, ...) {}
extern "C" int norrath_host_sprintf(char* d, const char* f, ...) {
    va_list ap; __builtin_va_start(ap, f); int r = std::vsnprintf(d, 128, f, ap); __builtin_va_end(ap); return r;
}
static unsigned char slots[256];
static void put32(int slot, unsigned v) { std::memset(slots + 8 * slot, 0, 8); std::memcpy(slots + 8 * slot, &v, 4); }
typedef unsigned short U;
static void widen(U* d, const char* s) { while ((*d++ = static_cast<unsigned char>(*s++))) {} }
static void run(U* out, int bytes, const U* fmt) { VI_vsnprintf__H1ZUs_PX01iPCX01Pv_v(out, bytes, fmt, slots); }
static int same(const U* got, const char* want, int code) {
    for (int i = 0;; ++i) { if (got[i] != static_cast<unsigned char>(want[i])) { std::printf("case %d differs at %d: %u vs %d\n", code, i, got[i], want[i]); return 1; } if (!want[i]) return 0; }
}
int main() {
    int bad = 0;
    U out[128], fmt[128];
    std::memcpy(arena + 100, "hello\x85", 7);
    const U wide[] = {'W', 0xe9, 0x2026, 'z', 0};
    std::memcpy(arena + 200, wide, sizeof wide);
    const unsigned char block[] = {0x01, 0xab, 0xcd, 0xef, 0x10, 0x20};
    std::memcpy(arena + 300, block, sizeof block);
    put32(0, 42); put32(1, 100); put32(2, 0x141);
    widen(fmt, "n=%d s=%s c=%c %%"); run(out, 256, fmt);
    if (out[0] != 'n' || out[2] != '4' || out[3] != '2' || out[7] != 'h' || out[12] != 0x2026 || out[16] != 0x141 || out[18] != '%' || out[19] != 0) { std::puts("case 1"); ++bad; }
    put32(0, 255); widen(fmt, "%04x|%-5d|"); run(out, 256, fmt); bad += same(out, "00ff|100  |", 2);
    widen(fmt, "abcdefgh"); run(out, 10, fmt); bad += same(out, "abcd", 3);
    put32(0, 0); widen(fmt, "<%s>"); run(out, 256, fmt); bad += same(out, "<(null)>", 4);
    put32(0, 0); widen(fmt, "<%S>"); run(out, 256, fmt); bad += same(out, "<(null)>", 5);
    put32(0, 200); widen(fmt, "%S"); run(out, 256, fmt);
    if (out[0] != 'W' || out[1] != 0xe9 || out[2] != 0x2026 || out[3] != 'z' || out[4] != 0) { std::puts("case 6"); ++bad; }
    put32(0, 300); put32(1, 6); widen(fmt, "%h"); run(out, 256, fmt); bad += same(out, "01ABCDEF 1020", 7);
    put32(0, 300); put32(1, 3); widen(fmt, "%h"); run(out, 10, fmt); bad += same(out, "01AB", 8);
    widen(fmt, "tail%"); run(out, 256, fmt); bad += same(out, "tail", 9);
    put32(0, 0); widen(fmt, "x"); run(out, 0, fmt); if (asserts != 1) { std::puts("assert count"); ++bad; }
    if (!bad) std::puts("wide vsnprintf check passed");
    return bad;
}
