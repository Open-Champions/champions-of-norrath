#define NORRATH_HOST_ARENA 1
#include "first_motif.cpp"
#include <string>

static unsigned char arena[0x400];
static unsigned char def[1024];
static int size_result, begin_result;
static std::string tr;
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return arena + a; }
extern "C" int Size__Ct6VIPool1ZP15VIParticleMotif(const void* p) { tr += "S" + std::to_string((const unsigned char*)p - def) + ";"; return size_result; }
extern "C" int Begin__Ct6VIPool1ZP15VIParticleMotif(const void* p) { tr += "B" + std::to_string((const unsigned char*)p - def) + ";"; return begin_result; }

int main() {
    *reinterpret_cast<unsigned*>(def + 724 + 8) = 0x100;
    for (int i = 0; i < 6; i++) *reinterpret_cast<unsigned*>(arena + 0x100 + 12 * i) = 0x7000u + i;
    size_result = 0; begin_result = 3;
    if (FirstMotif__C22VIParticleDefinitionEx(def) != 0) return 1;
    if (tr != "S724;") return 2;
    tr.clear(); size_result = 4;
    if (FirstMotif__C22VIParticleDefinitionEx(def) != 0x7003u) return 3;
    if (tr != "S724;B724;") return 4;
    begin_result = 0;
    if (FirstMotif__C22VIParticleDefinitionEx(def) != 0x7000u) return 5;
    return 0;
}
