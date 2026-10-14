// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "create_particle_emitters.cpp"
#include <cstring>
#include <initializer_list>
static int pdN, crN, failK; static int pdIds[8]; static void* crEm[8]; static void* crSys[8]; static void* crDef[8]; static unsigned char marker[8];
extern "C" void* ParticleDefinition__16VIParticleSystemi(void*, int id) { pdIds[pdN++] = id; return marker + (id & 7); }
extern "C" int Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition(void* e, void* s, void* d) {
    int n = crN++; crEm[n] = e; crSys[n] = s; crDef[n] = d; return n == failK ? -4 : n;
}
int main() {
    alignas(8) static unsigned char obj[8192], scene[2048];
    const int idsets[][3] = {{4, -1, 6}, {-1, -1, -1}, {1, 2, 3}};
    for (int cnt : {0, 1, 3, -2}) for (auto& ids : idsets) for (int fk = -1; fk < 3; ++fk) {
        std::memset(arena, 0, sizeof arena);
        for (int k = 0; k < 3; ++k) std::memcpy(arena + 0x100 + 1056 * k, &ids[k], 4);
        unsigned base = arena_base + 0x100;
        std::memcpy(obj + 4616, &cnt, 4); std::memcpy(obj + 4620, &base, 4);
        pdN = crN = 0; failK = fk;
        int r = CreateParticleEmitters__9VICSpriteR7VIScene(obj, scene);
        int expectCreates = 0; int want = 0; int wantPd = 0;
        for (int i = 0; i < cnt; ++i) {
            if (ids[i] == -1) continue;
            ++wantPd; ++expectCreates;
            if (i >= 0 && expectCreates - 1 == fk) { want = -1; break; }
        }
        if (r != want || pdN != wantPd || crN != expectCreates) return 1;
        int k = 0;
        for (int i = 0; i < cnt && k < crN; ++i) {
            if (ids[i] == -1) continue;
            if (pdIds[k] != ids[i] || crEm[k] != arena + 0x100 + 1056 * i + 16 || crSys[k] != scene + 1352 || crDef[k] != marker + (ids[i] & 7)) return 2;
            ++k;
        }
    }
    return 0;
}
