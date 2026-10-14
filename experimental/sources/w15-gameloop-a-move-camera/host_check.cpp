// Host check for moveCamera__Fv. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -DNORRATH_HOST_ONLY host_check.cpp
#include "move_camera.cpp"

#include <cstdio>
#include <string>
#include <vector>

extern "C" {
int src, dst;
float hermiteTime, delta, hermDeltaScale;
float p1[3], p2[3], v1[3], v2[3];
float lightInfo[32], lightMats[36];
short yaws[4];
unsigned short pitch1, pitch2;
void* gameCam = reinterpret_cast<void*>(0x10);
}
static std::vector<std::string> events;
static int initCalls;
extern "C" {
void getTweeks__FiPsT1P6Point3(int index, short* yaw, short* pitch, float* vector) {
    events.push_back("tweek" + std::to_string(index));
    *yaw = 100; *pitch = 200; vector[0] = 1.0f; vector[1] = 2.0f; vector[2] = 3.0f;
}
void evalHermite3D__FfP6Point3N41(float t, const float* a, const float* b, const float*, const float*, float* out) {
    events.push_back("h3d:" + std::to_string(t) + ":" + std::to_string(a[0]) + ":" + std::to_string(b[2]));
    out[0] = 5.0f; out[1] = 6.0f; out[2] = 7.0f;
}
float evalHermite__Ffffff(float t, float a, float b, float c, float d) {
    events.push_back("herm:" + std::to_string(a) + std::to_string(b) + std::to_string(c) + std::to_string(d));
    return t > 0.9f ? 1.0f : 0.5f;
}
void matRot__FP8Matrix33is(float*, int axis, short angle) { events.push_back("rot:" + std::to_string(axis) + ":" + std::to_string(angle)); }
void matTransform__FP6Point3P8Matrix33T0(float* out, float*, const float* in) { out[0] = in[0]; out[1] = in[1]; out[2] = in[2]; }
int lerpShortRot__Fssfff(short a, short b, float t, float, float) { events.push_back("lerp:" + std::to_string(a) + ":" + std::to_string(b) + ":" + std::to_string(t)); return a + b; }
void SetYaw__6Camerai(void*, int yaw) { events.push_back("yaw:" + std::to_string(yaw)); }
void SetPitch__6Camerai(void*, int pitch) { events.push_back("pitch:" + std::to_string(pitch)); }
void BuildMatrix__6CamerafffP6Point3(void*, float x, float y, float z, float* point) {
    events.push_back("build:" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + (point ? ":p" : ":null"));
}
void lightSetSceneLights__FG6Point3iiiiiiiii(const void* d, int a, int b, int c, int dd, int e, int f, int g, int h, int i) {
    const float* v = static_cast<const float*>(d);
    events.push_back("light:" + std::to_string(v[0]) + "," + std::to_string(v[1]) + "," + std::to_string(v[2]) + ":" + std::to_string(a) + "," + std::to_string(b) + "," +
                     std::to_string(c) + "," + std::to_string(dd) + "," + std::to_string(e) + "," + std::to_string(f) + "," + std::to_string(g) + "," + std::to_string(h) + "," + std::to_string(i));
}
void initPosition__Fii(int current, int target) { ++initCalls; events.push_back("init:" + std::to_string(current) + "," + std::to_string(target)); }
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

int main() {
    src = 1; dst = 2;
    for (int i = 0; i < 32; ++i) lightInfo[i] = static_cast<float>(i);
    // record 1 colours are 8..15 -> fields +8 +12 +16 +20 +24 +28 are indices 10 11 12 13 14 15; record 2 are 18 19 20 21 22 23
    yaws[1] = 1000; yaws[2] = 2000; pitch1 = 30; pitch2 = 40;
    p1[0] = 10.0f; p2[2] = 20.0f;
    hermiteTime = 0.5f; delta = 0.0f; hermDeltaScale = 1.0f;
    CHECK(moveCamera__Fv() == 0);
    CHECK(hermiteTime == 0.5f && delta == 0.0f && initCalls == 0);
    // idle step: the Hermite end points include the tweaks, the blend factor is 0.5 and the camera gets lerped angles
    CHECK(events[0] == "tweek1" && events[1] == "tweek2");
    CHECK(events[2] == "h3d:0.500000:11.000000:23.000000");
    bool sawYaw = false, sawPitch = false, sawBuild = false, sawLight = false;
    for (const std::string& e : events) {
        if (e == "lerp:1100:2100:0.500000") sawYaw = true;
        if (e == "lerp:230:240:0.500000") sawPitch = true;
        if (e == "build:5.000000,6.000000,7.000000:null") sawBuild = true;
        if (e == "light:0.000000,-1.000000,0.000000:14,15,16,0,17,18,19,0,128") sawLight = true;   // lerp(10..., 18...) at 0.5, rounded
    }
    CHECK(sawYaw && sawPitch && sawBuild);
    CHECK(sawLight);
    // a forward step past the end: equal ends make a positive delta start at 1, the step overflows and resets
    events.clear();
    src = dst = 3; hermiteTime = 0.25f; delta = 2.0f;
    CHECK(moveCamera__Fv() == 1);
    CHECK(initCalls == 1 && hermiteTime == 0.0f && delta == 0.0f);
    CHECK(events.back() == "build:5.000000,6.000000,7.000000:null" || events.back() == "init:3,3");
    // a negative delta at equal ends starts at 0 and the step underflows
    events.clear(); initCalls = 0;
    hermiteTime = 0.75f; delta = -1.0f;
    CHECK(moveCamera__Fv() == 1);
    CHECK(initCalls == 0 && hermiteTime == 0.0f && delta == 0.0f);
    // a partial step advances by delta * scale * 0.99999994
    events.clear(); src = 0; dst = 1; hermiteTime = 0.25f; delta = 0.5f; hermDeltaScale = 0.5f;
    CHECK(moveCamera__Fv() == 1);
    CHECK(hermiteTime >= 0.49999997f && hermiteTime <= 0.5f && delta == 0.5f);
    std::puts("ok");
    return 0;
}
