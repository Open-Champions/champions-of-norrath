#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
static std::vector<std::string> events;
static int shadowCount;
static float* matrixA;
static unsigned char* lastModel;
static unsigned char* lastState;
extern "C" {
void modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(unsigned char* model, unsigned char* state, int attach, float* matrix, float scale) {
    lastModel = model; lastState = state; matrixA = matrix;
    events.push_back("attach " + std::to_string(attach) + " " + std::to_string(scale));
    for (int i = 0; i != 12; ++i) matrix[i] = 1.0f + i;
}
void matMakeTransYaw__FP8Matrix34G6Point3s(float* matrix, const float* point, short yaw) {
    events.push_back("yaw " + std::to_string(point[0]) + " " + std::to_string(point[2]) + " " + std::to_string(yaw));
    for (int i = 0; i != 12; ++i) matrix[i] = 100.0f + i;
}
void matMul__FP8Matrix34N20(float* d, float* l, float* r) {
    events.push_back(std::string("mul ") + (d == l && l == matrixA && r != l ? "in-place" : "other") + " " + std::to_string(l[0]) + " " + std::to_string(r[0]));
    for (int i = 0; i != 12; ++i) d[i] = l[i] + r[i];
}
int modelGetNmShadowPoints__FP12_modelHeader(unsigned char*) { events.push_back("count"); return shadowCount; }
void modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(unsigned char*, float* matrix, int index, float* position, float* height) {
    events.push_back("pos " + std::to_string(index) + " " + std::to_string(matrix[0]) + " " + (matrix == matrixA ? "A" : "?"));
    position[0] = 10.0f * index; position[1] = 1; position[2] = 2; *height = 3;
}
void addEffect__FiG6Point3(int kind, const float* position) {
    events.push_back("fx " + std::to_string(kind) + " " + std::to_string(position[0]));
}
void fxWeaponToSparkles__FP12_modelHeaderT0P14AnimationStateG6Point3sffi(unsigned char* m1, unsigned char* m2, unsigned char* state, const float* point, short yaw, float scale, float unused, int bone);
}
int main() {
    unsigned char model1[64] = {}, model2[64] = {}, state[16] = {};
    model2[19] = 0xfe;  // signed byte -2
    float point[3] = {4.0f, 5.0f, 6.0f};
    shadowCount = 2;
    fxWeaponToSparkles__FP12_modelHeaderT0P14AnimationStateG6Point3sffi(model1, model2, state, point, 0x7fff, 0.5f, 9.0f, 7);
    if (lastModel != model1 || lastState != state) return 1;
    const char* want[] = {"attach 5 0.500000", "yaw 4.000000 6.000000 -32769 ", "mul in-place 1.000000 100.000000", "count",
                          "pos 0 101.000000 A", "fx 14 0.000000", "pos 1 101.000000 A", "fx 14 10.000000"};
    // yaw + 0x4000 wraps to a short: 0x7fff + 0x4000 = 0xbfff = -16385.
    want[1] = "yaw 4.000000 6.000000 -16385";
    if (events.size() != 8) { std::printf("%zu events\n", events.size()); return 2; }
    for (int i = 0; i != 8; ++i)
        if (events[i] != want[i]) { std::printf("%d: %s\n", i, events[i].c_str()); return 3; }
    events.clear(); shadowCount = 0;
    fxWeaponToSparkles__FP12_modelHeaderT0P14AnimationStateG6Point3sffi(model1, model2, state, point, 0, 0.5f, 9.0f, 0);
    if (events.size() != 4 || events[3] != "count") return 4;
    events.clear(); shadowCount = -3;
    fxWeaponToSparkles__FP12_modelHeaderT0P14AnimationStateG6Point3sffi(model1, model2, state, point, 0, 0.5f, 9.0f, 0);
    if (events.size() != 4) return 5;
    std::puts("fxWeaponToSparkles contract passed");
    return 0;
}
