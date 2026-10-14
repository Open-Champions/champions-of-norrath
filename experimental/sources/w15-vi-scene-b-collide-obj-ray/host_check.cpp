// Host check only; not a build input. Every callee is a recorder with canned results.
#define NORRATH_HOST_ARENA 1
#include "collide_obj.cpp"
#include <cstdint>
#include <cstring>
static unsigned char arena[0x1000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
extern "C" void* norrath_host_function(unsigned) { return nullptr; }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }
static int init_result, coll_result, ctor_calls, init_calls, coll_calls, ml_calls, norm_calls;
static unsigned char* probe_seen; static const unsigned char *init_src, *init_matrix, *ml_matrix; static unsigned char* coll_third;
extern "C" void __9VICollRay(unsigned char* p) { ++ctor_calls; probe_seen = p; std::memset(p, 0, 96); }
extern "C" int Init__9VICollRayRC9VICollRayRC10VIMatrix44(unsigned char* p, const unsigned char* s, const unsigned char* m) { ++init_calls; init_src = s; init_matrix = m; return p == probe_seen ? init_result : 99; }
extern "C" int Collide__9VICollideR9VICollRayR12VICollBuffer(unsigned char* self, unsigned char* p, unsigned char* third) {
    ++coll_calls; coll_third = third; (void)self;
    float* f = reinterpret_cast<float*>(p);
    f[17] = 4.5f; *reinterpret_cast<unsigned*>(p + 72) = 0xabcd; f[19] = 1.0f; f[20] = 2.0f; f[21] = 3.0f;
    return coll_result;
}
extern "C" float* __ml__FRC7VIVect4RC10VIMatrix44(float* r, const float* v, const unsigned char* m) {
    ++ml_calls; ml_matrix = m;
    if (v[3] != 0.0f) return nullptr;
    r[0] = v[0] + 10; r[1] = v[1] + 20; r[2] = v[2] + 30; r[3] = 5;
    return r;
}
extern "C" void Normalize__7VIVect4(float* v) { ++norm_calls; for (int i = 0; i < 4; ++i) v[i] *= 0.5f; }
int main() {
    unsigned char* self = arena;
    unsigned char* ray_obj = arena + 0x400;
    unsigned char* table = arena + 0x600;
    unsigned char* buffers = arena + 0x700;
    int idx = 2;
    struct { int unused; } capsule;
    (void)capsule; (void)table; (void)buffers; (void)idx;
    *reinterpret_cast<unsigned*>(self + 536) = word_of(table);
    for (int k = 0; k < 4; ++k) *reinterpret_cast<unsigned*>(table + 12 * k) = word_of(buffers + 0x40 * k);
    for (int i = 0x400; i < 0x500; ++i) arena[i] = 0x11;
    // refused by the local init: -1 and nothing else
    init_result = -1; coll_result = 1;
    if (CollideObj__9VICollideR9VICollRayi(self, ray_obj, idx) != -1 || coll_calls != 0 || ctor_calls != 1 || init_calls != 1) return 1;
    if (init_src != ray_obj || init_matrix != self + 80) return 2;
    // no hit: 0, and the caller's object is untouched
    init_result = 0; coll_result = 0;
    if (CollideObj__9VICollideR9VICollRayi(self, ray_obj, idx) != 0 || coll_calls != 1 || ml_calls != 0) return 3;
    if (ray_obj[68] != 0x11 || ray_obj[76] != 0x11) return 4;
    // hit: result fields are copied and the vector is transformed with C+16 and normalised
    coll_result = 1; init_result = 7;
    if (CollideObj__9VICollideR9VICollRayi(self, ray_obj, idx) != 1 || ml_calls != 1 || norm_calls != 1) return 5;
    if (ml_matrix != self + 16) return 6;
    if (coll_third != buffers + 0x40 * idx) return 7;
    if (*reinterpret_cast<float*>(ray_obj + 68) != 4.5f || *reinterpret_cast<unsigned*>(ray_obj + 72) != 0xabcdu) return 8;
    if (*reinterpret_cast<float*>(ray_obj + 76) != 5.5f || *reinterpret_cast<float*>(ray_obj + 80) != 11.0f || *reinterpret_cast<float*>(ray_obj + 84) != 16.5f) return 9;
    return 0;
}
