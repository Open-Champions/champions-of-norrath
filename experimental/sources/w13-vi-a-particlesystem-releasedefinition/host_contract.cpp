#define NORRATH_HOST_ARENA 1
#include "release_definition.cpp"
#include <string.h>
#include <string>

static unsigned char arena[0x4000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return arena + a; }
static std::string tr; static int motifs, erase_ret;
static std::string o(const void* p) { return std::to_string((const unsigned char*)p - arena); }
extern "C" int Remove__12VIDictionaryR10VIResource(void* d, void* r) { tr += "D" + o(d) + "," + o(r) + ";"; return 0; }
extern "C" void* Material__8VIRasteri(void* r, int m) { tr += "M" + o(r) + "," + std::to_string(m) + ";"; return arena + 0x3000; }
extern "C" int SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(void* m, int l, int t, void* r, void* d) { tr += "T" + o(m) + "," + std::to_string(l) + "," + std::to_string(t) + "," + o(r) + "," + o(d) + ";"; return 0; }
extern "C" int ReleaseMaterial__8VIRasteriR12VIDictionary(void* r, int m, void* d) { tr += "X" + o(r) + "," + std::to_string(m) + "," + o(d) + ";"; return 0; }
extern "C" void* FirstMotif__C22VIParticleDefinitionEx(const void* d) { tr += "F" + o(d) + ";"; return motifs-- > 0 ? arena + 0x3100 + motifs : 0; }
extern "C" void DestroyMotif__22VIParticleDefinitionExP15VIParticleMotif(void* d, void* m) { tr += "K" + o(d) + "," + o(m) + ";"; }
extern "C" void _$_22VIParticleDefinitionEx(void* d, int f) { tr += "~" + o(d) + "," + std::to_string(f) + ";"; }
extern "C" void __builtin_delete(void* p) { tr += "L" + o(p) + ";"; }
extern "C" int Erase__t6VIPool1ZP20VIParticleDefinitioni(void* p, int i) { tr += "E" + o(p) + "," + std::to_string(i) + ";"; return erase_ret; }

// system at arena 0x100 (raster word 0x2000, pool at +84 with item array at 0x800); definition
// 2 is at 0x1000, its refs at +764.
static unsigned char* sys = arena + 0x100;
static void setup(int count, int reg, int mat) {
    memset(arena, 0x2c, 0x3000); tr.clear();
    *(unsigned*)sys = 0x2000; *(unsigned*)(sys + 92) = 0x800;
    *(unsigned*)(arena + 0x800 + 2 * 12) = 0x1000;
    *(int*)(arena + 0x1000 + 764) = reg; *(int*)(arena + 0x1000 + 768) = count; *(int*)(arena + 0x1000 + 776) = mat;
}
int main() {
    int dict = 0;
    unsigned char* dictp = arena + 0x20; (void)dict;
    setup(3, 1, 4);
    if (ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary(sys, 2, dictp) != 0 || !tr.empty()) return 1;
    if (*(int*)(arena + 0x1000 + 768) != 2) return 2;
    setup(1, 1, 4); motifs = 2; erase_ret = 0;
    if (ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary(sys, 2, dictp) != 0) return 3;
    std::string want = "D32,4860;M8192,4;T12288,0,-1,8192,32;X8192,4,32;F4096;K4096,12545;F4096;K4096,12544;F4096;~4096,0;L4096;E" + std::to_string(0x100 + 84) + ",2;";
    if (tr != want) return 4;
    if (*(int*)(arena + 0x1000 + 768) != 0) return 5;
    // not registered, no material, no motifs, erase failure
    setup(0, 0, -1); motifs = 0; erase_ret = -7;
    if (ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary(sys, 2, dictp) != -1) return 6;
    if (tr != "F4096;~4096,0;L4096;E" + std::to_string(0x100 + 84) + ",2;") return 7;
    if (*(int*)(arena + 0x1000 + 768) != -1) return 8;
    return 0;
}
