// Host check only; not a build input. Native build: c++ -std=c++17 -ffp-contract=off host_check.cpp
#include "set_lavastorm_intensity.cpp"
#include <cstring>

static int clears;
static float seen_at_call;
static unsigned char* seen_self;
static unsigned char object[8192];
extern "C" void ClearAllFireballs__12VIAtmosphere(unsigned char* self) {
    ++clears;
    seen_self = self;
    std::memcpy(&seen_at_call, self + 76, 4);
}

static float slot() { float f; std::memcpy(&f, object + 76, 4); return f; }
static void arm(float old, int a, int b) {
    std::memset(object, 0x21, sizeof object);
    std::memcpy(object + 76, &old, 4);
    std::memcpy(object + 4480, &a, 4);
    std::memcpy(object + 4484, &b, 4);
    clears = 0;
}

int main() {
    arm(0.0f, 3, 4);
    SetLavastormIntensity__12VIAtmospheref(object, 0.5f);
    if (clears != 1 || seen_self != object || seen_at_call != 0.0f || slot() != 0.5f) return 1;
    // Either marker word equal to -1 stops everything: no store, no clear.
    arm(0.0f, -1, 4); SetLavastormIntensity__12VIAtmospheref(object, 0.5f);
    if (clears != 0 || slot() != 0.0f) return 2;
    arm(0.0f, 3, -1); SetLavastormIntensity__12VIAtmospheref(object, 0.5f);
    if (clears != 0 || slot() != 0.0f) return 3;
    arm(0.0f, -1, -1); SetLavastormIntensity__12VIAtmospheref(object, 0.5f);
    if (clears != 0 || slot() != 0.0f) return 4;
    // Zero words are valid.
    arm(0.0f, 0, 0); SetLavastormIntensity__12VIAtmospheref(object, 0.5f);
    if (clears != 1 || slot() != 0.5f) return 5;
    // Already on: store, no clear.
    arm(0.25f, 3, 4); SetLavastormIntensity__12VIAtmospheref(object, 0.5f);
    if (clears != 0 || slot() != 0.5f) return 6;
    // Non-positive argument from zero: store, no clear.
    arm(0.0f, 3, 4); SetLavastormIntensity__12VIAtmospheref(object, 0.0f);
    if (clears != 0 || slot() != 0.0f) return 7;
    arm(0.0f, 3, 4); SetLavastormIntensity__12VIAtmospheref(object, -2.0f);
    if (clears != 0 || slot() != -2.0f) return 8;
    // Nothing but the one float changes.
    arm(0.0f, 3, 4);
    unsigned char want[sizeof object];
    std::memcpy(want, object, sizeof want);
    SetLavastormIntensity__12VIAtmospheref(object, 1.0f);
    float v = 1.0f;
    std::memcpy(want + 76, &v, 4);
    return std::memcmp(object, want, sizeof want) == 0 ? 0 : 9;
}
