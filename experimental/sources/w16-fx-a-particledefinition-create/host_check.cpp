// Host check only; not a build input.
#include "create.cpp"
#include <string>
static std::string trace;
static int created;
static char rasterObject[8], materialObject[8];
static std::string P(const void* p) { return p == rasterObject ? "R" : p == materialObject ? "M" : "?"; }
extern "C" int CreateMaterial__8VIRasteri(void* r, int f) { trace += "C" + P(r) + std::to_string(f) + ";"; return created; }
extern "C" void* Material__8VIRasteri(void* r, int i) { trace += "Mat" + P(r) + std::to_string(i) + ";"; return materialObject; }
extern "C" void Init__10VIMateriali(void* m, int v) { trace += "I" + P(m) + std::to_string(v) + ";"; }
extern "C" void SetLayerFillType__10VIMateriali18VIMaterialFillType(void* m, int l, int t) { trace += "F" + P(m) + std::to_string(l) + std::to_string(t) + ";"; }
extern "C" void SetLayerModulate__10VIMaterialii(void* m, int l, int v) { trace += "O" + P(m) + std::to_string(l) + std::to_string(v) + ";"; }
extern "C" void SetDither__10VIMateriali(void* m, int v) { trace += "D" + P(m) + std::to_string(v) + ";"; }
extern "C" void SetBlendMode__20VIParticleDefinitionQ222VIParticleDefinitionEx9BLENDMODE(void*, int m) { trace += "B" + std::to_string(m) + ";"; }
extern "C" void SetZWrite__20VIParticleDefinitionb(void*, int v) { trace += "W" + std::to_string(v) + ";"; }
extern "C" void SetZTest__20VIParticleDefinitionb(void*, int v) { trace += "T" + std::to_string(v) + ";"; }
int main() {
    alignas(8) static unsigned char def[800];
    int* w = reinterpret_cast<int*>(def);
    w[187] = 3; w[188] = 5; w[189] = 9;
    created = 12;
    if (Create__20VIParticleDefinitionR8VIRaster(def, rasterObject) != 0) return 1;
    if (trace != "CR0;MatR12;IM1;FM00;OM01;DM0;B3;W5;T9;") return 2;
    if (w[194] != 12 || w[193] != static_cast<int>(reinterpret_cast<unsigned long>(rasterObject) & 0xffffffffu)) return 3;
    trace.clear(); created = -1; w[194] = 0;
    if (Create__20VIParticleDefinitionR8VIRaster(def, rasterObject) != -1 || trace != "CR0;" || w[194] != -1) return 4;
    return 0;
}
