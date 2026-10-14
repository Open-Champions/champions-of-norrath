// Host check for subfontPrintu, built with -DNORRATH_HOST_SUBFONT: the entry limits, glyph lookup, kerning,
// the grey glyph colour change, and the three drawing layouts. Stored addresses are arena offsets.
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>
unsigned char* norrath_subfont_arena;
struct Packet { unsigned cursor; };
signed char supersampleMode;
int engineFullFrameMode, fontCharAlpha, lastFontCharAlpha, nmChars;
unsigned char* curFont;
unsigned char lastFontColor[4];
extern "C" { Packet packet __asm__("p"); }
static int field_value;
static std::vector<std::vector<long long>> colors, draws;
extern "C" int engineGetField__Fv() { return field_value; }
extern "C" void fontColor__FUiii(unsigned c, int a, int f) { colors.push_back({(long long)c, a, f}); }
extern "C" void drawTexture__FR6Packetiiiiiiiii(Packet&, int a, int b, int c, int d, int e, int f, int g, int h, int i) {
    draws.push_back({a, b, c, d, e, f, g, h, i});
}
extern "C" void subfontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int limit);
static int fails;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
static void seth(unsigned char* b, unsigned off, int v) { unsigned short h = (unsigned short)v; memcpy(b + off, &h, 2); }
static void setw(unsigned char* b, unsigned off, unsigned v) { memcpy(b + off, &v, 4); }
static long long getq(const unsigned char* b, unsigned off) { long long v; memcpy(&v, b + off, 8); return v; }
static long long pk(int hi, int lo) { return (long long)(int)((unsigned)hi << 16) | (long long)lo; }
int main() {
    static unsigned char arena[1 << 18];
    norrath_subfont_arena = arena;
    const unsigned F = 0x1000, G = 0x2000, K = 0x3000, T = 0x3800, BUF = 0x8000;
    unsigned char* font = arena + F;
    curFont = font;
    seth(font, 0, 130); setw(font, 8, G); setw(font, 12, K); setw(font, 16, T);
    seth(arena, T + 2, 77);
    for (int c = 0; c < 256; ++c) seth(font, 20 + 2 * c, c < 128 ? c - 31 : -1);   // code c maps to glyph c - 31
    seth(font, 110, 14);                                                          // fallback for unmapped codes
    for (int i = 0; i < 130; ++i) {
        unsigned char* g = arena + G + 16 * i;
        seth(g, 0, 256 + i); seth(g, 2, i); seth(g, 4, i + 5); seth(g, 6, 2 * i); seth(g, 8, 2 * i + 3);
        seth(g, 10, 4); seth(g, 12, 10 + i % 3); seth(g, 14, -1);
    }
    // kerning: glyph 34 ('A' + ... ) has entries (prev 33 adj -6) then (prev 40 adj 9), both for glyph 34
    seth(arena, G + 16 * 34 + 14, 2);
    seth(arena, K + 12, 33); seth(arena, K + 14, 34); seth(arena, K + 16, -6);
    seth(arena, K + 18, 40); seth(arena, K + 20, 34); seth(arena, K + 22, 9);
    seth(arena, K + 24, 1); seth(arena, K + 26, 35); seth(arena, K + 28, 3);
    fontCharAlpha = -1; lastFontCharAlpha = 5; lastFontColor[0] = 1; lastFontColor[1] = 2; lastFontColor[2] = 3; lastFontColor[3] = 0x44;
    unsigned short text[8] = {0};
    // limits
    text[0] = 'A'; packet.cursor = BUF; supersampleMode = 0; engineFullFrameMode = 0;
    subfontPrintu__FiiPUsi(0, 801, text, 1);
    subfontPrintu__FiiPUsi(0, -101, text, 1);
    subfontPrintu__FiiPUsi(0, 5, text, 0);
    CHECK(packet.cursor == BUF && nmChars == 0);
    // plain layout, glyph 'A'-31 = 34, no previous glyph so no kerning
    subfontPrintu__FiiPUsi(10, 5, text, 5);
    {
        int u0 = 34 << 4, u1 = 39 << 4, v0 = 68 << 4, v1 = 71 << 4, h = 4 << 4, cx = (10 << 4) + 27648, cy = (5 << 4) + 29184;
        CHECK(packet.cursor == BUF + 32 && nmChars == 1);
        CHECK(getq(arena, BUF) == pk(v0 + 8, u0 + 8));
        CHECK(getq(arena, BUF + 8) == pk(cy + h, cx));
        CHECK(getq(arena, BUF + 16) == pk(v1 + 8, u1 + 8));
        CHECK(getq(arena, BUF + 24) == pk(cy + h + (v1 - v0), cx + u1 - u0));
    }
    // newline between two letters: previous glyph becomes 2047 (no kerning); second letter draws after the first
    packet.cursor = BUF; nmChars = 0;
    text[0] = 'A'; text[1] = 10; text[2] = 'B'; text[3] = 0;
    subfontPrintu__FiiPUsi(0, 0, text, 9);
    CHECK(nmChars == 2 && packet.cursor == BUF + 64);
    // kerning: 'A' then 'B' glyph 35; glyph 35 has no kern; use A A: glyph 34 after 34 has no match; set prev 33 via '@'
    packet.cursor = BUF; nmChars = 0;
    text[0] = '@'; text[1] = 'A'; text[2] = 0;           // glyph 33 then glyph 34 with kerning entry (33 -> -6)
    subfontPrintu__FiiPUsi(0, 0, text, 9);
    {
        int cx0 = 27648, adv33 = (10 + 33 % 3) << 4, cx1 = cx0 + adv33 - (6 << 3);
        int u0 = 34 << 4, u1 = 39 << 4, v0 = 68 << 4, v1 = 71 << 4, h = 4 << 4;
        CHECK(nmChars == 2);
        CHECK(getq(arena, BUF + 32 + 8) == pk(29184 + h, cx1));
        (void)u0; (void)u1; (void)v0; (void)v1;
    }
    // second kerning entry of the chain: prev 40 -> +9
    packet.cursor = BUF; nmChars = 0;
    text[0] = 'G'; text[1] = 'A'; text[2] = 0;           // 'G' = 71 -> glyph 40
    subfontPrintu__FiiPUsi(0, 0, text, 9);
    CHECK(getq(arena, BUF + 32 + 8) == pk(29184 + (4 << 4), 27648 + ((10 + 40 % 3) << 4) + (9 << 3)));
    // supersample, even y, field low bit clear: offset = ((77 >> 1) << 4) - 4
    packet.cursor = BUF; nmChars = 0; supersampleMode = 1; field_value = 0;
    text[0] = 'A'; text[1] = 0;
    subfontPrintu__FiiPUsi(3, 2, text, 9);
    {
        int off = ((77 >> 1) << 4) - 4;
        int u0 = 34 << 4, u1 = 39 << 4, v0 = 68 * 8, v1 = 71 * 8 - 0, h = 4 << 3;
        v1 = (2 * 34 + 3) << 3; v0 = (2 * 34) << 3;
        int cx = (3 << 5) + 22528, cy = (2 << 4) + 29184;
        CHECK(getq(arena, BUF) == pk(off + v0 + 4, u0 + 4));
        CHECK(getq(arena, BUF + 8) == pk(cy + (h << 1), cx));
        CHECK(getq(arena, BUF + 16) == pk(off + v1 + 4, u1 + 4));
        CHECK(getq(arena, BUF + 24) == pk(cy + ((h + v1 - v0) << 1), cx + ((u1 - u0) << 1)));
        CHECK(packet.cursor == BUF + 32);
    }
    // full-frame mode goes through drawTexture
    draws.clear(); engineFullFrameMode = 1; packet.cursor = BUF; field_value = 1;
    subfontPrintu__FiiPUsi(3, 2, text, 9);
    CHECK(draws.size() == 1 && draws[0][0] == 77 && packet.cursor == BUF);
    // grey glyph (index 91..104) sets a colour and it is restored at the end
    engineFullFrameMode = 0; colors.clear();
    text[0] = 91 + 31; text[1] = 0;                       // code 122 -> glyph 91
    subfontPrintu__FiiPUsi(0, 0, text, 9);
    CHECK(colors.size() == 2);
    CHECK(colors[0][0] == (long long)(((unsigned)0x44 << 24) | 0x606060u) && colors[0][1] == -1 && colors[0][2] == 1);
    CHECK(colors[1][0] == (long long)0x44030201u && colors[1][1] == 5 && colors[1][2] == 0);
    if (fails) return 1;
    puts("subfontPrintu ok");
    return 0;
}
