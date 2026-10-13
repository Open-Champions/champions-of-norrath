#include <cstdio>
#include <cstring>
extern "C" {
unsigned char* glistp;
unsigned char* gstart;
int GLISTSIZE;
}
static int calls = 0;
static void* seenPacket; static int seenOrder; static void* seenTexture; static void* seenRecord; static int seenFlag;
extern "C" void drawAddToList__FPviP8_textureP11_drawRecordi(void* p, int order, struct Texture* t, void* r, int f) {
    calls++; seenPacket = p; seenOrder = order; seenTexture = t; seenRecord = r; seenFlag = f;
}
#include "texture_draw_float_inner.cpp"

int main() {
    int bad = 0;
    alignas(16) static unsigned long long buf[400];
    short tex[2] = {64, 32};
    gstart = reinterpret_cast<unsigned char*>(buf);
    glistp = gstart + 16;
    GLISTSIZE = 3000 + 2;
    std::memset(buf, 0x77, sizeof buf);
    _textureDrawFloat__FP8_textureffiUiff(reinterpret_cast<Texture*>(tex), 10.0f, 20.0f, 5, 0xa1b2c3d4u, 1.0f, 0.5f);
    unsigned long long* s = buf + 2;
    if (calls != 1 || seenPacket != s || seenOrder != 5 || seenTexture != tex || seenRecord || seenFlag != 1) bad++;
    if (glistp != reinterpret_cast<unsigned char*>(s + 26)) bad++;
    if (s[0] != 0x60000000ull + 12) {   // id rewritten from 1 to 6, size 12
        std::printf("tag %llx\n", s[0]); bad++;
    }
    if (reinterpret_cast<unsigned*>(s)[2] != 0x11000000u || reinterpret_cast<unsigned*>(s)[3] != 0x5000000cu) bad++;
    if (s[2] != (0x1000000000008000ull | 11) || s[3] != 14) bad++;
    if (s[10] != 0xa1b2c3d4ull || s[11] != 1) bad++;
    if (s[18] != 0x80008ull || s[19] != 3 || s[21] != 5 || s[23] != 3 || s[25] != 5) bad++;
    const long long z = 0xa00000000ll;
    if (s[20] != (unsigned long long)(((29184 + 320) << 16 | (22528 + 320)) | z)) { std::printf("p0 %llx\n", s[20]); bad++; }
    if (s[22] != (unsigned long long)((32 << 20) | (64 << 4))) bad++;
    // 32 rows * 16 * 0.5 = 256 below, 64 columns * 32 * 1.0 = 2048 right
    if (s[24] != (unsigned long long)((((29184 + 320 + 256) << 16) | (22528 + 320 + 2048)) | z)) { std::printf("p1 %llx\n", s[24]); bad++; }
    // too little room: nothing written, nothing queued
    std::memset(buf, 0x77, sizeof buf);
    glistp = gstart + 16; GLISTSIZE = 3000 + 1; calls = 0;
    _textureDrawFloat__FP8_textureffiUiff(reinterpret_cast<Texture*>(tex), 10.0f, 20.0f, 5, 0u, 1.0f, 1.0f);
    if (calls != 0 || glistp != gstart + 16 || buf[2] != 0x7777777777777777ull) bad++;
    if (!bad) std::puts("_textureDrawFloat passed");
    return bad;
}
