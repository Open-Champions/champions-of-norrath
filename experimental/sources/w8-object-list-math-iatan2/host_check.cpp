// Host contract for iatan2__Fff. Not an EE plan input. The host converts by
// truncation; the EE cvt.w.s rounding behavior is not exercised here.
#include <cmath>
#include <cstdio>

static float last_y, last_x;
static float answer;
extern "C" float norrath_native_atan2f(float y, float x) { last_y = y; last_x = x; return answer; }
extern "C" short iatan2__Fff(float, float);

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    answer = 0.0f;
    expect(iatan2__Fff(3.0f, 4.0f) == 0 && last_y == 3.0f && last_x == 4.0f, "argument order is y then x");
    answer = 1.0f;
    expect(iatan2__Fff(0, 0) == 10430, "one radian truncates to 10430");
    answer = 3.14159265f;
    expect(iatan2__Fff(0, 0) == static_cast<short>(32768), "pi wraps to the signed halfword minimum");
    answer = -3.14159265f;
    expect(iatan2__Fff(0, 0) == -32768, "-pi is the halfword minimum");
    answer = -1.0f;
    expect(iatan2__Fff(0, 0) == -10430, "negative angles truncate toward zero");
    answer = 3.0f;
    expect(iatan2__Fff(0, 0) == 31291, "3.0 stays inside the halfword");
    answer = 4.0f;
    expect(iatan2__Fff(0, 0) == -23815, "4.0 keeps the low 16 bits (41721 - 65536)");
    answer = 6.2831853f;
    expect(iatan2__Fff(0, 0) == 0 || iatan2__Fff(0, 0) == -1,
           "a full turn lands on zero or one count short, by single-float rounding");
    if (failures) return 1;
    std::printf("iatan2 contract passed\n");
    return 0;
}
