// Host contract for fxRunCreatureFire. Not an EE plan input. Build with -ffp-contract=off.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
struct Point3;
static std::vector<std::string> events;
static int closeResult, shadowCount;
static float heightOut;
extern "C" {
int eRandState;
unsigned char cFireParticle[80];
unsigned char cSmokeParticle[80];
}
#include "fx_run_creature_fire.cpp"
extern "C" {
int gameCloseToCamera__FP6Point3(Point3* p) { events.push_back("close " + std::to_string(reinterpret_cast<float*>(p)[0])); return closeResult; }
int modelGetNmShadowPoints__FP12_modelHeader(void*) { return shadowCount; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void*, void*, Point3, short value, int index, Point3* position, float* height, void* matrix, float scale) {
    events.push_back("pos " + std::to_string(value) + " " + std::to_string(index) + " " + (matrix ? "m" : "null") + " " + std::to_string(scale));
    float* p = reinterpret_cast<float*>(position);
    p[0] = 100.0f; p[1] = 200.0f; p[2] = -0.0f;
    *height = heightOut;
}
float icos__Fi(int) { return 1.0f; }
float isin__Fi(int) { return 0.5f; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* def, Point3* position, Point3* velocity, float life) {
    const float* p = reinterpret_cast<const float*>(position);
    const float* v = reinterpret_cast<const float*>(velocity);
    float speed2 = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    events.push_back(std::string("add ") + (def == cFireParticle ? "fire" : def == cSmokeParticle ? "smoke" : "?") + " " +
                     std::to_string(life >= 0.0f && life < 0.3f ? 1 : 0) + " " + std::to_string(speed2 <= 100.01f ? 1 : 0) + " " +
                     std::to_string(p[1] > 200.0f - 1.0f ? 1 : 0));
}
}
int main() {
    Point3 here = {1.0f, 2.0f, 3.0f};
    auto call = [&](void* model, float scale, float push) {
        events.clear();
        fxRunCreatureFire__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStateffff(model, nullptr, here, 5, nullptr, 9.0f, scale, push, 9.0f);
    };
    char model;
    eRandState = 12345; shadowCount = 3; heightOut = 2.0f;
    closeResult = 0; call(&model, 1.0f, 1.0f);
    if (events.size() != 1) return 1;
    closeResult = 1; call(nullptr, 1.0f, 1.0f);
    if (events.size() != 1) return 2;
    shadowCount = 0; call(&model, 1.0f, 1.0f);
    if (events.size() != 1) return 3;
    shadowCount = 3;
    // Scale 0.1: 0.1 * 16.59 + 0.5 truncates to 2 fire repetitions; 0.1 * 3.76 + 0.5 gives 0 smoke.
    call(&model, 0.1f, 0.0f);
    int fire = 0, smoke = 0, pos = 0;
    for (const auto& e : events) { fire += e.rfind("add fire 1 1 1", 0) == 0; smoke += e.rfind("add smoke", 0) == 0; pos += e.rfind("pos 5 ", 0) == 0; }
    if (fire != 2 || smoke != 0 || pos != 2) { std::printf("fire %d smoke %d pos %d\n", fire, smoke, pos); return 4; }
    // Scale 1: 17 fire (16.59 + .5) and 4 smoke (3.76 + .5) repetitions, one particle each.
    call(&model, 1.0f, 0.0f);
    fire = smoke = 0;
    for (const auto& e : events) { fire += e.rfind("add fire", 0) == 0; smoke += e.rfind("add smoke", 0) == 0; }
    if (fire != 17 || smoke != 4) { std::printf("fire %d smoke %d\n", fire, smoke); return 5; }
    // A tall height repeats the particle: 12000^2 = 1.44e8 needs two subtractions of 1.145e8.
    heightOut = 12000.0f;
    call(&model, 0.06f, 0.0f);   // one fire repetition
    fire = 0;
    for (const auto& e : events) fire += e.rfind("add fire", 0) == 0;
    if (fire != 2) { std::printf("fire %d\n", fire); return 6; }
    // Zero or negative height emits nothing; negative scale emits nothing.
    heightOut = 0.0f; call(&model, 1.0f, 0.0f);
    for (const auto& e : events) if (e.rfind("add", 0) == 0) return 7;
    heightOut = 5.0f; call(&model, -1.0f, 0.0f);
    for (const auto& e : events) if (e.rfind("pos", 0) == 0) return 8;
    std::puts("fxRunCreatureFire contract passed");
    return 0;
}
