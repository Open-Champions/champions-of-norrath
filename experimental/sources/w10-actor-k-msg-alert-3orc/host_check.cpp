// Host contract for msg_alert__3OrcifG6Point3P10GameObject; not an EE input. Words that hold
// addresses are offsets from a host base; table calls and callees append to a log. The contract
// fails when a branch, argument or store differs from what the source comment states.
#define W10_ACTOR_K_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_k_host_base;
static std::string calls; static unsigned type_token = 0x1000; static int angle_to_return;
unsigned w10_actor_k_host_call(unsigned fn, void* adjusted, int n, unsigned long a1, unsigned long) {
    if (fn == 0x7777) { calls += "type;"; return type_token; }
    calls += "e184(" + std::to_string(static_cast<unsigned char*>(adjusted) - w10_actor_k_host_base) + "," + std::to_string(a1) + ");";
    (void)n; return 0;
}
extern "C" const void* __tf3Orc() { return reinterpret_cast<const void*>(static_cast<__UINTPTR_TYPE__>(0x1000)); }
extern "C" int __eq__C9type_infoRC9type_info(const void* a, const void* b) { return a == b; }
extern "C" int iatan2__Fff(float y, float x) { calls += "atan(" + std::to_string(y) + "," + std::to_string(x) + ");"; return angle_to_return; }
#include "msg_alert.cpp"
alignas(16) static unsigned char arena[0x4000];
static void put32(unsigned at, unsigned v) { std::memcpy(arena + at, &v, 4); }
static void put16(unsigned at, short v) { std::memcpy(arena + at, &v, 2); }
static void putf(unsigned at, float v) { std::memcpy(arena + at, &v, 4); }
static void reset() {
    std::memset(arena, 0, sizeof arena); calls.clear();
    put32(0x100 + 72, 0x2000); put32(0x2000 + 184 + 4, 0x5555); put16(0x2000 + 184, 16);
    put32(0x1000 + 72, 0x2800); put32(0x2800 + 4, 0x7777);
}
int main() {
    w10_actor_k_host_base = arena;
    unsigned char* const self = arena + 0x100; unsigned char* const other = arena + 0x1000;
    int bad = 0;
    Point3 p = {1.5f, -2.5f, 3.5f};
    // kind 0, same type, low state: entry called with 2 and position stored.
    reset(); put16(0x100 + 572, 1);
    msg_alert__3OrcifG6Point3P10GameObject(self, 0, 0.0f, p, other);
    if (calls != "type;e184(272,2);") bad = 1;
    { Point3 q; std::memcpy(&q, arena + 0x100 + 272, 12); if (q.x != 1.5f || q.y != -2.5f || q.z != 3.5f) bad = 1; }
    // kind 0, different type: nothing.
    reset(); type_token = 0x2000; put16(0x100 + 572, 0);
    msg_alert__3OrcifG6Point3P10GameObject(self, 0, 0.0f, p, other);
    if (calls != "type;") bad = 1; type_token = 0x1000;
    // kind 0, state 2 (not below 2): type checked, nothing else.
    reset(); put16(0x100 + 572, 2);
    msg_alert__3OrcifG6Point3P10GameObject(self, 0, 0.0f, p, other);
    if (calls != "type;") bad = 1;
    // kind 0, null other: nothing.
    reset(); msg_alert__3OrcifG6Point3P10GameObject(self, 0, 0.0f, p, nullptr);
    if (!calls.empty()) bad = 1;
    // kind 2: angle window.
    for (int state : {3, 5, 4}) for (int angle : {-9999, -10000, 0, 9999 + 100 - 100, 9998, 10000, 19999 - 9999, 10001}) for (int nz : {0, 1}) {
        reset(); put16(0x100 + 572, static_cast<short>(state)); put32(0x100 + 1112, nz); put16(0x100 + 56, 100);
        putf(0x100 + 20, 10.0f); putf(0x100 + 16, 4.0f); putf(0x1000 + 20, 16.0f); putf(0x1000 + 16, 1.0f);
        angle_to_return = angle;
        msg_alert__3OrcifG6Point3P10GameObject(self, 2, 0.0f, p, other);
        const bool want_atan = nz && (state == 3 || state == 5);
        const bool want_call = want_atan && static_cast<unsigned>(angle - 100 + 9999) < 19999u;
        std::string want;
        if (want_atan) want = "atan(6.000000,-3.000000);";
        if (want_call) want += "e184(272,16);";
        if (calls != want) { std::printf("kind2 s%d a%d nz%d got %s\n", state, angle, nz, calls.c_str()); bad = 1; }
    }
    // kind 2 with null other, other kinds.
    reset(); put16(0x100 + 572, 3); put32(0x100 + 1112, 1);
    msg_alert__3OrcifG6Point3P10GameObject(self, 2, 0.0f, p, nullptr); if (!calls.empty()) bad = 1;
    for (int kind : {1, 3, -1}) { reset(); msg_alert__3OrcifG6Point3P10GameObject(self, kind, 0.0f, p, other); if (!calls.empty()) bad = 1; }
    return bad;
}
