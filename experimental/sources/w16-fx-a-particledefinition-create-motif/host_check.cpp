// Host check only; not a build input.
#include "create_motif.cpp"
#include <cstring>
#include <string>
static unsigned char block[800];
static void* newResult = block;
static std::string trace;
static unsigned addValue = 99;
static const void* addPool;
extern "C" void* __builtin_new(unsigned size) { trace += "new" + std::to_string(size) + ";"; return newResult; }
extern "C" void __20VIParticleAttributes(void* p) { trace += "ctor;"; std::memset(p, 0x31, 760); }
extern "C" unsigned VIHashResourceID(const char* s) { trace += std::string("hash[") + s + "];"; return 0xabcdef01u; }
extern "C" int Add__t6VIPool1ZP15VIParticleMotifRCP15VIParticleMotif(void* pool, const unsigned* m) { trace += "add;"; addPool = pool; addValue = *m; return 3; }
int main() {
    alignas(8) static unsigned char def[800];
    unsigned r = CreateMotif__22VIParticleDefinitionEx(def);
    if (trace != "new760;ctor;hash[];add;" || addPool != def + 724) return 1;
    if (r != static_cast<unsigned>(reinterpret_cast<unsigned long>(block))) return 2;
    if (block[724] != 0 || *reinterpret_cast<unsigned*>(block + 756) != 0xabcdef01u || block[0] != 0x31 || block[723] != 0x31) return 3;
    trace.clear(); newResult = nullptr;
    if (CreateMotif__22VIParticleDefinitionEx(def) != 0 || trace != "new760;add;" || addValue != 0) return 4;
    return 0;
}
