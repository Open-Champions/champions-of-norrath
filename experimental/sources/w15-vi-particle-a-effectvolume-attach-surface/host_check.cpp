// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "attach_surface.cpp"
#include <cstring>
#include <string>
static unsigned char arena[0x8000];
static unsigned char scene_block[20000];
extern "C" unsigned char* _14VIEffectVolume$mScene = scene_block;
extern "C" void* norrath_host_pointer(unsigned a) { return arena + a; }
static std::string tr; static int created; static unsigned char* mat = arena + 0x4000;
static std::string P(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - arena); }
extern "C" void DetachSurface__14VIEffectVolumei(void* s, int i) { tr += "Det" + P(s) + "," + std::to_string(i) + ";"; }
extern "C" int CreateMaterial__8VIRasteri(void* r, int f) { tr += "Cre" + P(r) + "," + std::to_string(f) + ";"; return created; }
extern "C" void* Material__8VIRasteri(void* r, int i) { tr += "Mat" + P(r) + "," + std::to_string(i) + ";"; return mat; }
extern "C" void Init__10VIMateriali(void* m, int v) { tr += "Init" + std::to_string(v) + ";"; (void)m; }
extern "C" void SetLayerFillType__10VIMateriali18VIMaterialFillType(void*, int l, int t) { tr += "Fill" + std::to_string(l) + std::to_string(t) + ";"; }
extern "C" void SetLayerModulate__10VIMaterialii(void*, int l, int v) { tr += "Mod" + std::to_string(l) + std::to_string(v) + ";"; }
extern "C" void SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(void*, int l, int m) { tr += "Blend" + std::to_string(l) + "," + std::to_string(m) + ";"; }
extern "C" void SetZWrite__10VIMateriali(void*, int v) { tr += "ZW" + std::to_string(v) + ";"; }
extern "C" void SetZTest__10VIMateriali(void*, int v) { tr += "ZT" + std::to_string(v) + ";"; }
extern "C" void SetDither__10VIMateriali(void*, int v) { tr += "Dith" + std::to_string(v) + ";"; }
extern "C" void SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(void*, int l, int s, void* r, void* d) { tr += "Tex" + std::to_string(l) + "," + std::to_string(s) + "," + P(r) + "," + P(d) + ";"; }
int main() {
    unsigned char* v = arena + 0x100;
    unsigned r = 0x2000, d = 0x3000;
    memcpy(scene_block + 19356, &r, 4); memcpy(scene_block + 19352, &d, 4);
    auto slot = [&](int i) { int x; memcpy(&x, v + 64 + 4 * i, 4); return x; };
    auto setw = [&](int off, int x) { memcpy(v + off, &x, 4); };
    auto reset = [&](unsigned type, int cur) { memset(arena, 0x33, 0x2000); setw(36, type); setw(64 + 8, cur); tr.clear(); };
    reset(0, -1); created = 4;
    if (AttachSurface__14VIEffectVolumeii(v, 2, -1) != -1 || !tr.empty() || slot(2) != -1) return 1;
    if (AttachSurface__14VIEffectVolumeii(v, 2, 77) != 0) return 2;
    if (tr != "Cre8192,0;Mat8192,4;Init1;Fill00;Mod01;Blend0,0;ZW0;ZT1;Dith0;Tex0,77,8192,12288;") return 3;
    if (slot(2) != 77 || *(int*)(v + 76 + 8) != 4) return 4;
    reset(1, 9); AttachSurface__14VIEffectVolumeii(v, 2, 5);
    if (tr.compare(0, 14, "Det256,2;Cre81") || tr.find("Blend0,3;") == std::string::npos) return 5;
    reset(2, -1); AttachSurface__14VIEffectVolumeii(v, 2, 5); if (tr.find("Blend0,1;") == std::string::npos) return 6;
    reset(3, -1); AttachSurface__14VIEffectVolumeii(v, 2, 5); if (tr.find("Blend0,3;") == std::string::npos) return 7;
    reset(4, -1); AttachSurface__14VIEffectVolumeii(v, 2, 5); if (tr.find("Blend0,3;") == std::string::npos) return 8;
    reset(5, -1); AttachSurface__14VIEffectVolumeii(v, 2, 5); if (tr.find("Blend") != std::string::npos || tr.find("ZW0;") == std::string::npos) return 9;
    reset(0, -1); created = -2;
    if (AttachSurface__14VIEffectVolumeii(v, 2, 5) != -1 || slot(2) != -1 || *(int*)(v + 76 + 8) != -2) return 10;
    if (tr != "Cre8192,0;") return 11;
    return 0;
}
