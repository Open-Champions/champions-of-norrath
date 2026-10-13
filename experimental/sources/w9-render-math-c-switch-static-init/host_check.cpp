// Host contract for _GLOBAL_$I$__6SwitchG6Point3PPc. Not an EE plan input. The ParticleDef
// constructor is a recorder.
#include <cstdio>
#include <cstring>
#include <set>
#include <vector>
struct Call { void* self; int ints[8]; unsigned f[3]; int tail[3]; void (*callback)(void*); };
static std::vector<Call> calls;
extern "C" void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
    float x, float y, float z, int i, int j, int k, void (*callback)(void*)) {
    Call r{self, {a, b, c, d, e, f, g, h}, {}, {i, j, k}, callback};
    std::memcpy(&r.f[0], &x, 4); std::memcpy(&r.f[1], &y, 4); std::memcpy(&r.f[2], &z, 4);
    calls.push_back(r);
}
extern "C" {
void _GLOBAL_$I$__6SwitchG6Point3PPc();
extern unsigned char fireTrailParticle[80], trackfireTrailParticle[80], saveParticle[80], saveParticle2[80];
extern unsigned char teleportParticle1[80], teleportParticle2[80];
extern unsigned char norrath_container_msg_run_fuseParticle_bytes[80], norrath_container_msg_run_smokeParticle_bytes[80];
}
int main() {
    _GLOBAL_$I$__6SwitchG6Point3PPc();
    if (calls.size() != 23) return 1;
    std::set<void*> distinct;
    for (const Call& c : calls) { distinct.insert(c.self); if (c.callback != nullptr) return 2; }
    if (distinct.size() != 23) return 3;
    if (calls[0].self != fireTrailParticle || calls[1].self != trackfireTrailParticle || calls[2].self != saveParticle || calls[3].self != saveParticle2) return 4;
    if (calls[7].self != teleportParticle1 || calls[8].self != teleportParticle2) return 5;
    if (calls[16].self != norrath_container_msg_run_fuseParticle_bytes || calls[17].self != norrath_container_msg_run_smokeParticle_bytes) return 6;
    // spot checks of argument tuples
    if (calls[7].ints[0] != 2 || calls[7].ints[1] != 83 || calls[7].ints[7] != 214 || calls[7].tail[0] != 3 || calls[7].tail[1] != 3 || calls[7].tail[2] != 2) return 7;
    if (calls[8].ints[0] != 72 || calls[8].f[0] != 0x404ae147u || calls[8].f[1] != 0x42916d91u) return 8;
    if (calls[2].ints[2] != 280 || calls[1].ints[4] != -11) return 9;
    if (calls[16].ints[0] != 255 || calls[16].f[2] != 0xc0a00000u || calls[17].ints[4] != -77) return 10;
    // the objects start zeroed (the recorder does not write them)
    for (unsigned char b : teleportParticle1) if (b != 0) return 11;
    std::printf("switch static init contract passed\n");
    return 0;
}
