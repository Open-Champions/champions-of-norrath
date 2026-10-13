// Host contract for rayHit__7VehicleP6Point3T1fT1. Not an EE plan input.
#include "ray_hit.cpp"

#include <cstdio>

unsigned char* world;
static float forbidden_result, height_result;
static int forbidden_calls, height_calls;
static unsigned char* seen_world[2];
static float seen_point[2][3], seen_radius[2];

extern "C" float worldTestForbidden__FP12_worldHeaderG6Point3f(unsigned char* w, const float* p, float r) {
    ++forbidden_calls; seen_world[0] = w; seen_radius[0] = r;
    for (int i = 0; i < 3; ++i) seen_point[0][i] = p[i];
    return forbidden_result;
}
extern "C" float worldFindHeight__FP12_worldHeaderG6Point3f(unsigned char* w, const float* p, float r) {
    ++height_calls; seen_world[1] = w; seen_radius[1] = r;
    for (int i = 0; i < 3; ++i) seen_point[1][i] = p[i];
    return height_result;
}

int main() {
    int failures = 0;
    unsigned char marker;
    world = &marker;
    const Point3 pt = {1.5f, -2.5f, 40.0f};
    Point3 out = {7, 8, 9};
    forbidden_result = 1.0f;
    float r = rayHit__7VehicleP6Point3T1fT1(nullptr, &pt, nullptr, 2.0f, &out);
    if (r != -9999.0f || height_calls != 0 || out.x != 7 || out.y != 8 || out.z != 9) { std::fprintf(stderr, "fail: forbidden\n"); ++failures; }
    forbidden_result = 0.0f; height_result = 10.0f;
    r = rayHit__7VehicleP6Point3T1fT1(nullptr, &pt, nullptr, 2.0f, &out);
    if (r != 15.0f) { std::fprintf(stderr, "fail: result %g\n", r); ++failures; }
    if (out.x != 0 || out.y != 0 || out.z != 1) { std::fprintf(stderr, "fail: out\n"); ++failures; }
    for (int k = 0; k < 2; ++k) {
        if (seen_world[k] != &marker || seen_radius[k] != 16.0f || seen_point[k][0] != 1.5f || seen_point[k][1] != -2.5f || seen_point[k][2] != 40.0f) {
            std::fprintf(stderr, "fail: call %d args\n", k); ++failures;
        }
    }
    forbidden_result = 2.0f;  // only exactly 1.0 rejects
    r = rayHit__7VehicleP6Point3T1fT1(nullptr, &pt, nullptr, 4.0f, &out);
    if (r != 7.5f) { std::fprintf(stderr, "fail: non-1 forbidden value\n"); ++failures; }
    return failures ? 1 : 0;
}
