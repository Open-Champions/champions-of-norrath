// Host check for textureInitTexture, built with -DNORRATH_HOST_TEXTURE: conversion, the early return, the
// single-word form, and the directory form with its size total. Addresses are offsets in a test arena.
#include <cstdio>
#include <cstring>
unsigned char* norrath_texture_arena;
extern "C" void textureInitTexture__FP12_worldHeaderP8_texturePUcPc(void*, void*, const unsigned char*, const unsigned char*);
static int fails;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
static unsigned conv(const unsigned char* map, unsigned base, unsigned v) {   // independent model
    return base + (unsigned)map[v >> 17] * 0x20000u + (v & 0x1ffff);
}
int main() {
    static unsigned char arena[1 << 20];
    norrath_texture_arena = arena;
    unsigned char map[8] = {0, 3, 1, 2, 0, 0, 0, 0};
    unsigned base = 0x1000;
    unsigned char t[48];
    auto word = [&](unsigned char* b, int off) { unsigned v; memcpy(&v, b + off, 4); return v; };
    auto setw = [&](unsigned char* b, int off, unsigned v) { memcpy(b + off, &v, 4); };
    auto seth = [&](unsigned char* b, int off, unsigned v) { unsigned short h = (unsigned short)v; memcpy(b + off, &h, 2); };
    // already initialised
    memset(t, 0x11, sizeof t);
    setw(t, 28, 5);
    textureInitTexture__FP12_worldHeaderP8_texturePUcPc(0, t, map, arena + base);
    CHECK(word(t, 28) == 5 && word(t, 16) == 0x11111111u);
    // single form (bit 0): +16 converted, the word it names converted in place
    memset(t, 0, sizeof t);
    unsigned stored = 0x00020040;          // block 1 offset 0x40
    setw(t, 16, stored); seth(t, 8, 1);
    unsigned at = base + 0x3 * 0x20000 + 0x40;   // map[1] == 3 ... but that is beyond the arena; use small map instead
    (void)at;
    unsigned char map2[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    stored = 0x40;
    setw(t, 16, stored);
    unsigned target = conv(map2, base, stored);
    setw(arena + target, 0, 0x80);
    textureInitTexture__FP12_worldHeaderP8_texturePUcPc(0, t, map2, arena + base);
    CHECK(word(t, 16) == target && word(t, 28) == 0xffffffffu);
    CHECK(word(arena + target, 0) == conv(map2, base, 0x80));
    unsigned short h10; memcpy(&h10, t + 10, 2); CHECK(h10 == 0xa7fb);
    // directory form
    memset(t, 0, sizeof t); memset(arena, 0, sizeof arena);
    seth(t, 8, 0x100); setw(t, 16, 0x200); seth(t, 6, 0xffff);
    unsigned d = base + 0x200;
    setw(arena + d, 0, 0x300);                       // first word
    unsigned char* g = arena + d + 4;
    g[0] = 1; g[1] = 2; g[2] = 3; g[3] = 4;          // x 1..3, y 2..4: 9 words
    for (int i = 0; i < 9; ++i) setw(g + 4, 4 * i, 0x400 + 4 * i);
    unsigned char* g2 = g + 4 + 36;
    g2[0] = 0; g2[1] = 5; g2[2] = 0; g2[3] = 4;      // y empty (b3 < b1): no words
    unsigned char* g3 = g2 + 4;
    g3[0] = 2; g3[1] = 0; g3[2] = 1; g3[3] = 1;      // x empty (b2 < b0): no words
    unsigned char* g4 = g3 + 4;
    g4[0] = 0; g4[1] = 0; g4[2] = 0; g4[3] = 0;      // single word
    setw(g4 + 4, 0, 0x500);
    g4[8] = 0xff;
    textureInitTexture__FP12_worldHeaderP8_texturePUcPc(0, t, map2, arena + base);
    CHECK(word(t, 16) == d);
    CHECK(word(arena + d, 0) == conv(map2, base, 0x300));
    for (int i = 0; i < 9; ++i) CHECK(word(g + 4, 4 * i) == conv(map2, base, 0x400 + 4 * i));
    CHECK(word(g4 + 4, 0) == conv(map2, base, 0x500));
    auto size = [](int b0, int b1, int b2, int b3) { return (((b3 - b1) << 11) + 2048) * (b2 - b0 + 1) / 128 + 6; };
    unsigned short h6; memcpy(&h6, t + 6, 2);
    CHECK(h6 == (unsigned short)(72 + size(1, 2, 3, 4) + size(0, 5, 0, 4) + size(2, 0, 1, 1) + size(0, 0, 0, 0)));
    // neither flag: only the pointer is converted
    memset(t, 0, sizeof t); setw(t, 16, 0x40); seth(t, 8, 0x200);
    textureInitTexture__FP12_worldHeaderP8_texturePUcPc(0, t, map2, arena + base);
    CHECK(word(t, 16) == conv(map2, base, 0x40) && word(t, 28) == 0xffffffffu);
    if (fails) return 1;
    puts("textureInitTexture ok");
    return 0;
}
