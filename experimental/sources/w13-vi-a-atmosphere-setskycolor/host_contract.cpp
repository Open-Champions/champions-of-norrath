#include <stdint.h>
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<uintptr_t>(word))
#include "set_sky_color.cpp"
#include <string.h>
#include <string>

static std::string tr; static int pal_size, set_neg_at; static int set_calls;
alignas(4) static unsigned char mats[3][16]; static unsigned char layer_alpha[3][4];
static void* sprite_tok = (void*)0x1234;
extern "C" void* Sprite__7VIScenei(void* s, int i) { tr += "P" + std::to_string(i) + ";"; (void)s; return sprite_tok; }
extern "C" int GetMaterialPal__C14VISimpleSprite(const void* s) { tr += s == sprite_tok ? "Q;" : "BADSPRITE;"; return 77; }
extern "C" int GetMaterialPalSize__8VIRasteri(void* r, int p) { tr += "N" + std::to_string((uintptr_t)r) + "," + std::to_string(p) + ";"; return pal_size; }
extern "C" unsigned char* GetMaterialPalEntry__8VIRasterii(void* r, int p, int i) { (void)r; (void)p; tr += "E" + std::to_string(i) + ";"; return mats[i]; }
extern "C" void GetLayerColor__C10VIMateriali(unsigned char* res, const void* m, int l) {
    int mi = ((const unsigned char*)m - &mats[0][0]) / 16; tr += "G" + std::to_string(mi) + "." + std::to_string(l) + ";";
    res[0] = 1; res[1] = 2; res[2] = 3; res[3] = layer_alpha[mi][l];
}
extern "C" int SetLayerColor__10VIMaterialiRC9VIColor32(void* m, int l, const unsigned char* c) {
    int mi = ((unsigned char*)m - &mats[0][0]) / 16;
    tr += "S" + std::to_string(mi) + "." + std::to_string(l) + ":" + std::to_string(c[0]) + "," + std::to_string(c[1]) + "," + std::to_string(c[2]) + "," + std::to_string(c[3]) + ";";
    return ++set_calls == set_neg_at ? -5 : 0;
}

int main() {
    alignas(4) static unsigned char atm[1300]; alignas(4) static unsigned char scene[19400];
    unsigned char color[4] = {10, 20, 30, 40};
    for (int i = 0; i < 3; ++i) for (int l = 0; l < 4; ++l) layer_alpha[i][l] = 100 + 10 * i + l;
    *(unsigned*)(scene + 19356) = 0x4000; *(int*)(atm + 1248) = 5;
    *(int*)(atm + 1228) = 0;
    if (SetSkyColor__12VIAtmosphereR7VISceneRC9VIColor32(atm, scene, color) != -1 || !tr.empty()) return 1;
    *(int*)(atm + 1228) = 1; pal_size = 0;
    if (SetSkyColor__12VIAtmosphereR7VISceneRC9VIColor32(atm, scene, color) != 0 || tr != "P5;Q;N16384,77;") return 2;
    // three materials with 2, 0 and 3 layers
    tr.clear(); pal_size = 3; set_neg_at = 0; set_calls = 0;
    *(int*)(mats[0] + 8) = 2; *(int*)(mats[1] + 8) = 0; *(int*)(mats[2] + 8) = 3;
    if (SetSkyColor__12VIAtmosphereR7VISceneRC9VIColor32(atm, scene, color) != 0) return 3;
    std::string want = "P5;Q;N16384,77;E0;G0.0;S0.0:10,20,30,100;G0.1;S0.1:10,20,30,101;E1;E2;"
                       "G2.0;S2.0:10,20,30,120;G2.1;S2.1:10,20,30,121;G2.2;S2.2:10,20,30,122;";
    if (tr != want) return 4;
    // a negative result at the fourth SetLayerColor stops everything with -1
    tr.clear(); set_calls = 0; set_neg_at = 4;
    if (SetSkyColor__12VIAtmosphereR7VISceneRC9VIColor32(atm, scene, color) != -1) return 5;
    if (tr.find("S2.1") == std::string::npos || tr.find("S2.2") != std::string::npos || tr.find("G2.1") == std::string::npos) return 6;
    return 0;
}
