#define tanf norrath_host_tanf
#include "init.cpp"
#undef tanf

#include <cmath>
#include <cstring>

static int order[8];
static int count;
static float tan_arg;
static float frustum[6];
static void* frustum_raster;
static VIMatrixLocal* frustum_matrix;
static VIMatrixLocal* update_matrix;
static VIMatrixLocal* dtor_matrix;
static int dtor_flags;
static void* calc_point;
static void* update_point;
static int update_result;
static float matrix_at_frustum[16];
static float axes_zero_ok = 1.0f;
static VIVect3Local* calc_axes[3];
static VIVect3Local* update_axes[3];

extern "C" float norrath_host_tanf(float angle) {
    tan_arg = angle;
    order[count++] = 1;
    return std::tan(angle);
}
extern "C" int Frustum__8VIRasterR10VIMatrix44ffffff(void* raster, VIMatrixLocal* matrix, float a, float b, float c, float d, float e, float f) {
    order[count++] = 2;
    frustum_raster = raster; frustum_matrix = matrix;
    frustum[0] = a; frustum[1] = b; frustum[2] = c; frustum[3] = d; frustum[4] = e; frustum[5] = f;
    std::memcpy(matrix_at_frustum, matrix->m, sizeof matrix_at_frustum);
    return 0;
}
extern "C" int CalcAxes__8VICameraRC7VIVect3R7VIVect3N22(void*, void* point, VIVect3Local* x, VIVect3Local* y, VIVect3Local* z) {
    order[count++] = 3;
    calc_point = point;
    calc_axes[0] = x; calc_axes[1] = y; calc_axes[2] = z;
    VIVect3Local* v[3] = {x, y, z};
    for (VIVect3Local* p : v) if (p->x != 0.0f || p->y != 0.0f || p->z != 0.0f) axes_zero_ok = 0.0f;
    return 0;
}
extern "C" int Update__8VICameraRC7VIVect3R7VIVect3N22R10VIMatrix44(void*, void* point, VIVect3Local* x, VIVect3Local* y, VIVect3Local* z, VIMatrixLocal* matrix) {
    order[count++] = 4;
    update_point = point;
    update_axes[0] = x; update_axes[1] = y; update_axes[2] = z;
    update_matrix = matrix;
    return update_result;
}
extern "C" void _$_10VIMatrix44(VIMatrixLocal* matrix, int flags) {
    order[count++] = 5;
    dtor_matrix = matrix;
    dtor_flags = flags;
}

int main() {
    float camera[10];
    for (float& f : camera) f = 123.0f;
    int raster, p, q;
    update_result = 0;
    int result = Init__8VICameraR8VIRasterRC7VIVect3T2ffff(camera, &raster, &p, &q, 60.0f, 1.5f, 2.0f, 100.0f);
    if (result != 0 || count != 5) return 1;
    const int expected[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; ++i) if (order[i] != expected[i]) return 2;
    float t = std::tan(60.0f * 0.017453292f * 0.5f);
    if (tan_arg != 60.0f * 0.017453292f * 0.5f) return 3;
    if (frustum_raster != &raster) return 4;
    if (frustum[0] != -t * 2.0f * 1.5f || frustum[1] != t * 2.0f * 1.5f || frustum[2] != t * 2.0f ||
        frustum[3] != -t * 2.0f || frustum[4] != 2.0f || frustum[5] != 100.0f) return 5;
    for (float f : matrix_at_frustum) if (f != 0.0f) return 6;
    if (camera[0] != 2.0f || camera[1] != 100.0f || camera[5] != 60.0f || camera[6] != 1.5f) return 7;
    if (camera[2] != 123.0f || camera[3] != 123.0f || camera[4] != 123.0f || camera[7] != 123.0f) return 8;
    if (calc_point != &q || update_point != &p) return 9;
    for (int i = 0; i < 3; ++i) if (calc_axes[i] != update_axes[i]) return 10;
    if (axes_zero_ok != 1.0f) return 11;
    if (update_matrix != frustum_matrix || dtor_matrix != frustum_matrix || dtor_flags != 2) return 12;

    count = 0;
    update_result = -3;
    if (Init__8VICameraR8VIRasterRC7VIVect3T2ffff(camera, &raster, &p, &q, 90.0f, 1.0f, 1.0f, 10.0f) != -1) return 13;
    if (count != 5) return 14;  // the matrix is destroyed on failure too
    update_result = 7;
    if (Init__8VICameraR8VIRasterRC7VIVect3T2ffff(camera, &raster, &p, &q, 90.0f, 1.0f, 1.0f, 10.0f) != 0) return 15;
    return 0;
}
