#include <cstdio>
#include <cstring>
struct CallRecord {
    void* state; float step; void* point3; void* point2; short* extra;
};
static CallRecord last_advance;
static int advance_calls, advance_result;
static int trig_angles[64], trig_calls;
extern "C" int animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps(void* state, float step, void* point3, void* point2, short* extra) {
    ++advance_calls;
    last_advance = {state, step, point3, point2, extra};
    return advance_result;
}
extern "C" float icos__Fi(int angle) { trig_angles[trig_calls++] = angle; return 0.5f; }
extern "C" float isin__Fi(int angle) { trig_angles[trig_calls++] = angle; return 0.25f; }
#include "creature_advance_animation.cpp"
static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
template <class T> static T& at(unsigned char* object, unsigned offset) { return *reinterpret_cast<T*>(object + offset); }
int main() {
    alignas(16) unsigned char object[1024];
    const float sixtieth = __builtin_bit_cast(float, 0x3c888888u);

    // No animation state word: no advance call, the motion is still scaled and rotated.
    std::memset(object, 0, sizeof object);
    at<float>(object, 180) = 2.0f; at<float>(object, 184) = 4.0f; at<float>(object, 224) = 1.0f;
    at<short>(object, 56) = 16384 + 100;
    trig_calls = 0; advance_calls = 0;
    int r = creatureAdvanceAnimation__8Creaturef(object, 1.5f);
    expect(r == 0 && advance_calls == 0, "no advance without the +236 word");
    expect(trig_calls == 2 && trig_angles[0] == 100 && trig_angles[1] == 100, "trig angle is the halfword at +56 minus 16384");
    expect(at<float>(object, 180) == 0.0f && at<float>(object, 184) == -2.5f, "rotation: (-x)c + ys, (-x)s - yc");

    // Scale first, then rotate.
    at<float>(object, 180) = 2.0f; at<float>(object, 184) = 4.0f; at<float>(object, 224) = 2.0f;
    creatureAdvanceAnimation__8Creaturef(object, 1.0f);
    expect(at<float>(object, 180) == (-4.0f) * 0.5f + 8.0f * 0.25f && at<float>(object, 184) == (-4.0f) * 0.25f - 8.0f * 0.5f, "scale at +224 applies before the rotation");

    // With the state word: the advance call, return value, slow-down byte.
    std::memset(object, 0, sizeof object);
    at<int>(object, 236) = 1; at<float>(object, 224) = 1.0f; at<float>(object, 180) = 2.0f; at<float>(object, 184) = 4.0f;
    at<short>(object, 188) = 100; object[800] = 30;
    advance_result = 9;
    r = creatureAdvanceAnimation__8Creaturef(object, 2.0f);
    expect(r == 9 && advance_calls == 1, "returns the advance result");
    expect(last_advance.state == object + 288 && last_advance.point3 == object + 16 && last_advance.point2 == object + 180 && last_advance.extra == reinterpret_cast<short*>(object + 188), "advance arguments");
    expect(last_advance.step == 2.0f * static_cast<float>(60 - 30) * sixtieth, "step is slowed by (60 - count) / 60");
    const float k = static_cast<float>(60 - 30) * sixtieth;
    expect(at<float>(object, 180) == 0.0f * k && at<float>(object, 184) == -2.5f * k, "motion is shrunk by k");
    expect(at<short>(object, 188) == static_cast<short>(static_cast<int>(100.0f * k)), "the halfword at +188 is shrunk and truncated");

    // Bit 0x02000000 of the doubleword at +160 hides the position argument; count 0 and 61 are not slowed.
    at<unsigned long long>(object, 160) = 0x02000000ull;
    object[800] = 61;
    at<short>(object, 188) = 100;
    creatureAdvanceAnimation__8Creaturef(object, 2.0f);
    expect(last_advance.point3 == nullptr && last_advance.step == 2.0f, "flag clears the position argument; count 61 leaves the step alone");
    expect(at<short>(object, 188) == 100, "count 61 leaves the halfword alone");
    object[800] = 0;
    creatureAdvanceAnimation__8Creaturef(object, 3.0f);
    expect(last_advance.step == 3.0f, "count 0 leaves the step alone");
    return failures;
}
