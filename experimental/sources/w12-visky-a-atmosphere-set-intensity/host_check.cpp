// Host check only; not a build input. Native build: c++ -std=c++17 -ffp-contract=off host_check.cpp
#include "set_intensity.cpp"
#include <cstring>

static int clears;
static float seen_at_call;
static unsigned char* seen_self;
static unsigned char object[512];
extern "C" void ClearAllSplashes__12VIAtmosphere(unsigned char* self) {
    ++clears;
    seen_self = self;
    std::memcpy(&seen_at_call, self + 72, 4);
}

static float slot() { float f; std::memcpy(&f, object + 72, 4); return f; }
static void arm(float old) {
    std::memset(object, 0x21, sizeof object);
    std::memcpy(object + 72, &old, 4);
    clears = 0;
}

int main() {
    // Off to on: one clear, issued before the store (it still sees the old value).
    arm(0.0f);
    SetIntensity__12VIAtmospheref(object, 0.75f);
    if (clears != 1 || seen_self != object || seen_at_call != 0.0f || slot() != 0.75f) return 1;
    arm(-0.0f);
    SetIntensity__12VIAtmospheref(object, 2.0f);
    if (clears != 1 || slot() != 2.0f) return 2;
    // Already on, or negative old value: no clear.
    arm(0.5f); SetIntensity__12VIAtmospheref(object, 1.0f);
    if (clears != 0 || slot() != 1.0f) return 3;
    arm(-1.0f); SetIntensity__12VIAtmospheref(object, 1.0f);
    if (clears != 0 || slot() != 1.0f) return 4;
    // Non-positive argument: store, no clear, even from zero.
    arm(0.0f); SetIntensity__12VIAtmospheref(object, 0.0f);
    if (clears != 0 || slot() != 0.0f) return 6;
    arm(0.0f); SetIntensity__12VIAtmospheref(object, -3.0f);
    if (clears != 0 || slot() != -3.0f) return 7;
    // Only the one word changes.
    arm(0.0f);
    unsigned char want[512];
    std::memcpy(want, object, sizeof want);
    SetIntensity__12VIAtmospheref(object, 0.25f);
    float v = 0.25f;
    std::memcpy(want + 72, &v, 4);
    return std::memcmp(object, want, sizeof want) == 0 ? 0 : 9;
}
