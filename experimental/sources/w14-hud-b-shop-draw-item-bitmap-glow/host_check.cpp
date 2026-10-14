// Host check for shopDrawItemBitmapGlow__FP8_textureii. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
// The soft-float routines are stood in by IEEE arithmetic; the draw call is recorded.
#include "shop_draw_item_bitmap_glow.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

struct Call { int x, y, a3; unsigned color; int a5, a6, a7, a8, a9; _texture* texture; };
static std::vector<Call> calls;
static double asDouble(Double v) { double d; std::memcpy(&d, &v, 8); return d; }
static Double fromDouble(double d) { Double v; std::memcpy(&v, &d, 8); return v; }
extern "C" {
float expf(float value) { return static_cast<float>(std::exp(static_cast<double>(value))); }
Double fptodp(float value) { return fromDouble(value); }
Double dpmul(Double a, Double b) { return fromDouble(asDouble(a) * asDouble(b)); }
int dptoli(Double value) { return static_cast<int>(asDouble(value)); }
void textureDrawColorSpecialHalo__FP8_textureiiiUiiiiii(_texture* texture, int x, int y, int a3, unsigned color,
                                                        int a5, int a6, int a7, int a8, int a9) {
    calls.push_back({x, y, a3, color, a5, a6, a7, a8, a9, texture});
}
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

int main() {
    _texture* texture = reinterpret_cast<_texture*>(0x1234);
    shopDrawItemBitmapGlow__FP8_textureii(texture, 100, 200);
    CHECK(calls.size() == 100);
    // order: dy outer, dx inner, both from -5
    for (int i = 0; i < 100; ++i) {
        int dy = i / 10 - 5, dx = i % 10 - 5;
        const Call& c = calls[i];
        CHECK(c.texture == texture && c.x == 100 + dx && c.y == 200 + dy);
        CHECK(c.a3 == 5 && c.a5 == 50 && c.a6 == 0 && c.a7 == 102 && c.a8 == 640 && c.a9 == 600);
        int d = static_cast<int>(std::sqrt(static_cast<float>(dx * dx + dy * dy)));
        double k = static_cast<float>(-(d * d)) * 0.064f;
        int a = static_cast<int>(static_cast<double>(std::exp(k)) * 13.7229999999999990);
        CHECK(c.color == (0x80000000u | (static_cast<unsigned>(a) << 8)));
    }
    // the centre has distance 0: 13.72 truncates to 13; its colour word is 0x80000d00
    CHECK(calls[55].x == 100 && calls[55].y == 200 && calls[55].color == 0x80000d00u);
    // cells at distance 1 (4 neighbours) share one colour and are darker or equal
    CHECK(calls[54].color == calls[56].color && calls[45].color == calls[65].color && calls[54].color == calls[45].color);
    // the colour is 0x80000000 | (alpha << 8) with the alpha limited to 0..255
    for (const Call& c : calls) CHECK((c.color & 0xffff00ffu) == 0x80000000u && ((c.color >> 8) & 0xff) <= 13);
    // a second call appends 100 more with shifted coordinates
    shopDrawItemBitmapGlow__FP8_textureii(texture, -3, 4);
    CHECK(calls.size() == 200 && calls[100].x == -8 && calls[100].y == -1 && calls[199].x == 1 && calls[199].y == 8);
    std::puts("ok");
    return 0;
}
