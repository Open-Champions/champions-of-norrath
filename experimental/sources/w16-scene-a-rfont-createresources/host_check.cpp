// Host check only; not a build input.
#include "create_resources.cpp"
#include <cstdio>
#include <cstring>
static char trace[256];
static int ret_cs = 5, ret_ss = 0, ret_si = 0, ret_cm = 7, ret_sm = 0;
static unsigned surfaceWords[32], materialWords[8];
static unsigned char lastColor[4];
static int lastBlend = -1, lastWrap = -1, lastDither = -1, lastLayerSurface = -1, lastInitBits = -1, lastFlag = -1;
static void T(const char* s) { std::strcat(trace, s); std::strcat(trace, ","); }
extern "C" {
int CreateSurface__8VIRasteri(unsigned char*, int) { T("cs"); return ret_cs; }
int ShareSurface__8VIRasteri(unsigned char*, int) { T("ss"); return ret_ss; }
unsigned char* Surface__8VIRasteri(unsigned char*, int) { T("sf"); return reinterpret_cast<unsigned char*>(surfaceWords); }
int Init__9VISurfaceR8VIRasterii10VIBitDepthi(unsigned char*, unsigned char*, int w, int h, int bits, int flag) { T("si"); lastInitBits = bits; lastFlag = flag; return (w == 256 && h == 256) ? ret_si : -9; }
int CreateMaterial__8VIRasteri(unsigned char*, int) { T("cm"); return ret_cm; }
int ShareMaterial__8VIRasteri(unsigned char*, int) { T("sm"); return ret_sm; }
unsigned char* Material__8VIRasteri(unsigned char*, int) { T("mt"); return reinterpret_cast<unsigned char*>(materialWords); }
void Init__10VIMateriali(unsigned char*, int) { T("mi"); }
void SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(unsigned char*, int, int id, unsigned char*, unsigned char*) { T("lt"); lastLayerSurface = id; }
void SetLayerWrapMode__10VIMateriali18VIMaterialWrapMode(unsigned char*, int, int m) { T("lw"); lastWrap = m; }
void SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(unsigned char*, int, int m) { T("lb"); lastBlend = m; }
void SetLayerColor__10VIMaterialiRC9VIColor32(unsigned char*, int, const unsigned char* c) { T("lc"); std::memcpy(lastColor, c, 4); }
void SetDither__10VIMateriali(unsigned char*, int) { T("sd"); lastDither = 0; }
}
static int run(unsigned* f, int extra, int bits, int last) {
    trace[0] = 0;
    return CreateResources__7VIRFontR8VIRasterR12VIDictionaryii10VIBitDepthi(reinterpret_cast<unsigned char*>(f), nullptr, nullptr, 99, extra, bits, last);
}
int main() {
    unsigned f[32] = {};
    f[6] = 12;
    surfaceWords[4] = 1; surfaceWords[10] = 1; surfaceWords[13] = 0x5150;
    bool ok = run(f, 16, 3, 1) == 0;
    ok = ok && !std::strcmp(trace, "cs,ss,sf,si,cm,sm,mt,mi,lt,lw,lb,lc,sd,");
    ok = ok && f[3] == 5 && f[4] == 7 && f[12] == (256 / 16) * (256 / 16) && f[7] == 1 && f[8] == 0x5150 && f[13] == 256;
    ok = ok && lastBlend == 0 && lastWrap == 3 && lastInitBits == 3 && lastFlag == 1 && lastLayerSurface == 5 && lastColor[0] == 255 && lastColor[3] == 255;
    ok = ok && run(f, 16, 3, 0) == 0 && lastBlend == 3;
    surfaceWords[4] = 0; ok = ok && run(f, 8, 3, 0) == 0 && f[13] == 16 && f[12] == (256 / 12) * (256 / 8);
    surfaceWords[4] = 5; ok = ok && run(f, 8, 3, 0) == -1 && !std::strstr(trace, "cm");
    surfaceWords[4] = 5; surfaceWords[10] = 0; ok = ok && run(f, 8, 3, 0) == 0 && f[13] == 0 && f[7] == 5;
    surfaceWords[10] = 1; surfaceWords[4] = 1;
    ret_cs = -3; ok = ok && run(f, 8, 3, 0) == -1 && f[3] == (unsigned)-3 && !std::strstr(trace, "ss"); ret_cs = 5;
    ret_ss = -1; ok = ok && run(f, 8, 3, 0) == -1 && !std::strstr(trace, "sf"); ret_ss = 0;
    ok = ok && run(f, 400, 3, 0) == -1 && !std::strstr(trace, "sf");   // 256 / 400 is zero: no cells
    ret_si = -1; ok = ok && run(f, 8, 3, 0) == -1 && !std::strstr(trace, "cm"); ret_si = 0;
    ret_cm = -2; ok = ok && run(f, 8, 3, 0) == -1 && f[4] == (unsigned)-2 && !std::strstr(trace, "sm"); ret_cm = 7;
    ret_sm = -2; ok = ok && run(f, 8, 3, 0) == -1 && !std::strstr(trace, "mt"); ret_sm = 0;
    ok = ok && run(f, 0, 3, 0) == -1 && !std::strstr(trace, "sf");      // 256 / 0 does not trap here; the result is not positive
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
