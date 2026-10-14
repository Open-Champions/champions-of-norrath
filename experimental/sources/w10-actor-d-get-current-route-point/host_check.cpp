// Host contract for getCurrentRoutePoint__8Creature; not an EE input. Includes the source.
#include "route_point.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::printf("fail: %s\n", what); ++failures; } }
int main() {
    alignas(16) static unsigned char object[1000];
    int* table = reinterpret_cast<int*>(object + 580);
    int* index = reinterpret_cast<int*>(object + 708);
    for (int i = 0; i < 16; ++i) { table[2 * i] = 100 + i; table[2 * i + 1] = -200 - i; }
    for (int i : {0, 1, 7, 15}) {
        *index = i; Point3 r = {9, 9, 9};
        Point3* out = getCurrentRoutePoint__8Creature(&r, object);
        expect(out == &r && r.x == float(100 + i) && r.y == float(-200 - i) && r.z == -1.0f, "entry i");
    }
    for (int i : {16, 17, 1000}) {
        *index = i; Point3 r = {9, 9, 9};
        getCurrentRoutePoint__8Creature(&r, object);
        expect(r.x == -1.0f && r.y == -1.0f && r.z == -1.0f, "index >= 16 gives all -1");
    }
    table[6] = -1;  // a sentinel x is still converted: the function does not test it
    *index = 3; Point3 r = {9, 9, 9};
    getCurrentRoutePoint__8Creature(&r, object);
    expect(r.x == -1.0f && r.y == -203.0f, "sentinel entry is converted, not skipped");
    return failures ? 1 : 0;
}
