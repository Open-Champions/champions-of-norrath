#pragma once
// Shared by the static-initializer units of the spell, skill and actor sources.
// Observed: each initializer passes an 80-byte, 16-byte-aligned record address
// followed by eight integers, three single floats, three integers and a null
// callback to the legacy particle definition constructor. The constructor body
// belongs to another package and stays an unresolved import; the record layout
// is not known here, so it is opaque storage.
extern "C" void __11ParticleDefiiiiiiiifffiiiPFPv_v(
    void* self, int a, int b, int c, int d, int e, int f, int g, int h,
    float x, float y, float z, int i, int j, int k, void (*callback)(void*));

namespace w8_static_init {

struct alignas(16) ParticleDefStorage {
    unsigned char bytes[80];
};

static inline __attribute__((always_inline)) void construct_particle_def(
    ParticleDefStorage& record, int a, int b, int c, int d, int e, int f, int g, int h,
    float x, float y, float z, int i, int j, int k) {
    __11ParticleDefiiiiiiiifffiiiPFPv_v(&record, a, b, c, d, e, f, g, h, x, y, z, i, j, k, 0);
}

}  // namespace w8_static_init
