#include "set_layer_texture.cpp"
#include <string.h>
#include <string>

static std::string tr; static void *r0, *d0;
extern "C" void ReleaseSurface__8VIRasteriR12VIDictionary(void* r, int s, void* d) { tr += "R" + std::to_string(s) + ";"; r0 = r; d0 = d; }
extern "C" void ShareSurface__8VIRasteri(void* r, int s) { tr += "S" + std::to_string(s) + ";"; r0 = r; }

int main() {
    alignas(4) static unsigned char m[1024]; int raster, dict;
    memset(m, 0, sizeof m);
    *(int*)(m + 48 + 144 * 2) = -1;
    if (SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(m, 2, 7, &raster, &dict) != 0) return 1;
    if (tr != "S7;" || *(int*)(m + 48 + 288) != 7 || *(int*)(m + 32) != 1 || r0 != &raster) return 2;
    tr.clear(); *(int*)(m + 32) = 0;
    SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(m, 2, -1, &raster, &dict);
    if (tr != "R7;" || *(int*)(m + 48 + 288) != -1 || d0 != &dict || *(int*)(m + 32) != 1) return 3;
    tr.clear();
    SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(m, 2, -1, &raster, &dict);
    if (tr != "") return 4;
    *(int*)(m + 48) = 5; tr.clear();
    SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(m, 0, 5, &raster, &dict);
    if (tr != "R5;S5;") return 5;
    for (int i = 0; i < 1024; ++i) if (i != 32 && i != 33 && i != 34 && i != 35 && !(i >= 48 && i < 52) && !(i >= 336 && i < 340) && m[i] != 0) return 6;
    return 0;
}
