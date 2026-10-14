// Host contract for doMyUniqueForces__4Boat. Not an EE plan input.
// cvt.w.s is modelled as round to nearest, ties to even.
#include "unique_forces.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>

static int failures = 0;
static int accum_calls, rot_calls, water_calls, rand_calls;
static float accum_point[3], accum_force[3];
static int accum_flag = -1;
static void* accum_vehicle;
static short rot_angle;
static int rot_axis;
static float rot_input[9];
static void* water_world;
static float water_point[3], water_a, water_b;
static int rand_script[64];
static int rand_len, rand_pos;
static int order[64], steps;
void* world;
int frameNm;

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static bool close(double a, double b) { return std::fabs(a - b) <= 1e-5 * (1.0 + std::fabs(b)); }

extern "C" void matRot__FP8Matrix33is(float* m, int axis, short angle) {
    order[steps++] = 1;
    ++rot_calls; rot_axis = axis; rot_angle = angle;
    std::memcpy(rot_input, m, sizeof rot_input);
    m[3] = 0.5f; m[4] = -2.0f; m[5] = 4.0f;
}
extern "C" void accumForceAndTorque__7VehicleP6Point3T1i(void* v, const float* p, const float* f, int flag) {
    order[steps++] = 2;
    ++accum_calls; accum_vehicle = v; accum_flag = flag;
    std::memcpy(accum_point, p, 12);
    std::memcpy(accum_force, f, 12);
}
extern "C" int eRand__10GameObject(void*) {
    order[steps++] = 3;
    ++rand_calls;
    return rand_script[rand_pos++ % rand_len];
}
extern "C" void worldPerturbWater__FP12_worldHeaderG6Point3ff(void* w, const float* p, float a, float b) {
    order[steps++] = 4;
    ++water_calls; water_world = w; water_a = a; water_b = b;
    std::memcpy(water_point, p, 12);
}

static void put_float(unsigned char* c, int off, float v) { std::memcpy(c + off, &v, 4); }
static float unit(int r) { return static_cast<float>(r) * (1.0f / 65536.0f) * 2.0f - 1.0f; }

int main() {
    static unsigned char boat[2304];
    int world_marker;
    world = &world_marker;
    for (int i = 0; i < 9; ++i) put_float(boat, 1116 + 4 * i, 1.0f + i);
    put_float(boat, 16, 100.0f); put_float(boat, 20, 200.0f); put_float(boat, 24, 300.0f);
    put_float(boat, 740, 2.0f);
    put_float(boat, 800, 45.0f);
    // frameNm     // No velocity: force step only. 3 != 0: force step only.
    frameNm = 1;
    steps = 0;
    int r = doMyUniqueForces__4Boat(boat);
    expect(r == 1, "returns 1");
    expect(rot_calls == 1 && rot_axis == 2, "rotation on axis 2");
    const float wantf = 45.0f * 0.0055555552244186f * 32767.0f;
    expect(rot_angle == static_cast<short>(static_cast<int>(std::nearbyint(wantf))), "angle from the float at +800");
    bool same = true;
    for (int i = 0; i < 9; ++i) same = same && rot_input[i] == 1.0f + i;
    expect(same, "matrix copied from +1116");
    expect(accum_calls == 1 && accum_vehicle == boat && accum_flag == 0, "force call with flag 0");
    expect(accum_point[0] == 0.0f && accum_point[1] == 191.0f && accum_point[2] == 0.0f, "application point (0,191,0)");
    const float k = 2.0f * 1216.0f;
    expect(accum_force[0] == -0.5f * k && accum_force[1] == 2.0f * k && accum_force[2] == -4.0f * k, "force is the negated rotated entries times k");
    expect(water_calls == 0 && rand_calls == 0, "no splash when frameNm & 3 is not zero");
    expect(order[0] == 1 && order[1] == 2 && steps == 2, "rotation then force");

    // Splash cases, with scripted random draws. Needs float(+740) != 0 and frameNm & 3 == 0.
    struct V { float f740; int frame; bool splash; };
    const V vs[] = {{0.0f, 0, false}, {-0.0f, 0, false}, {2.0f, 0, true}, {2.0f, 4, true}, {2.0f, 1, false}, {2.0f, 2, false}, {2.0f, 3, false}, {-1.0f, 8, true}};
    for (const V& v : vs) {
        put_float(boat, 740, v.f740);
        frameNm = v.frame;
        // first attempt outside the unit sphere, second inside
        const int script[] = {65535, 65535, 65535, 40000, 20000, 33000};
        std::memcpy(rand_script, script, sizeof script);
        rand_len = 6; rand_pos = 0; rand_calls = 0; water_calls = 0; steps = 0;
        doMyUniqueForces__4Boat(boat);
        expect((water_calls == 1) == v.splash, "splash exactly when +740 is nonzero and frameNm & 3 == 0");
        if (!v.splash) { expect(rand_calls == 0, "no draws without a splash"); continue; }
        expect(rand_calls == 6, "first attempt rejected (sum above 1), second accepted");
        expect(water_world == world && water_a == 400.0f && water_b == 24.0f, "world and the two floats");
        const float r1 = unit(40000), r2 = unit(20000), r3 = unit(33000);
        expect(r1 * r1 + r2 * r2 + r3 * r3 <= 1.0f, "script's accepted attempt is inside the sphere");
        expect(close(water_point[0], 100.0f + 60.0f * r1), "x offset from first draw");
        expect(close(water_point[1], 200.0f + 60.0f * r2), "y offset from second draw");
        expect(water_point[2] == 300.0f, "z unchanged");
        expect(order[steps - 1] == 4 && order[steps - 2] == 3, "splash is last, after the draws");
    }
    // Boundary: exactly length 1 is accepted (the retry test is strictly above 1).
    put_float(boat, 740, 2.0f); frameNm = 0;
    const int script2[] = {65536, 32768, 32768};  // r1 = 1, r2 = 0, r3 = 0: sum is exactly 1
    std::memcpy(rand_script, script2, sizeof script2);
    rand_len = 3; rand_pos = 0; rand_calls = 0; water_calls = 0;
    doMyUniqueForces__4Boat(boat);
    expect(rand_calls == 3 && water_calls == 1, "a sum of exactly 1 is accepted");
    return failures ? 1 : 0;
}
