#include <cstdio>
#include <cstring>
#include <vector>
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
extern "C" {
unsigned char DynamicLightArray[16 * 68];
int frameNm, eRandState;
unsigned char* lightHaloTexture;
static unsigned lightNumber;
static int addCalls, spriteCalls, noiseArg;
static unsigned char* addedLight;
static float spriteArgs[16];
static int spriteInts[8];
unsigned getDynamicLightNumber__FP13PolysetHeader(unsigned char*) { return lightNumber; }
void lightAdd__FP5Light(unsigned char* l) { ++addCalls; addedLight = l; }
float ef1Noise__Fi(int index) { noiseArg = index; return 0.5f; }
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(float* p, float a, float b, int c, int d, int e, int f, unsigned char* tex, short g, int h) {
    ++spriteCalls; spriteArgs[0] = p[0]; spriteArgs[1] = p[1]; spriteArgs[2] = p[2]; spriteArgs[3] = a; spriteArgs[4] = b;
    spriteInts[0] = c; spriteInts[1] = d; spriteInts[2] = e; spriteInts[3] = f; spriteInts[4] = g; spriteInts[5] = h;
    spriteInts[6] = tex == lightHaloTexture;
}
void drawWorldDynamicLight__FP13PolysetHeader(unsigned char* ph);
}
int main() {
    unsigned char* ph = arena_at(0x100);
    unsigned char* h = arena_at(0x1000);
    unsigned char* light = DynamicLightArray + 3 * 68 + 8;
    auto arm = [&](unsigned mode) {
        std::memset(DynamicLightArray, 0, sizeof DynamicLightArray);
        std::memset(ph, 0, 64); std::memset(h, 0, 64);
        sw_(ph, norrath_host_address(h));
        sw_(ph + 40, 400); sw_(ph + 44, (unsigned)-40); sw_(ph + 48, 8);
        sh_(h + 20, mode); sh_(h + 16, 50); sh_(h + 18, 150); sh_(h + 22, 10);
        sh_(h + 24, 1); sh_(h + 26, (unsigned)-2); sh_(h + 28, 3); sh_(h + 30, 4);
        sf_(light + 28, 77.0f);
        lightNumber = 3; frameNm = 1000; addCalls = spriteCalls = 0; lightHaloTexture = nullptr;
    };
    // Number >= 16 does nothing at all.
    arm(0); lightNumber = 16;
    drawWorldDynamicLight__FP13PolysetHeader(ph);
    if (addCalls) return 1;
    // Mode 0: fixed radius h[16]; the light gets position, radius, 1/r^2, halfwords, then lightAdd.
    arm(0);
    drawWorldDynamicLight__FP13PolysetHeader(ph);
    if (addCalls != 1 || addedLight != light) return 2;
    if (lf_(light + 28) != 50.0f || lf_(light + 36) != 50.0f || lf_(light + 32) != 1.0f / 2500.0f || lf_(light + 40) != 1.0f / 2500.0f) return 3;
    if (lf_(light + 48) != 100.0f || lf_(light + 52) != -10.0f || lf_(light + 56) != 2.0f) return 4;
    if (hw(light + 44) != 0 || light[8] != 0) return 5;
    if ((short)(light[18] | (light[19] << 8)) != 1 || (short)(light[20] | (light[21] << 8)) != -2 ||
        (short)(light[12] | (light[13] << 8)) != -2 || (short)(light[16] | (light[17] << 8)) != 4) return 6;
    // Mode 4 does not touch the light, mode 9 acts like the generic path.
    arm(4);
    drawWorldDynamicLight__FP13PolysetHeader(ph);
    if (addCalls != 0 || lf_(light + 28) != 77.0f) return 7;
    arm(9);
    drawWorldDynamicLight__FP13PolysetHeader(ph);
    if (addCalls != 1 || lf_(light + 28) != 77.0f) return 8;
    // Mode 1: only when ph[32] < frameNm; random value between h[16] and h[18].
    arm(1); sw_(ph + 32, 2000); eRandState = 5;
    drawWorldDynamicLight__FP13PolysetHeader(ph);
    if (lf_(light + 28) != 77.0f || eRandState != 5 || hw(ph + 32) != 2000) return 9;
    sw_(ph + 32, 0);
    drawWorldDynamicLight__FP13PolysetHeader(ph);
    {
        unsigned s = 5u * 1664525u + 1013904223u;
        int want = 50 + (int)((float)100 * ((float)(int)(s >> 16) * (1.0f / 65536.0f)));
        if ((unsigned)eRandState != s || hw(ph + 32) != 1010 || lf_(light + 28) != (float)want) return 10;
    }
    // Mode 2: toggles bit 31 of ph[4]; endpoint then step.
    arm(2); sw_(ph + 32, 0);
    drawWorldDynamicLight__FP13PolysetHeader(ph);
    if (hw(ph + 4) != 0x80000000u || hw(ph + 32) != 1010) return 11;
    if (lf_(light + 28) != (float)(150 - 10)) return 12;     // 150 + (-1 * 100) / 10
    arm(2); sw_(ph + 4, 0x80000000u | 7); sw_(ph + 32, 0);
    drawWorldDynamicLight__FP13PolysetHeader(ph);
    if (hw(ph + 4) != 7u || lf_(light + 28) != (float)(50 + 10)) return 13;
    arm(2); sw_(ph + 32, 5000); sf_(light + 28, 100.0f);
    drawWorldDynamicLight__FP13PolysetHeader(ph);
    if (lf_(light + 28) != 110.0f) return 14;                // not toggled: current radius plus the step
    arm(2); sh_(h + 22, 0); sw_(ph + 32, 5000); sf_(light + 28, 100.0f);
    drawWorldDynamicLight__FP13PolysetHeader(ph);
    if (lf_(light + 28) != 100.0f) return 15;
    // Mode 3: flips between the two bounds.
    arm(3); sf_(light + 28, 50.0f);
    drawWorldDynamicLight__FP13PolysetHeader(ph);
    if (lf_(light + 28) != 150.0f || hw(ph + 32) != 1010) return 16;
    arm(3); sf_(light + 28, 70.0f);
    drawWorldDynamicLight__FP13PolysetHeader(ph);
    if (lf_(light + 28) != 50.0f) return 17;
    // Clamping: radius above 299 becomes 299, below zero 0.
    arm(0); sh_(h + 16, 500);
    drawWorldDynamicLight__FP13PolysetHeader(ph);
    if (lf_(light + 28) != 299.0f) return 18;
    // Halo sprite.
    arm(4);
    h[39] = 1; h[34] = 2; h[35] = 11; h[36] = 12; h[37] = 13; h[38] = 14; h[40] = 9; sh_(h + 32, 100);
    unsigned char texture[4];
    lightHaloTexture = texture;
    drawWorldDynamicLight__FP13PolysetHeader(ph);
    if (spriteCalls != 1) return 19;
    if (spriteArgs[0] != 100.0f || spriteArgs[3] != 9.0f || spriteArgs[4] != 100.0f + (11 << 2)) return 20;   // k = (int)(0.5 * 22) = 11
    if (spriteInts[0] != 11 || spriteInts[1] != 12 || spriteInts[2] != 13 || spriteInts[3] != 14 || spriteInts[4] != 0 || spriteInts[5] != 2 || !spriteInts[6]) return 21;
    h[39] = 0; spriteCalls = 0;
    drawWorldDynamicLight__FP13PolysetHeader(ph);
    if (spriteCalls) return 22;
    h[39] = 1; lightHaloTexture = nullptr;
    drawWorldDynamicLight__FP13PolysetHeader(ph);
    if (spriteCalls) return 23;
    std::puts("drawWorldDynamicLight contract passed");
    return 0;
}
