#include "texture_draw_super_color.cpp"

#include <cstdio>
#include <cstring>

unsigned long glistp, gstart;
int GLISTSIZE, engineScreenSlide, engineScissory1, engineScissory2, fieldValue;
signed char supersampleMode;
alignas(16) static unsigned char list[8192];
static TextureHeader tex = {64, 32};

static int tex_calls, add_calls, swap_calls;
static int dt_args[6], dt_x, dt_y, add_list;
static DrawPacketRecord dt_record;
static void* add_packet;
static TextureHeader* add_tex;
static void* add_record;
static int add_flags;

extern "C" int engineGetField__Fv() { return fieldValue; }
extern "C" void swap__H1Zi_RX01T0_v(int& a, int& b) { ++swap_calls; int t = a; a = b; b = t; }
extern "C" void drawTexture__FR6PacketP8_textureiiiiii(DrawPacketRecord* p, TextureHeader*, int a, int b, int c, int d,
                                                       int e, int f) {
    ++tex_calls;
    dt_record = *p;
    dt_x = a; dt_y = b;
    dt_args[0] = c; dt_args[1] = d; dt_args[2] = e; dt_args[3] = f;
    std::memset(p->cursor, 0xab, 0x30);
    p->cursor += 0x30;
}
extern "C" void* drawAddToList__FPviP8_textureP11_drawRecordi(void* packet, int l, TextureHeader* t, void* r, int f) {
    ++add_calls; add_packet = packet; add_list = l; add_tex = t; add_record = r; add_flags = f;
    return 0;
}

#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d\n", __LINE__); return 1; } } while (0)
static unsigned long long q(const unsigned char* p) { unsigned long long v; std::memcpy(&v, p, 8); return v; }
static unsigned w(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }

static void reset(int free_units) {
    std::memset(list, 0x5a, sizeof list);
    gstart = (unsigned long)list;
    glistp = (unsigned long)(list + 64);
    GLISTSIZE = (64 >> 3) + free_units;
    engineScissory1 = 0x11; engineScissory2 = 0x1bf;
    tex_calls = add_calls = swap_calls = 0;
}

int main() {
    // Too little room: nothing is written or called.
    reset(2999); supersampleMode = 0;
    textureDrawSuperColor__FP8_textureiiiUiiiiicc(&tex, 1, 2, 3, 0x11223344, 0, 0, 640, 447, 0, 0);
    CHECK(glistp == (unsigned long)(list + 64) && tex_calls == 0 && add_calls == 0 && list[64] == 0x5a);

    // Plain clip, no flips.
    reset(3000); supersampleMode = 0; engineScreenSlide = 6;
    textureDrawSuperColor__FP8_textureiiiUiiiiicc(&tex, 11, 22, 33, 0x11223344, -4, 20, 700, 460, 0, 0);
    unsigned char* P = list + 64;
    CHECK(tex_calls == 1 && swap_calls == 0 && dt_x == 11 && dt_y == 22);
    CHECK(dt_args[0] == 0 && dt_args[1] == 0 && dt_args[2] == 64 && dt_args[3] == 32);
    CHECK(dt_record.start == P && dt_record.gifTag == P + 16 && dt_record.vifWord == P + 12);
    CHECK(dt_record.cursor == P + 160);
    CHECK(w(P + 8) == 0x11000000u && w(P + 12) == 0x5000000du);
    CHECK(q(P + 16) == (0x1000000000008000ull | 12) && q(P + 24) == 0xe);
    CHECK(q(P + 32) == 0 && q(P + 40) == 0x3f && q(P + 48) == 0 && q(P + 56) == 6);
    CHECK(q(P + 64) == 1 && q(P + 72) == 20);
    CHECK(w(P + 80) == 0x11223344u && w(P + 84) == 0 && q(P + 88) == 1);
    CHECK(q(P + 96) == 0x44 && q(P + 104) == 0x42 && q(P + 112) == 342 && q(P + 120) == 0);
    CHECK(q(P + 128) == 0x3001d && q(P + 136) == 0x47);
    // x0 -4 -> 0, y0 20 - 6, x1 640, y1 454 -> 447
    CHECK(q(P + 144) == (0ull | (640ull << 16) | (14ull << 32) | (447ull << 48)) && q(P + 152) == 64);
    CHECK(q(P + 208) == (0x04ff0000ull | (0x11ull << 32) | (0x1bfull << 48)) && q(P + 216) == 64);
    CHECK(q(P) == 0x6000000dull);
    CHECK(w(P + 12) == 0x5000000du);
    CHECK(glistp == (unsigned long)(P + 224));
    CHECK(add_calls == 1 && add_packet == P && add_list == 33 && add_tex == &tex && add_record == 0 && add_flags == 1);

    // Supersampling with the odd field; flips swap the corner pairs.
    reset(3000); supersampleMode = 1; fieldValue = 3; engineScreenSlide = 0;
    textureDrawSuperColor__FP8_textureiiiUiiiiicc(&tex, 5, 6, 7, 0xff, 10, 30, 100, 301, 1, -1);
    P = list + 64;
    CHECK(swap_calls == 2 && dt_args[0] == 64 && dt_args[1] == 32 && dt_args[2] == 0 && dt_args[3] == 0);
    // y1 301: (301 >> 1) + 1 * 1 = 151, doubled minus one = 301
    CHECK(q(P + 144) == (20ull | (199ull << 16) | (30ull << 32) | (301ull << 48)));
    // even bottom row: no field correction
    reset(3000); supersampleMode = 1; fieldValue = 1;
    textureDrawSuperColor__FP8_textureiiiUiiiiicc(&tex, 5, 6, 7, 0xff, 10, 30, 100, 300, 0, 1);
    P = list + 64;
    CHECK(swap_calls == 1 && dt_args[0] == 0 && dt_args[1] == 32 && dt_args[2] == 64 && dt_args[3] == 0);
    CHECK(q(P + 144) == (20ull | (199ull << 16) | (30ull << 32) | (299ull << 48)));
    reset(3000); supersampleMode = 1; fieldValue = 0;
    textureDrawSuperColor__FP8_textureiiiUiiiiicc(&tex, 5, 6, 7, 0xff, 10, 30, 100, 301, 0, 0);
    P = list + 64;
    CHECK(q(P + 144) == (20ull | (199ull << 16) | (30ull << 32) | (299ull << 48)));
    std::puts("ok");
    return 0;
}
