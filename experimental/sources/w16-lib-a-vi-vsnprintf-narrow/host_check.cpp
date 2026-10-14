// Host check, not a plan input. Pointer slots are offsets into a small arena (the target uses 32-bit pointers); numbers go
// through the host sprintf. Floating-point conversions are covered by the paired runs, not here.
#include <cstdio>
#include <cstring>
#define NORRATH_HOST_CONTRACT 1
static unsigned char arena[65536];
extern "C" void* norrath_host_buffer(unsigned address) { return address == 0 ? nullptr : arena + address; }
#define sprintf norrath_host_sprintf
#include "vsnprintf.cpp"
#undef sprintf
extern "C" const unsigned short _8VIString$ConversionTable1252[32] = {0xfffe, 0xfffe, 0x20a1};
extern "C" int norrath_conversion_table_size __asm__("ConversionTableSize.180") = 32;
extern "C" const char* __VIAssertf_file; extern "C" int __VIAssertf_line;
const char* __VIAssertf_file; int __VIAssertf_line;
static int asserts;
extern "C" void _VIAssert(const char* e, const char* f, int l) { ++asserts; if (std::strcmp(e, "DstCount > 0") || std::strcmp(f, "../core/vistring.cpp") || l != 662) std::puts("bad assert"); }
extern "C" void _VIAssertf(int, const char*, ...) {}
extern "C" int norrath_host_sprintf(char* d, const char* f, ...) {
    va_list ap; __builtin_va_start(ap, f); int r = std::vsnprintf(d, 128, f, ap); __builtin_va_end(ap); return r;
}
static unsigned char slots[256];
static void put32(int slot, unsigned v) { std::memset(slots + 8 * slot, 0, 8); std::memcpy(slots + 8 * slot, &v, 4); }
static int run(char* out, int cap, const char* fmt) { VI_vsnprintf__H1Zc_PX01iPCX01Pv_v(out, cap, fmt, slots); return static_cast<int>(std::strlen(out)); }
static int check(const char* got, const char* want, int code) { if (std::strcmp(got, want)) { std::printf("case %d: got '%s' want '%s'\n", code, got, want); return 1; } return 0; }
int main() {
    int bad = 0;
    char out[128];
    std::memcpy(arena + 100, "hello", 6);
    const unsigned short wide[] = {'W', 0xe9, 0x2026, 'z', 0};
    std::memcpy(arena + 200, wide, sizeof wide);
    const unsigned char block[] = {0x01, 0xab, 0xcd, 0xef, 0x10, 0x20};
    std::memcpy(arena + 300, block, sizeof block);
    put32(0, 42); put32(1, 100); put32(2, 'Q');
    run(out, 128, "n=%d s=%s c=%c %%"); bad += check(out, "n=42 s=hello c=Q %", 1);
    put32(0, 255); run(out, 128, "%04x|%-5d|"); bad += check(out, "00ff|100  |", 2);
    run(out, 5, "abcdefgh"); bad += check(out, "abcd", 3);
    put32(0, 0); run(out, 128, "<%s>"); bad += check(out, "<(null)>", 4);
    put32(0, 200); run(out, 128, "%S"); bad += check(out, "W??z", 5);
    put32(0, 300); put32(1, 6); run(out, 128, "%h"); bad += check(out, "01ABCDEF 1020", 6);
    put32(0, 300); put32(1, 3); run(out, 5, "%h"); bad += check(out, "01AB", 7);
    put32(0, 0); run(out, 0, "x"); if (asserts != 1) { std::puts("assert count"); ++bad; }
    run(out, 128, "tail%"); bad += check(out, "tail", 9);
    if (!bad) std::puts("narrow vsnprintf check passed");
    return bad;
}
