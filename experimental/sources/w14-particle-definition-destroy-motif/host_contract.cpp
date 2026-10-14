#define NORRATH_HOST_ARENA 1
#include "destroy_motif.cpp"
#include <string>

static unsigned char arena[0x1000];
static unsigned char def[1024];
static std::string tr;
static int count, step;
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return arena + a; }
extern "C" int Begin__Ct6VIPool1ZP15VIParticleMotif(const void*) { tr += "B;"; return 1; }
extern "C" int End__Ct6VIPool1ZP15VIParticleMotif(const void*) { tr += "E;"; return count; }
extern "C" int Next__Ct6VIPool1ZP15VIParticleMotifi(const void*, int i) { tr += "N" + std::to_string(i) + ";"; return i + step; }
extern "C" void Erase__t6VIPool1ZP15VIParticleMotifi(void*, int i) { tr += "X" + std::to_string(i) + ";"; }
extern "C" void __builtin_delete(void* p) { tr += "D" + std::to_string((unsigned char*)p - arena) + ";"; }

int main() {
    *reinterpret_cast<unsigned*>(def + 724 + 8) = 0x100;
    count = 6; step = 2;
    for (int i = 0; i < 8; i++) *reinterpret_cast<unsigned*>(arena + 0x100 + 12 * i) = 0x200u + 0x10u * i;
    DestroyMotif__22VIParticleDefinitionExP15VIParticleMotif(def, 0x230);   // entry at index 3
    if (tr != "B;E;N1;E;X3;D560;") return 1;
    tr.clear();
    DestroyMotif__22VIParticleDefinitionExP15VIParticleMotif(def, 0x999);   // absent
    if (tr != "B;E;N1;E;N3;E;N5;E;") return 2;
    tr.clear();
    *reinterpret_cast<unsigned*>(arena + 0x100 + 12) = 0;                   // a null entry matches null
    DestroyMotif__22VIParticleDefinitionExP15VIParticleMotif(def, 0);
    if (tr != "B;E;X1;" ) return 3;
    tr.clear(); count = 1;
    DestroyMotif__22VIParticleDefinitionExP15VIParticleMotif(def, 0x210);   // empty range
    if (tr != "B;E;") return 4;
    return 0;
}
