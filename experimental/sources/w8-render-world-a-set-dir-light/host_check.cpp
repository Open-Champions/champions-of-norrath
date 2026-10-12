#include <cstdio>
#include <vector>
struct Point3;
static int noise_argument;
static std::vector<int> trig_arguments;
static int calls_secondary, calls_primary;
static float seen_direction[3];
static short seen_color[4];
extern "C" {
int frameNm = 3;
float ef1Noise__Fi(int index) { noise_argument = index; return 0.5f; }
float icos__Fi(int angle) { trig_arguments.push_back(angle); return angle == 10 ? 0.5f : 0.25f; }
float isin__Fi(int angle) { trig_arguments.push_back(angle); return angle == 10 ? 0.25f : 0.5f; }
}
#include "set_dir_light.cpp"
extern "C" {
void lightSetSecondaryDirectional__FG6Point3Ps(Point3* direction, short* color) {
    ++calls_secondary;
    const float* v = reinterpret_cast<const float*>(direction);
    seen_direction[0] = v[0]; seen_direction[1] = v[1]; seen_direction[2] = v[2];
    for (int i = 0; i < 4; ++i) seen_color[i] = color[i];
}
void lightSetPrimaryDirectional__FG6Point3Ps(Point3* direction, short* color) {
    ++calls_primary;
    const float* v = reinterpret_cast<const float*>(direction);
    seen_direction[0] = v[0]; seen_direction[1] = v[1]; seen_direction[2] = v[2];
    for (int i = 0; i < 4; ++i) seen_color[i] = color[i];
}
}
static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
int main() {
    // yaw 10: cos 0.5, sin 0.25. pitch 20: cos 0.25, sin 0.5. amount 0.5 * noise 0.5 = 0.25: colours scaled by 0.75.
    setDirLight__Fsssssfb(10, 20, 100, 200, 40, 0.5f, 1);
    expect(noise_argument == 0x007efe41 + (3 << 4), "noise index from frameNm");
    expect(calls_secondary == 1 && calls_primary == 0, "nonzero flag: secondary light");
    expect(seen_color[0] == 75 && seen_color[1] == 150 && seen_color[2] == 30 && seen_color[3] == 0, "colours times 0.75, truncated");
    // heading (0.5, 0.25, 0), flat = heading * 0.25, direction = (flat.x, flat.y, 0 + 0.5)
    expect(seen_direction[0] == 0.125f && seen_direction[1] == 0.0625f && seen_direction[2] == 0.5f, "direction from yaw and pitch");
    setDirLight__Fsssssfb(10, 20, 3, 3, 3, 0.0f, 0);
    expect(calls_primary == 1 && seen_color[0] == 3, "zero flag: primary light, no flicker");
    return failures;
}
