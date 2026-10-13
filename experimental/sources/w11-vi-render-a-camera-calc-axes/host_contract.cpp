#include "calc_axes.cpp"
#include <string.h>

static int calls; static void* seen[4]; static float snap[4][3];
static int results[4] = {1, 1, 1, 1};
static int use_real = 0;
extern "C" int Normalize__7VIVect3(void* v) {
    float* f = static_cast<float*>(v);
    seen[calls] = v; memcpy(snap[calls], f, 12);
    int r = results[calls++];
    if (use_real) { float l = f[0] * f[0] + f[1] * f[1] + f[2] * f[2]; if (l > 0) { l = 1.0f / __builtin_sqrtf(l); f[0] *= l; f[1] *= l; f[2] *= l; } }
    return r;
}

int main() {
    unsigned char object[32]; memset(object, 0x5a, sizeof object);
    float d[3] = {2.0f, 3.0f, 5.0f}, r[3] = {9, 9, 9}, u[3] = {8, 8, 8}, f[3] = {7, 7, 7};
    int ret = CalcAxes__8VICameraRC7VIVect3R7VIVect3N22(object, d, r, u, f);
    if (ret != 0 || calls != 2) return 1;
    if (seen[0] != f || snap[0][0] != 2 || snap[0][1] != 3 || snap[0][2] != 5) return 2;
    if (seen[1] != r) return 3;
    // r = f x (0,1,0) = (fy*0 - fz*1, fz*0 - fx*0, fx*1 - fy*0) = (-5, 0, 2)
    if (snap[1][0] != -5 || snap[1][1] != 0 || snap[1][2] != 2) return 4;
    float mem[3]; memcpy(mem, object + 8, 12);
    if (mem[0] != 2 || mem[1] != 3 || mem[2] != 5) return 5;
    // u = r x f = (0*5 - 2*3, 2*2 - (-5)*5, -5*3 - 0*2) = (-6, 29, -15)
    if (u[0] != -6 || u[1] != 29 || u[2] != -15) return 6;
    if (object[7] != 0x5a || object[20] != 0x5a) return 7;
    // degenerate first result: the x axis is tried and normalized again
    calls = 0; results[1] = 0;
    float d2[3] = {0.0f, 4.0f, 0.0f};
    CalcAxes__8VICameraRC7VIVect3R7VIVect3N22(object, d2, r, u, f);
    if (calls != 3 || seen[2] != r) return 8;
    // first r = f x (0,1,0) = 0; second r = f x (1,0,0) = (0, 0, -4)
    if (snap[1][0] != 0 || snap[1][1] != 0 || snap[1][2] != 0) return 9;
    if (snap[2][0] != 0 || snap[2][1] != 0 || snap[2][2] != -4) return 10;
    // u = r x f = (0*0 - (-4)*4, -4*0 - 0*0, 0*4 - 0*0) = (16, 0, 0)
    if (u[0] != 16 || u[1] != 0 || u[2] != 0) return 11;
    return 0;
}
