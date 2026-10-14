// Host contract for resetScissor__Fv. Not an EE plan input.
#include <cstdint>
#include <cstdio>
#include <cstring>
extern "C" unsigned long glistp;
extern "C" volatile signed char coreLoadingScreenRunning;
unsigned long glistp;
volatile signed char coreLoadingScreenRunning;
static void* got_packet; static int got_list, calls; static unsigned long glistp_at_call;
extern "C" void* drawAddToList__FPviP8_textureP11_drawRecordi(void* p, int list, void* t, void* r, int f) {
    got_packet = p; got_list = list; calls++; glistp_at_call = glistp;
    if (t || r || f) got_list = -12345;
    return 0;
}
extern "C" void resetScissor__Fv();
alignas(16) static unsigned char buf[256];
static uint64_t q(int i) { uint64_t v; memcpy(&v, buf + i * 8, 8); return v; }
static uint32_t w(int i) { uint32_t v; memcpy(&v, buf + i * 4, 4); return v; }
int main() {
    memset(buf, 0xcc, sizeof buf);
    glistp = (unsigned long)buf;
    coreLoadingScreenRunning = 1;
    resetScissor__Fv();
    if (calls != 0 || glistp != (unsigned long)buf || buf[0] != 0xcc) return 1;
    coreLoadingScreenRunning = 0;
    resetScissor__Fv();
    if (calls != 1 || got_packet != buf || got_list != 4) return 2;
    if (glistp != (unsigned long)buf + 64 || glistp_at_call != glistp) return 3;
    if (q(0) != 0x60000003ull) return 4;
    if (w(2) != 0x11000000u || w(3) != 0x50000003u) return 5;
    if (q(2) != (0x1000000000008000ull | 2) || q(3) != 0xeull) return 6;
    if (q(4) != 0x1bf000004ff0000ull || q(5) != 0x40 || q(6) != 0x1bf000004ff0000ull || q(7) != 0x41) return 7;
    if (buf[64] != 0xcc) return 8;
    std::printf("resetScissor contract passed\n");
    return 0;
}
