#include <cstring>
#include <cstdint>
extern "C" unsigned long glistp, gstart;
extern "C" int GLISTSIZE;
extern "C" signed char supersampleMode;
unsigned long glistp, gstart;
int GLISTSIZE;
signed char supersampleMode;
static void* got_packet; static int got_list, calls; static unsigned long glistp_at_call;
extern "C" void* drawAddToList__FPviP8_textureP11_drawRecordi(void* p, int list, void* t, void* r, int f) {
    got_packet = p; got_list = list; calls++; glistp_at_call = glistp;
    if (t || r || f) got_list = -12345;
    return 0;
}
extern "C" void rectangleDraw__FiiiiiUi(int, int, int, int, int, unsigned);

alignas(16) static unsigned char buf[512];
static uint64_t q(int i) { uint64_t v; memcpy(&v, buf + i * 8, 8); return v; }
static uint32_t w(int i) { uint32_t v; memcpy(&v, buf + i * 4, 4); return v; }

static uint64_t corner(long long x, long long y) { return (uint64_t)x | ((uint64_t)y << 16) | 0xa00000000ull; }

static int run(int mode, int x0, int y0, int x1, int y1) {
    memset(buf, 0xcc, sizeof buf); calls = 0;
    glistp = (unsigned long)buf; gstart = glistp - 800; GLISTSIZE = 100 + 3000 + 100 - 0; // used = 100 units
    supersampleMode = mode;
    rectangleDraw__FiiiiiUi(x0, y0, x1, y1, 7, 0x80ff4020u);
    return 0;
}

int main() {
    run(0, 10, 20, 100, 200);
    if (calls != 1 || got_packet != buf || got_list != 7) return 1;
    if (glistp != (unsigned long)buf + 128 || glistp_at_call != glistp) return 2;
    if (q(0) != 0x60000007ull) return 3;
    if (w(2) != 0x11000000u || w(3) != 0x50000007u) return 4;
    if (q(2) != (0x1000000000008000ull | 6) || q(3) != 0xeull) return 5;
    if (w(8) != 0x80ff4020u || w(9) != 0 || q(5) != 1) return 6;
    if (q(6) != 0x44 || q(7) != 0x42 || q(8) != 0x46 || q(9) != 0 || q(10) != 0x31001 || q(11) != 0x47) return 7;
    if (q(12) != corner((10 << 4) + 27648, (20 << 4) + 29184) || q(13) != 5) return 8;
    if (q(14) != corner((100 << 4) + 27648, (200 << 4) + 29184) || q(15) != 5) return 9;
    if (q(16) != 0xccccccccccccccccull) return 10;  // nothing written past the packet
    run(1, 10, 20, 100, 200);
    if (q(12) != corner((10 << 5) + 22528, (20 << 4) + 29184) || q(14) != corner((100 << 5) + 22528, (200 << 4) + 29184)) return 11;
    // Negative coordinates keep their sign extension through the 64-bit OR.
    run(0, -100, -50, 5, 6);
    if (q(12) != (uint64_t)((long long)((-100 << 4) + 27648) | ((long long)((-50 << 4) + 29184) << 16) | 0xa00000000ll)) return 12;
    // y negative after scaling.
    run(0, 0, -2000, 1, 1);
    if (q(12) != (uint64_t)((long long)(27648) | ((long long)((-2000 << 4) + 29184) << 16) | 0xa00000000ll)) return 13;
    // Too little room: nothing is touched.
    memset(buf, 0xcc, sizeof buf); calls = 0;
    glistp = (unsigned long)buf; gstart = glistp - 800; GLISTSIZE = 100 + 2999;
    rectangleDraw__FiiiiiUi(1, 2, 3, 4, 5, 6);
    if (calls || glistp != (unsigned long)buf || q(0) != 0xccccccccccccccccull) return 14;
    GLISTSIZE = 100 + 3000;
    rectangleDraw__FiiiiiUi(1, 2, 3, 4, 5, 6);
    if (calls != 1) return 15;
    return 0;
}
