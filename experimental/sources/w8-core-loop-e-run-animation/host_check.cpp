// Host check for runAnimation__Fv. Not an EE plan input.
#include <cmath>
#include <cstring>
#include <string>

extern "C" unsigned char* dialogCreature = nullptr;
#include "run_animation.cpp"

static std::string trace;
static int found_index = 5;
static int amount_value = 1024;
extern "C" int animFindComponentWithFlags__FP14AnimationStatei(unsigned char* state, int flags) {
    trace += "A" + std::to_string(flags) + ";";
    if (state != dialogCreature + 288) trace += "?";
    return found_index;
}
extern "C" int soundGetStreamAmountPlayed__Fi(int stream) {
    trace += "S" + std::to_string(stream) + ";";
    return amount_value;
}
static float fget(unsigned char* p) { float f; std::memcpy(&f, p, 4); return f; }
static void fput(unsigned char* p, float f) { std::memcpy(p, &f, 4); }
static bool near(float a, float b) { return std::fabs(a - b) < 1e-5f; }

int main() {
    alignas(16) static unsigned char creature[1024];
    // Null creature: nothing at all.
    runAnimation__Fv();
    if (!trace.empty()) return 1;
    dialogCreature = creature;
    // Negative component index: stops before reading the stream.
    found_index = -1;
    runAnimation__Fv();
    if (trace != "A32;") return 2;
    trace.clear();
    found_index = 5;
    unsigned char* state = creature + 288;
    state[4] = 3;                       // three entries
    for (int i = 0; i < 3; ++i) fput(state + 32 + i * 32, 10.0f * (i + 1));
    // The component at index 1 has its float at creature+320+160.
    // scaled*0.016666668-0.07 target; expected = (amount-1024)*7.29167e-5.
    fput(creature + 320 + 160, 3.0f);     // target = 0.05-0.07 = -0.02
    amount_value = 1024 + 200;           // expected = 0.014583; diff from -0.02 is 0.0346 > 0.02, expected above target -> +0.1
    runAnimation__Fv();
    if (trace != "A32;S2;") return 3;
    if (!near(fget(state + 32), 10.1f) || !near(fget(state + 64), 20.1f) || !near(fget(state + 96), 30.1f)) return 4;
    // Same amount again: no recomputation, but the pending correction is applied again.
    trace.clear();
    runAnimation__Fv();
    if (!near(fget(state + 32), 10.2f)) return 5;
    // Different amount close to the target: correction cleared, nothing added.
    amount_value = 1024 - 274;           // expected = -0.0200 ; diff ~ 0.0000 -> within 0.02
    runAnimation__Fv();
    if (!near(fget(state + 32), 10.2f)) return 6;
    // Expected well below target: -0.1. Target is -0.02; expected = -0.0729 diff 0.0529.
    amount_value = 1024 - 1000;          // expected = -0.0729
    runAnimation__Fv();
    if (!near(fget(state + 32), 10.1f)) return 7;
    // Count byte is signed: a negative count adds nothing.
    state[4] = (unsigned char)-1;
    runAnimation__Fv();
    if (!near(fget(state + 32), 10.1f)) return 8;
    return 0;
}
