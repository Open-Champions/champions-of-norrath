// Host contract for changeOrientation__7VehicleG6Point3. Not an EE plan input.
#include "change_orientation.cpp"

#include <cstdio>
#include <cstring>

static int order[3], n;
static Quat mul_a, mul_b;
static Quat norm_in;
static unsigned char* mat_out;
static Quat mat_in;

extern "C" void quatMul__FP4QuatN20(Quat* out, const Quat* a, const Quat* b) {
    order[n++] = 1; mul_a = *a; mul_b = *b;
    out->x = 1; out->y = 2; out->z = 3; out->w = 4;  // product the caller must add
}
extern "C" void quatNormalize__FP4Quat(Quat* q) { order[n++] = 2; norm_in = *q; q->x = -1; }
extern "C" void quatToMat__FP8Matrix33P4Quat(void* m, Quat* q) { order[n++] = 3; mat_out = (unsigned char*)m; mat_in = *q; }

int main() {
    int failures = 0;
    unsigned char vehicle[2304];
    std::memset(vehicle, 0, sizeof vehicle);
    Quat start = {10, 20, 30, 40};
    std::memcpy(vehicle + 1084, &start, sizeof start);
    const float v[3] = {2, -4, 6};
    changeOrientation__7VehicleG6Point3(vehicle, v);
    if (n != 3 || order[0] != 1 || order[1] != 2 || order[2] != 3) { std::fprintf(stderr, "fail: call order\n"); ++failures; }
    if (mul_a.x != 10 || mul_a.w != 40) { std::fprintf(stderr, "fail: multiplicand\n"); ++failures; }
    if (mul_b.x != 1 || mul_b.y != -2 || mul_b.z != 3 || mul_b.w != 0) { std::fprintf(stderr, "fail: spin\n"); ++failures; }
    if (norm_in.x != 11 || norm_in.y != 22 || norm_in.z != 33 || norm_in.w != 44) { std::fprintf(stderr, "fail: sum before normalise\n"); ++failures; }
    if (mat_out != vehicle + 1116 || mat_in.x != -1 || mat_in.y != 22) { std::fprintf(stderr, "fail: matrix call\n"); ++failures; }
    return failures ? 1 : 0;
}
