// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "create.cpp"
#include <cstring>
#include <string>
static unsigned char arena[0x3000];
extern "C" void* norrath_host_pointer(unsigned a) { return arena + a; }
static std::string tr; static bool fail_alloc; static int create_result, add_result; static unsigned char* newobj;
extern "C" void* __builtin_new(unsigned n) { tr += "N" + std::to_string(n) + ";"; return fail_alloc ? nullptr : newobj; }
extern "C" void __builtin_delete(void* p) { tr += "X" + std::to_string(static_cast<unsigned char*>(p) - arena) + ";"; }
extern "C" void __22VIParticleDefinitionEx(void* p) { tr += "C" + std::to_string(static_cast<unsigned char*>(p) - arena) + ";"; }
extern "C" void _$_22VIParticleDefinitionEx(void* p, int f) { tr += "D" + std::to_string(static_cast<unsigned char*>(p) - arena) + "," + std::to_string(f) + ";"; }
extern "C" int Create__20VIParticleDefinitionR8VIRaster(void* d, void* r) { tr += "R" + std::to_string(static_cast<unsigned char*>(d) - arena) + "," + std::to_string(static_cast<unsigned char*>(r) - arena) + ";"; return create_result; }
extern "C" int Add__t6VIPool1ZP20VIParticleDefinitionRCP20VIParticleDefinition(void* pool, void* const* e) { tr += "A" + std::to_string(static_cast<unsigned char*>(pool) - arena) + "," + std::to_string(static_cast<unsigned char*>(*e) - arena) + ";"; return add_result; }
int main() {
    unsigned char* sys = arena + 0x100; newobj = arena + 0x1000;
    memset(arena, 0x33, sizeof arena);
    *(unsigned*)sys = 0x2000;  // raster at arena+0x2000
    fail_alloc = true;
    if (CreateParticleDefinition__16VIParticleSystemi(sys, 1) != -1 || tr != "N784;") return 1;
    tr.clear(); fail_alloc = false; create_result = -3;
    if (CreateParticleDefinition__16VIParticleSystemi(sys, 1) != -1 || tr != "N784;C4096;R4096,8192;D4096,0;X4096;") return 2;
    tr.clear(); create_result = 0; add_result = 17;
    if (CreateParticleDefinition__16VIParticleSystemi(sys, 1) != 17 || tr != "N784;C4096;R4096,8192;A340,4096;") return 3;
    auto w = [&](int off) { return *(unsigned*)(newobj + off); };
    if (w(748) || w(752) || w(764) != 0 || w(768) != 1 || w(772) || w(756) != 1 || w(760) != 1 || w(776) != 0xffffffffu || w(780) != 0xffffffffu) return 4;
    for (int off = 0; off < 784; off += 4) {
        if (off == 748 || off == 752 || off == 756 || off == 760 || off == 764 || off == 768 || off == 772 || off == 776 || off == 780) continue;
        if (w(off) != 0x33333333u) return 5;
    }
    tr.clear(); memset(newobj, 0x33, 784); create_result = 5;
    CreateParticleDefinition__16VIParticleSystemi(sys, 0);
    if (w(768) != 0) return 6;
    return 0;
}
