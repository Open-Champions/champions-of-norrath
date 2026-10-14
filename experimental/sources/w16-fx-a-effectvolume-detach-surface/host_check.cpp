// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "detach_surface.cpp"
#include <string>
static unsigned char arena[1 << 16];
static unsigned char* sceneBytes = arena + 0x100;
extern "C" unsigned char* _14VIEffectVolume$mScene = sceneBytes;
extern "C" void* norrath_host_pointer(unsigned address) { return arena + address; }
static std::string trace;
static int off(const void* p) { return static_cast<int>(static_cast<const unsigned char*>(p) - arena); }
extern "C" void* Material__8VIRasteri(void* r, int i) { trace += "M" + std::to_string(off(r)) + "," + std::to_string(i) + ";"; return arena + 0x40; }
extern "C" void SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(void* m, int l, int s, void* r, void* d) {
    trace += "T" + std::to_string(off(m)) + "," + std::to_string(l) + "," + std::to_string(s) + "," + std::to_string(off(r)) + "," + std::to_string(off(d)) + ";";
}
extern "C" void ReleaseMaterial__8VIRasteriR12VIDictionary(void* r, int i, void* d) {
    trace += "R" + std::to_string(off(r)) + "," + std::to_string(i) + "," + std::to_string(off(d)) + ";";
}
int main() {
    alignas(8) static unsigned char o[128];
    *reinterpret_cast<unsigned*>(sceneBytes + 19356) = 0x1000;
    *reinterpret_cast<unsigned*>(sceneBytes + 19352) = 0x2000;
    int* w = reinterpret_cast<int*>(o);
    w[17] = 99; w[20] = 12;
    if (DetachSurface__14VIEffectVolumei(o, 1) != 0) return 1;
    if (trace != "M4096,12;T64,0,-1,4096,8192;R4096,12,8192;") return 2;
    if (w[17] != -1 || w[20] != -1) return 3;
    trace.clear(); w[16] = 5; w[19] = -1;
    DetachSurface__14VIEffectVolumei(o, 0);
    if (!trace.empty() || w[16] != -1 || w[19] != -1) return 4;
    return 0;
}
