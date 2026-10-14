#include "sphere_capsule.cpp"
#include <initializer_list>
#include <string.h>

// Mock callees: each logs its shape and returns a scripted value.
static int pis_cap_result, pis_cyl_result[2], nc_result, sphere_result[2];
static int n_cap, n_cyl, n_nc, n_sph;
static float seen_cap[8], seen_cyl[2][8], seen_nc[8], seen_sph[2][4];
static const void *seen_nc_probe, *seen_sph_probe;
extern "C" int PointInSolid__9VICapsuleRC7VIVect3(const float* c, const float*) {
    memcpy(seen_cap, c, 32); ++n_cap; return pis_cap_result;
}
extern "C" int PointInSolid__10VICylinderRC7VIVect3(const float* c, const float*) {
    memcpy(seen_cyl[n_cyl < 2 ? n_cyl : 1], c, 32); return pis_cyl_result[n_cyl++ < 2 ? n_cyl - 1 : 1];
}
extern "C" int CollideNoCaps__9VICollideR9VICollRayRC10VICylinder(void*, void* p, const float* c) {
    memcpy(seen_nc, c, 32); seen_nc_probe = p; ++n_nc; return nc_result;
}
extern "C" int Collide__9VICollideR9VICollRayRC8VISphere(void*, void* p, const float* s) {
    memcpy(seen_sph[n_sph < 2 ? n_sph : 1], s, 16); seen_sph_probe = p; return sphere_result[n_sph++ < 2 ? n_sph - 1 : 1];
}

static void reset() { n_cap = n_cyl = n_nc = n_sph = 0; }

int main() {
    alignas(16) float probe[32] = {};
    alignas(16) float cap[8] = {1, 2, 3, 0, 0, 1, 10, 0.5f};   // base (1,2,3), axis +z, length 10, radius 0.5
    probe[22] = 0.25f;                                           // probe radius
    probe[0] = 1; probe[1] = 2; probe[2] = 8;
    unsigned char dummy[1];

    // 1. Outside the grown capsule: all three tests run, result is their OR.
    for (int nc : {0, 1}) for (int s0 : {0, 1}) for (int s1 : {0, 1}) {
        reset(); pis_cap_result = 0; nc_result = nc; sphere_result[0] = s0; sphere_result[1] = s1;
        int r = Collide__9VICollideR12VICollSphereR9VICapsule(dummy, reinterpret_cast<unsigned char*>(probe), reinterpret_cast<unsigned char*>(cap));
        if (r != (nc | s0 | s1)) return 1;
        if (n_cap != 1 || n_nc != 1 || n_sph != 2 || n_cyl != 0) return 2;
        if (seen_cap[7] != 0.75f || seen_cap[6] != 10 || seen_nc[7] != 0.75f || seen_nc[3] != 0 || seen_nc[5] != 1) return 3;
        if (seen_sph[0][0] != 1 || seen_sph[0][2] != 3 || seen_sph[0][3] != 0.75f) return 4;
        if (seen_sph[1][0] != 1 || seen_sph[1][1] != 2 || seen_sph[1][2] != 13 || seen_sph[1][3] != 0.75f) return 5;
        if (seen_nc_probe != probe || seen_sph_probe != probe) return 6;
    }
    // 2. Inside the plain cylinder: 0 with nothing else called.
    reset(); pis_cap_result = 1; pis_cyl_result[0] = 1;
    if (Collide__9VICollideR12VICollSphereR9VICapsule(dummy, reinterpret_cast<unsigned char*>(probe), reinterpret_cast<unsigned char*>(cap)) != 0) return 7;
    if (n_cyl != 1 || seen_cyl[0][7] != 0.5f || n_nc != 0 || n_sph != 0) return 8;
    // 3. Inside the wide cylinder only: result is CollideNoCaps.
    for (int nc : {0, 7}) {
        reset(); pis_cap_result = 1; pis_cyl_result[0] = 0; pis_cyl_result[1] = 1; nc_result = nc;
        int r = Collide__9VICollideR12VICollSphereR9VICapsule(dummy, reinterpret_cast<unsigned char*>(probe), reinterpret_cast<unsigned char*>(cap));
        if (r != (nc ? 1 : 0) || n_cyl != 2 || seen_cyl[1][7] != 0.75f || seen_nc[7] != 0.75f || n_sph != 0) return 9;
    }
    // 4. Near a cap: the sphere sits at the base for centres behind it, at the far end otherwise.
    for (float z : {-5.0f, 3.0f, 8.0f}) {
        reset(); pis_cap_result = 1; pis_cyl_result[0] = 0; pis_cyl_result[1] = 0; sphere_result[0] = 4;
        probe[2] = z;
        int r = Collide__9VICollideR12VICollSphereR9VICapsule(dummy, reinterpret_cast<unsigned char*>(probe), reinterpret_cast<unsigned char*>(cap));
        if (r != 1 || n_sph != 1 || n_nc != 0) return 10;
        float expect_z = z - 3.0f < 0 ? 3.0f : 13.0f;
        if (seen_sph[0][2] != expect_z || seen_sph[0][3] != 0.75f || seen_sph[0][0] != 1 || seen_sph[0][1] != 2) return 11;
        sphere_result[0] = 0;
        reset();
        if (Collide__9VICollideR12VICollSphereR9VICapsule(dummy, reinterpret_cast<unsigned char*>(probe), reinterpret_cast<unsigned char*>(cap)) != 0) return 12;
    }
    return 0;
}
