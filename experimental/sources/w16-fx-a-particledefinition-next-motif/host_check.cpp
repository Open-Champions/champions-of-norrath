// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 12];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
#include "next_motif.cpp"
#include <string>
static std::string trace;
static int count = 3, firstIndex = 0, step = 1;
extern "C" int Begin__Ct6VIPool1ZP15VIParticleMotif(const void*) { trace += "B;"; return firstIndex; }
extern "C" int End__Ct6VIPool1ZP15VIParticleMotif(const void*) { trace += "E;"; return count; }
extern "C" int Next__Ct6VIPool1ZP15VIParticleMotifi(const void*, int i) { trace += "N" + std::to_string(i) + ";"; return i + step; }
int main() {
    alignas(8) static unsigned char def[800];
    *reinterpret_cast<unsigned*>(def + 724 + 8) = 0x100;
    for (unsigned i = 0; i < 4; ++i) *reinterpret_cast<unsigned*>(arena + 0x100 + 12 * i) = 0x1000 + i;
    if (NextMotif__C22VIParticleDefinitionExP15VIParticleMotif(def, 0x1000) != 0x1001) return 1;
    if (trace != "B;E;N0;E;N0;") return 2;
    trace.clear();
    if (NextMotif__C22VIParticleDefinitionExP15VIParticleMotif(def, 0x1002) != 0 || trace != "B;E;N0;E;N1;E;N2;E;N2;E;") return 3;
    trace.clear();
    if (NextMotif__C22VIParticleDefinitionExP15VIParticleMotif(def, 0x7777) != 0) return 4;
    trace.clear(); step = 2;
    if (NextMotif__C22VIParticleDefinitionExP15VIParticleMotif(def, 0x1000) != 0x1002 || trace != "B;E;N0;E;N0;") return 5;
    return 0;
}
