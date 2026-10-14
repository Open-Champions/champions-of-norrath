// Host check only; not a build input.
#include "texture_draw_color_special_halo.cpp"
#include <cstdio>
#include <cstring>
unsigned long glistp, gstart;
int GLISTSIZE, engineScreenSlide, engineScissory1, engineScissory2, fieldValue;
signed char supersampleMode;
alignas(16) static unsigned char list[8192];
static TextureHeader tex = {64, 32};
static int tex_calls, add_calls, add_list, dt_x, dt_y, dt_args[4];
static DrawPacketRecord dt_record;
extern "C" int engineGetField__Fv() { return fieldValue; }
extern "C" void drawTexture__FR6PacketP8_textureiiiiii(DrawPacketRecord* p, TextureHeader*, int a, int b, int c, int d, int e, int f) {
    ++tex_calls; dt_record = *p; dt_x = a; dt_y = b; dt_args[0] = c; dt_args[1] = d; dt_args[2] = e; dt_args[3] = f;
    std::memset(p->cursor, 0xab, 0x30); p->cursor += 0x30;
}
extern "C" void* drawAddToList__FPviP8_textureP11_drawRecordi(void* packet, int l, TextureHeader*, void* r, int f) {
    ++add_calls; add_list = l; (void)packet; (void)r; (void)f; return 0;
}
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d\n", __LINE__); return 1; } } while (0)
static unsigned long long q(const unsigned char* p) { unsigned long long v; std::memcpy(&v, p, 8); return v; }
static unsigned w(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static void reset(int free_units) {
    std::memset(list, 0x5a, sizeof list);
    gstart = (unsigned long)list; glistp = (unsigned long)(list + 64); GLISTSIZE = (64 >> 3) + free_units;
    engineScissory1 = 0x11; engineScissory2 = 0x1bf; tex_calls = add_calls = 0;
}
int main() {
    reset(2999); supersampleMode = 0;
    textureDrawColorSpecialHalo__FP8_textureiiiUiiiiii(&tex, 1, 2, 3, 0x11223344, 5, 0, 0, 640, 447);
    CHECK(glistp == (unsigned long)(list + 64) && tex_calls == 0 && add_calls == 0 && list[64] == 0x5a);
    reset(3000); supersampleMode = 0; engineScreenSlide = 6;
    textureDrawColorSpecialHalo__FP8_textureiiiUiiiiii(&tex, 11, 22, 33, 0x11223344, 5, -4, 20, 700, 460);
    unsigned char* P = list + 64;
    CHECK(tex_calls == 1 && dt_x == 11 && dt_y == 22 && dt_args[0] == 0 && dt_args[1] == 0 && dt_args[2] == 64 && dt_args[3] == 32);
    CHECK(dt_record.start == P && dt_record.gifTag == P + 16 && dt_record.vifWord == P + 12 && dt_record.cursor == P + 208);
    CHECK(w(P + 8) == 0x11000000u && w(P + 12) == 0x50000011u && q(P + 16) == (0x1000000000008000ull | 16) && q(P + 24) == 0xe);
    CHECK(q(P + 32) == 0 && q(P + 40) == 0x3f && q(P + 48) == 0 && q(P + 56) == 6 && q(P + 64) == 1 && q(P + 72) == 20);
    CHECK(w(P + 80) == 0x80808080u && w(P + 84) == 0 && q(P + 88) == 1 && q(P + 96) == 0x48 && q(P + 104) == 0x42);
    CHECK(q(P + 112) == 0x223344 && q(P + 120) == 0x3d && q(P + 128) == 0 && q(P + 136) == 10 && q(P + 144) == 0x176 && q(P + 152) == 0);
    CHECK(q(P + 160) == 0x1212303012123030ull && q(P + 168) == 0x44 && q(P + 176) == 0x3005d && q(P + 184) == 0x47);
    CHECK(q(P + 192) == (0ull | (639ull << 16) | (14ull << 32) | (447ull << 48)) && q(P + 200) == 64);
    CHECK(q(P + 256) == 0x3131757531317575ull && q(P + 264) == 0x44);
    CHECK(q(P + 272) == (0x04ff0000ull | (0x11ull << 32) | (0x1bfull << 48)) && q(P + 280) == 64);
    CHECK(q(P) == 0x60000011ull && glistp == (unsigned long)(P + 288) && add_calls == 1 && add_list == 33);
    // supersample: x doubled, y1 halved with the field bit
    reset(3000); supersampleMode = 1; engineScreenSlide = 0; fieldValue = 1;
    textureDrawColorSpecialHalo__FP8_textureiiiUiiiiii(&tex, 1, 2, 3, 0xff000000, -2, 3, 4, 100, 201);
    P = list + 64;
    CHECK(q(P + 176) == (unsigned long long)(long long)(int)((-32) | 0x3000d) && q(P + 112) == 0);
    CHECK(q(P + 192) == (6ull | (199ull << 16) | (4ull << 32) | (201ull << 48)));
    fieldValue = 0; reset(3000);
    textureDrawColorSpecialHalo__FP8_textureiiiUiiiiii(&tex, 1, 2, 3, 0, 0, 3, 4, 100, 201);
    CHECK(q(list + 64 + 192) == (6ull | (199ull << 16) | (4ull << 32) | (199ull << 48)));
    std::puts("ok");
    return 0;
}
