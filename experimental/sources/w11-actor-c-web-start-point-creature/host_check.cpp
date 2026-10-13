// Host contract for start__3WebG6Point3P8Creaturefi; not an EE input. Callees are stubs that log; the
// reciprocal square root is the host's 1/sqrt fallback, applied identically in the expectation.
#define W11_ACTOR_C_HOST 1
#include "start.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w11_actor_c_host_base;
static std::string calls;
static unsigned char* the_web;
static unsigned char* the_creature;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%.9g", v); return b; }
extern "C" void listDestroy__FP8ListHead(void* head) {
    calls += "destroy(" + std::to_string(static_cast<unsigned char*>(head) - the_web) + "," + std::to_string(*reinterpret_cast<unsigned*>(the_web + 28)) + ");";
}
extern "C" void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data) {
    calls += "link(" + std::to_string(static_cast<unsigned char*>(head) - the_web) + "," + (link == the_creature ? "c" : "?") + "," + (data == the_creature ? "c" : "?") +
             "," + std::to_string(*reinterpret_cast<unsigned*>(the_web + 28)) + ");";
}
extern "C" void start__3WebG6Point3T1i(void* web, Point3 from, Point3 velocity, int n) {
    calls += std::string("start(") + (web == the_web ? "w" : "?") + "," + num(from.x) + "," + num(from.y) + "," + num(from.z) + "," + num(velocity.x) + "," +
             num(velocity.y) + "," + num(velocity.z) + "," + std::to_string(n) + ");";
}
int main() {
    int failures = 0;
    const float spots[][3] = {{0, 0, 0}, {10, -4, 2.5f}, {-100.5f, 33, 7}, {1, 2, 3}};
    for (unsigned head : {0u, 777u})
        for (const auto& from : spots)
            for (float speed : {1.0f, 300.0f, -2.5f})
                for (float height : {0.0f, 160.0f, -40.0f}) {
                    static unsigned char arena[1024];
                    std::memset(arena, 0, sizeof arena);
                    the_web = arena + 64;
                    the_creature = arena + 512;
                    float cp[3] = {64.0f, -32.0f, 10.0f};
                    std::memcpy(the_creature + 16, cp, 12);
                    std::memcpy(the_creature + 32, &height, 4);
                    std::memcpy(the_web + 28, &head, 4);
                    Point3 p = {from[0], from[1], from[2]};
                    calls.clear();
                    start__3WebG6Point3P8Creaturefi(the_web, p, the_creature, speed, 5);
                    float qz = cp[2] + height * 0.25f;
                    float dx = cp[0] - from[0], dy = cp[1] - from[1], dz = qz - from[2];
                    float s = 1.0f / __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
                    dx = dx * s; dy = dy * s; dz = dz * s;
                    dx = dx * speed; dy = dy * speed; dz = dz * speed;
                    std::string want;
                    if (head != 0u) want += "destroy(28,777);";
                    want += "link(28,c,c,0);";
                    want += "start(w," + num(from[0]) + "," + num(from[1]) + "," + num(from[2]) + "," + num(dx) + "," + num(dy) + "," + num(dz) + ",5);";
                    if (calls != want) { std::fprintf(stderr, "got  %s\nwant %s\n", calls.c_str(), want.c_str()); ++failures; }
                    unsigned w28; std::memcpy(&w28, the_web + 28, 4);
                    if (w28 != 0u) ++failures;
                }
    if (!failures) std::puts("web start ok");
    return failures != 0;
}
