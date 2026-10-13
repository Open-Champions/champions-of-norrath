// Host contract for moveArrow__14WoodElfSoldier; not an EE input. Includes the source.
#include "move_arrow.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static std::string calls;
extern "C" {
float icos__Fi(int a) { calls += "cos(" + std::to_string(a) + ");"; return 0.5f; }
float isin__Fi(int a) { calls += "sin(" + std::to_string(a) + ");"; return a == -8192 ? -0.5f : 0.25f; }
}
static unsigned char obj[1100];
static void setup(unsigned flag, short angle, float x, float y, float z) {
    std::memset(obj, 0x33, sizeof obj);
    std::memcpy(obj + 988, &flag, 4); std::memcpy(obj + 984, &angle, 2);
    std::memcpy(obj + 972, &x, 4); std::memcpy(obj + 976, &y, 4); std::memcpy(obj + 980, &z, 4);
    calls.clear();
}
static float f(int off) { float v; std::memcpy(&v, obj + off, 4); return v; }
static unsigned w(int off) { unsigned v; std::memcpy(&v, obj + off, 4); return v; }

int main() {
    int bad = 0;
    const unsigned kbits = 0x3c888888u; float k; std::memcpy(&k, &kbits, 4);
    setup(0, 5, 1, 2, 3);
    moveArrow__14WoodElfSoldier(obj);
    if (!calls.empty() || f(972) != 1 || f(976) != 2 || f(980) != 3) { std::puts("fail: inactive"); bad = 1; }
    setup(1, 1234, 10, 20, 30);
    moveArrow__14WoodElfSoldier(obj);
    if (calls != "cos(1234);sin(1234);sin(-8192);") { std::puts(calls.c_str()); std::puts("fail: call order"); bad = 1; }
    if (f(972) != 10 + 200.0f * k || f(976) != 20 + 100.0f * k || f(980) != 30 + -200.0f * k || w(988) != 1) { std::printf("fail: moved %g %g %g %u\n", f(972), f(976), f(980), w(988)); bad = 1; }
    setup(7, -3, 0, 0, 1.0f);   // falls to z = 1 - 200k < 0.0? 200k = 3.33, so yes
    moveArrow__14WoodElfSoldier(obj);
    if (w(988) != 0) { std::puts("fail: not cleared below zero"); bad = 1; }
    setup(7, -3, 0, 0, 3.4f);   // 3.4 - 3.333 stays >= 0
    moveArrow__14WoodElfSoldier(obj);
    if (w(988) != 7) { std::puts("fail: cleared too early"); bad = 1; }
    return bad;
}
