// The callee is a recording stub that reads integer variable arguments as 8-byte slots through the
// argument pointer. That holds on the Apple arm64 host this check was run on; elsewhere the body is
// compiled out and the check is not meaningful.
#include "snprintf16.cpp"
#if defined(__APPLE__) && defined(__aarch64__)
static void* gbuf; static int gsize; static const void* gfmt; static long gv0, gv1; static int gcalls;
extern "C" void VI_vsnprintf__H1ZUs_PX01iPCX01Pv_v(unsigned short* buf, int size, const unsigned short* fmt, void* ap) {
    gbuf = buf; gsize = size; gfmt = fmt; ++gcalls;
    const long* s = static_cast<const long*>(ap); gv0 = s[0]; gv1 = s[1];
}
int main() {
    unsigned short b[4]; unsigned short fmt[2] = {};
    snprintf16__8VIStringPUsiPCUse(b, 77, fmt, 1234L, -5L);
    if (gcalls != 1 || gbuf != b || gsize != 77 || gfmt != fmt || gv0 != 1234 || gv1 != -5) return 1;
    snprintf16__8VIStringPUsiPCUse(b + 1, -3, fmt + 1, 0L, 9L);
    if (gcalls != 2 || gbuf != b + 1 || gsize != -3 || gfmt != fmt + 1 || gv0 != 0 || gv1 != 9) return 2;
    return 0;
}
#else
int main() { return 0; }
#endif
