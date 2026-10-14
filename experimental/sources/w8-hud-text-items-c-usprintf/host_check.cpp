#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <string>
#include <vector>
typedef unsigned short u16;
extern "C" void usprintf__FPUsT0ffffffff(u16*, u16*, float, float, float, float, float, float, float, float);
extern "C" int norrath_host_sprintf(char* out, const char* fmt, ...) {
    va_list ap; va_start(ap, fmt);
    int n;
    if (!strcmp(fmt, "%.2f")) {
        unsigned long long bits = va_arg(ap, unsigned long long);
        double d; memcpy(&d, &bits, 8);
        n = snprintf(out, 256, fmt, d);
    } else if (!strcmp(fmt, "%s")) {
        int p = va_arg(ap, int);
        n = snprintf(out, 256, fmt, (const char*)(uintptr_t)p);
    } else {
        n = snprintf(out, 256, fmt, va_arg(ap, int));
    }
    va_end(ap);
    return n;
}
extern "C" unsigned long long fptodp(float f) { double d = f; unsigned long long b; memcpy(&b, &d, 8); return b; }

// Host pointers rarely fit a float's 24-bit mantissa, so the %u case is only
// exercised when the static buffer's address does, and %s is not exercised.
static std::string run(const char* fmt, float a, float b = 0, float c = 0, float d = 0) {
    u16 f[64] = {0}; for (int i = 0; fmt[i]; i++) f[i] = (unsigned char)fmt[i];
    u16 out[256]; for (auto& u : out) u = 0xdddd;
    usprintf__FPUsT0ffffffff(out, f, a, b, c, d, 0, 0, 0, 0);
    std::string s;
    for (int i = 0; out[i]; i++) s += (char)out[i];
    return s;
}

int main() {
    if (run("hello", 0) != "hello") return 1;
    if (run("a%db", 42) != "a42b") return 2;
    if (run("%d %d", -7, 100) != "-7 100") return 3;
    if (run("x%dy", 3.9f) != "x3y") return 4;                 // truncation toward zero
    if (run("%c%c", 72, 105) != "Hi") return 5;
    if (run("100%% sure", 0) != "100% sure") return 6;
    if (run("%f", 1.005f) != "1.00" && run("%f", 1.005f) != "1.01") return 7;
    if (run("[%f]", 3.14159f) != "[3.14]") return 8;
    if (run("%d:%f", 5, 2.5f) != "5:2.50") return 9;
    if (run("%q%d", 9) != "9") return 10;                      // unknown conversion consumes no argument
    if (run("%%%d", 12) != "%12") return 11;
    if (run("%d%d%d%d", 1, 2, 3, 4) != "1234") return 12;
    // %u copies a 16-bit string whose address is passed as a float.
    static u16 wide[8] = {'o', 'k', 0};
    uintptr_t addr = (uintptr_t)wide;
    if (addr < (1u << 24)) {
        u16 f[8] = {'%', 'u', 0, 0}; u16 out[16] = {0};
        usprintf__FPUsT0ffffffff(out, f, (float)(int)addr, 0, 0, 0, 0, 0, 0, 0);
        if (out[0] != 'o' || out[1] != 'k' || out[2] != 0) return 13;
    }
    // Format ending in a conversion needs a following zero unit.
    { u16 f[4] = {'%', 'd', 0, 0}; u16 out[16] = {0xdddd, 0xdddd, 0xdddd, 0xdddd};
      usprintf__FPUsT0ffffffff(out, f, 5, 0, 0, 0, 0, 0, 0, 0);
      if (out[0] != '5' || out[1] != 0 || out[2] != 0) return 14; }
    // Signed byte widening of sprintf output is covered by the decimal text; negative numbers include '-'.
    if (run("%d", -2147483648.0f) != "-2147483648") return 15;
    return 0;
}
