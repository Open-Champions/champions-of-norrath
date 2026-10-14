// Host contract for _GLOBAL_$I$msg_run__7AntTrap. Not an EE plan input. The ParticleDef
// constructor is a recorder.
#include <cstdio>
#include <cstring>
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
void _GLOBAL_$I$msg_run__7AntTrap();
extern unsigned char norrath_anttrap_msg_run_FireParticle_bytes[80], norrath_anttrap_msg_run_HateGateParticle_bytes[80];
}
int main() {
    _GLOBAL_$I$msg_run__7AntTrap();
    if (calls.size() != 2) return 1;
    if (calls[0].self != norrath_anttrap_msg_run_FireParticle_bytes || calls[1].self != norrath_anttrap_msg_run_HateGateParticle_bytes) return 2;
    if (calls[0].ints[0] != 205 || calls[0].ints[7] != 4 || calls[0].f[0] != 0x41980000u || calls[0].f[2] != 0x3faccccc || calls[0].tail[2] != 6) return 3;
    if (calls[1].ints[0] != 227 || calls[1].ints[7] != 6 || calls[1].f[0] != 0x41200000u || calls[1].f[2] != 0x3f000000u) return 4;
    if (calls[1].tail[0] != 3 || calls[1].tail[1] != 1 || calls[1].tail[2] != 11) return 5;
    if (calls[0].callback != nullptr || calls[1].callback != nullptr) return 6;
    std::printf("anttrap static init contract passed\n");
    return 0;
}
