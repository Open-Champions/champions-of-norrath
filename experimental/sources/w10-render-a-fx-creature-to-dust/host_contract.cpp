// Host contract for fxCreatureToDust. Not an EE plan input.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
static std::vector<std::string> events;
static int shadowCount;
extern "C" {
int modelGetNmShadowPoints__FP12_modelHeader(unsigned char*) { return shadowCount; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    unsigned char*, unsigned char*, const unsigned char* point, short value, int index,
    float* position, float* height, unsigned char* matrix, float scale) {
    float p[3]; std::memcpy(p, point, 12);
    events.push_back("pos " + std::to_string(p[0]) + " " + std::to_string(value) + " " + std::to_string(index) +
                     " " + (matrix ? "m" : "null") + " " + std::to_string(scale));
    position[0] = 10.0f * index; position[1] = 1.0f; position[2] = 2.0f;
    *height = 5.0f;
}
void addEffect__FiG6Point3(int kind, const float* position) {
    events.push_back("fx " + std::to_string(kind) + " " + std::to_string(position[0]));
}
}
#include "fx_creature_to_dust.cpp"
int main() {
    float point[3] = {4.0f, 5.0f, 6.0f};
    shadowCount = 0; fxCreatureToDust__FP12_modelHeaderP14AnimationStateG6Point3sff(nullptr, nullptr, (unsigned char*)point, 3, 0.5f, 9.0f);
    if (!events.empty()) return 1;
    shadowCount = -2; fxCreatureToDust__FP12_modelHeaderP14AnimationStateG6Point3sff(nullptr, nullptr, (unsigned char*)point, 3, 0.5f, 9.0f);
    if (!events.empty()) return 2;
    shadowCount = 2; fxCreatureToDust__FP12_modelHeaderP14AnimationStateG6Point3sff(nullptr, nullptr, (unsigned char*)point, -7, 0.5f, 9.0f);
    const char* want[] = {"pos 4.000000 -7 0 null 0.500000", "fx 2 0.000000", "pos 4.000000 -7 1 null 0.500000", "fx 2 10.000000"};
    if (events.size() != 4) return 3;
    for (int i = 0; i != 4; ++i) if (events[i] != want[i]) { std::printf("%d: %s\n", i, events[i].c_str()); return 4; }
    std::puts("fxCreatureToDust contract passed");
    return 0;
}
