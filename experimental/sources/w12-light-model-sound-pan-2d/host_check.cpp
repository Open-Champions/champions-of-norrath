// Host contract for soundPan2D__FiP6Point3f. Not an EE plan input.
#include <cstdio>
struct Point3 { float x, y, z; };
static int panCalls, volumeCalls, volumeId;
static Point3 seenPoint;
static float seenA, seenB, seenFirst, seenSecond;
extern "C" {
void pan__FG6Point3ffPfT3(Point3 p, float a, float b, float* first, float* second) {
    ++panCalls; seenPoint = p; seenA = a; seenB = b;
    *first = p.x + 1.0f; *second = p.z + 2.0f;
}
void soundChangeVolume__Fiff(int id, float first, float second) { ++volumeCalls; volumeId = id; seenFirst = first; seenSecond = second; }
void soundPan2D__FiP6Point3f(int, Point3*, float);
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    Point3 p{3.0f, 4.0f, 5.0f};
    soundPan2D__FiP6Point3f(-1, &p, 1.0f);
    expect(panCalls == 0 && volumeCalls == 0, "id -1 does nothing");
    soundPan2D__FiP6Point3f(7, &p, 0.5f);
    expect(panCalls == 1 && volumeCalls == 1, "one call each");
    expect(volumeId == 7, "id passed on");
    expect(seenPoint.x == 3.0f && seenPoint.y == 4.0f && seenPoint.z == 5.0f, "point passed");
    expect(seenA == 180.0f && seenB == 420.0f, "scaled floats");
    expect(seenFirst == 4.0f && seenSecond == 7.0f, "pan results forwarded in order");
    soundPan2D__FiP6Point3f(0, &p, 2.0f);
    expect(volumeId == 0 && seenA == 720.0f && seenB == 1680.0f, "id 0 and f = 2");
    if (failures) return 1;
    std::printf("soundPan2D contract passed\n");
    return 0;
}
