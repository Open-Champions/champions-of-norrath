// Host contract for getNextRoutePoint__8CreatureP6Point3G6Point3ffi; not an EE input.
#include "route_point.cpp"
#include <cstdio>
#include <cstring>
static int failures, plotCalls; static void* plotSelf; static Point3 plotPoint; static float plotA, plotB; static int plotC;
void plotRoute__8CreatureG6Point3ffi(void* s, const Point3* p, float a, float b, int c) {
    ++plotCalls; plotSelf = s; plotPoint = *p; plotA = a; plotB = b; plotC = c;
}
static void expect(bool ok, const char* what) { if (!ok) { std::printf("fail: %s\n", what); ++failures; } }
int main() {
    alignas(16) static unsigned char object[1000];
    int* table = reinterpret_cast<int*>(object + 580);
    int* index = reinterpret_cast<int*>(object + 708);
    Point3 from = {1, 2, 3};
    auto reset = [&](int i, int ends) {
        std::memset(object, 0, sizeof object);
        for (int k = 0; k < 16; ++k) { table[2 * k] = k < ends ? 10 + k : -1; table[2 * k + 1] = 50 + k; }
        *index = i; plotCalls = 0;
    };
    Point3 out;
    reset(2, 16); out = {7, 7, 7};
    expect(getNextRoutePoint__8CreatureP6Point3G6Point3ffi(object, &out, &from, 1.5f, 2.5f, 9) == 1 && *index == 3 && out.x == 13.0f && out.y == 53.0f && out.z == 7.0f && plotCalls == 0, "advance");
    reset(2, 3); out = {7, 7, 7};
    expect(getNextRoutePoint__8CreatureP6Point3G6Point3ffi(object, &out, &from, 1.5f, 2.5f, 9) == 0 && *index == 3 && out.x == -1.0f && out.y == -1.0f && out.z == 7.0f && plotCalls == 1, "next is a sentinel: index moves, plan");
    expect(plotSelf == object && plotPoint.x == 1 && plotPoint.y == 2 && plotPoint.z == 3 && plotA == 1.5f && plotB == 2.5f && plotC == 9, "plot arguments");
    reset(15, 16); out = {7, 7, 7};
    expect(getNextRoutePoint__8CreatureP6Point3G6Point3ffi(object, &out, &from, 0, 0, 4) == 0 && *index == 16 && plotCalls == 1 && out.x == -1.0f, "last entry: index becomes 16, plan");
    reset(16, 16); out = {7, 7, 7};
    expect(getNextRoutePoint__8CreatureP6Point3G6Point3ffi(object, &out, &from, 0, 0, 4) == 0 && *index == 16 && plotCalls == 0 && out.x == -1.0f && out.y == -1.0f && out.z == 7.0f, "index 16: no plan");
    reset(0, 0); out = {7, 7, 7};
    expect(getNextRoutePoint__8CreatureP6Point3G6Point3ffi(object, &out, &from, 0, 0, 4) == 0 && *index == 0 && plotCalls == 1, "current entry is a sentinel: plan, index kept");
    reset(1, 5);
    expect(getNextRoutePoint__8CreatureP6Point3G6Point3ffi(object, nullptr, &from, 0, 0, 4) == 0 && *index == 1 && plotCalls == 0, "null output: nothing");
    return failures ? 1 : 0;
}
