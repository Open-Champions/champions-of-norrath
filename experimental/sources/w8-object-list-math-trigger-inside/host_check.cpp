// Host contract for objectInside__7TriggerP10GameObject. Not an EE plan input.
#include <cmath>
#include <cstdio>
struct TriggerBounds { unsigned char before[76]; float lower_a, lower_b, upper_a, upper_b; };
struct ObjectPlane { unsigned char before[16]; float a, b; };
extern "C" int objectInside__7TriggerP10GameObject(const TriggerBounds*, const ObjectPlane*);

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    TriggerBounds t = {};
    t.lower_a = -10; t.lower_b = 20; t.upper_a = 30; t.upper_b = 60;
    ObjectPlane o = {};
    auto inside = [&](float a, float b) { o.a = a; o.b = b; return objectInside__7TriggerP10GameObject(&t, &o); };
    expect(inside(0, 40) == 1, "interior point");
    expect(inside(-10, 20) == 1 && inside(30, 60) == 1 && inside(-10, 60) == 1 && inside(30, 20) == 1, "all four corners are inclusive");
    expect(inside(-10.01f, 40) == 0 && inside(30.01f, 40) == 0, "a outside either side");
    expect(inside(0, 19.99f) == 0 && inside(0, 60.01f) == 0, "b outside either side");
    expect(inside(NAN, 40) == 0 && inside(0, NAN) == 0, "NaN is outside");
    t.lower_a = NAN;
    expect(inside(0, 40) == 0, "NaN bound is outside");
    t.lower_a = -10; t.upper_a = -20;
    expect(inside(-15, 40) == 0, "an inverted range contains nothing");
    // a is tested against lower_a/upper_a and b against lower_b/upper_b, not swapped.
    t.upper_a = 30; t.lower_a = 0; t.lower_b = 0; t.upper_b = 5;
    expect(inside(25, 3) == 1 && inside(3, 25) == 0, "axes are not swapped");
    if (failures) return 1;
    std::printf("objectInside contract passed\n");
    return 0;
}
