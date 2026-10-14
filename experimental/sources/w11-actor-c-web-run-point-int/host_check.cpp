// Host contract for run__3WebG6Point3i; not an EE input. Callees are stubs that log. Pointer words are
// offsets from a host base. Each scenario states the expected call log and the expected final fields.
#define W11_ACTOR_C_HOST 1
#include "run.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w11_actor_c_host_base;
unsigned char* world;
W11ActorCHostSlot w11_actor_c_host_slots[1];
static unsigned char base[8192];
static std::string calls;
static std::vector<float> heights;
static unsigned heightIndex;
static bool allocFails;
static int angleResult;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%.9g", v); return b; }
static std::string pt(const Point3& p) { return num(p.x) + ":" + num(p.y) + ":" + num(p.z); }
extern "C" void* blockAlloc__Fi(int size) { calls += "alloc(" + std::to_string(size) + ");"; return allocFails ? nullptr : base + 7000; }
extern "C" void __7WebTrapG6Point3T1(void* memory, Point3 a, Point3 b) {
    calls += std::string("trap(") + (memory == base + 7000 ? "M" : "?") + "," + pt(a) + "," + pt(b) + ");";
}
extern "C" float worldFindHeight__FP12_worldHeaderG6Point3f(void* w, Point3 position, float radius) {
    calls += std::string("height(") + (w == world ? "W" : "?") + "," + pt(position) + "," + num(radius) + ");";
    return heights[heightIndex++ % heights.size()];
}
extern "C" int iatan2__Fff(float y, float x) { calls += "atan(" + num(y) + "," + num(x) + ");"; return angleResult; }
extern "C" void breakWeb__3Web(void*) { calls += "break;"; }
extern "C" void listDestroy__FP8ListHead(void* head) { calls += "destroy(" + std::to_string(static_cast<unsigned char*>(head) - (base + 64)) + ");"; }

static unsigned char* web = base + 64;
static unsigned char* node = base + 1024;
static unsigned char* creature = base + 2048;
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(web + off); }
static float& WF(unsigned off) { return *reinterpret_cast<float*>(web + off); }
static unsigned& C(unsigned off) { return *reinterpret_cast<unsigned*>(creature + off); }
static float& CF(unsigned off) { return *reinterpret_cast<float*>(creature + off); }
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; } }
static void expect_calls(const std::string& want, const char* what) {
    if (calls != want) { std::fprintf(stderr, "fail: %s\n got  %s\n want %s\n", what, calls.c_str(), want.c_str()); ++failures; }
}
static void setup(unsigned until, bool hasNode, unsigned creatureFlags) {
    std::memset(base, 0, sizeof base);
    world = base + 300;
    W(0) = 3; WF(4) = 1.0f; W(8) = 10; W(12) = 0;
    WF(16) = 100.0f; WF(20) = 200.0f; WF(24) = 50.0f;
    WF(32) = 60.0f; WF(36) = -120.0f; WF(40) = 30.0f;
    W(44) = until; W(52) = 25; W(56) = 5;
    W(28) = hasNode ? 1024 : 0;
    *reinterpret_cast<unsigned*>(node + 12) = 2048;
    C(12) = creatureFlags;
    CF(16) = 130.0f; CF(20) = 190.0f; CF(24) = 40.0f; CF(28) = 80.0f; CF(32) = 20.0f; CF(756) = 0.0f;
    C(1400) = 1;
    heights = {-1000.0f}; heightIndex = 0; allocFails = false; angleResult = 40000; calls.clear();
}
static const Point3 P = {7.0f, 9.0f, 11.0f};
static const float K = 0.0166666657f;

int main() {
    w11_actor_c_host_base = base;
    // expired, equal time
    setup(100, true, 0x20); run__3WebG6Point3i(web, P, 100); expect_calls("break;", "equal time breaks");
    expect(W(44) == 100 && W(8) == 10 && W(28) == 1024, "equal time changes nothing");
    // expired
    setup(50, true, 0x20); W(12) = 1; run__3WebG6Point3i(web, P, 100);
    expect_calls("destroy(28);", "expired destroys the node");
    expect(W(44) == 0 && W(12) == 0 && W(8) == 7 && W(28) == 0, "expired: clear 44, 12, length-3, node");
    setup(1, false, 0x20); W(8) = 2; run__3WebG6Point3i(web, P, 100);
    expect_calls("", "expired, no node");
    expect(W(44) == 1 && W(8) == 0, "44 stays 1, length 2-3 clamps to 0");
    setup(0, false, 0x20); W(8) = static_cast<unsigned>(-5); run__3WebG6Point3i(web, P, 100);
    expect(W(44) == 0 && static_cast<int>(W(8)) == 0, "negative length clamps to 0 untouched by -3");
    setup(0, false, 0x20); W(8) = 0; run__3WebG6Point3i(web, P, 100);
    expect(W(8) == 0, "zero length stays");
    // flying: ground hit on the first step
    {
        setup(1000, true, 0x20); heights = {100.0f};
        run__3WebG6Point3i(web, P, 10);
        float x = 100.0f + 60.0f * K, y = 200.0f + -120.0f * K, z = 50.0f + 30.0f * K;
        expect_calls("height(W," + pt({x, y, z}) + ",5);alloc(160);trap(M,7:9:11," + pt({x, y, z}) + ");break;", "ground hit");
        expect(WF(16) == x && W(12) == 0, "position advanced once");
    }
    // flying: ground hit, allocation fails
    {
        setup(1000, true, 0x20); heights = {100.0f}; allocFails = true;
        run__3WebG6Point3i(web, P, 10);
        float x = 100.0f + 60.0f * K, y = 200.0f + -120.0f * K, z = 50.0f + 30.0f * K;
        expect_calls("height(W," + pt({x, y, z}) + ",5);alloc(160);break;", "ground hit, no memory");
    }
    // flying: no node (three steps)
    {
        setup(1000, false, 0);
        run__3WebG6Point3i(web, P, 10);
        float x = 100.0f, y = 200.0f, z = 50.0f;
        std::string want;
        for (int i = 0; i < 3; ++i) {
            x = x + 60.0f * K; y = y + -120.0f * K; z = z + 30.0f * K;
            want += "height(W," + pt({x, y, z}) + ",5);break;";
        }
        expect_calls(want, "no node: three steps");
        expect(W(12) == 0, "still flying");
    }
    // flying: creature without bit 0x20
    {
        setup(1000, true, 0x1f);
        run__3WebG6Point3i(web, P, 10);
        float x = 100.0f + 60.0f * K, y = 200.0f + -120.0f * K, z = 50.0f + 30.0f * K;
        expect_calls("height(W," + pt({x, y, z}) + ",5);alloc(160);trap(M,7:9:11," + pt({x, y, z}) + ");break;", "flag clear: trap");
    }
    // flying: creature already webbed (756 > 0), and flags 0x28 set
    for (int variant = 0; variant < 2; ++variant) {
        setup(1000, true, variant == 0 ? 0x20u : 0x28u);
        if (variant == 0) CF(756) = 0.5f;
        run__3WebG6Point3i(web, P, 10);
        float x = 100.0f, y = 200.0f, z = 50.0f;
        std::string want;
        for (int i = 0; i < 3; ++i) {
            x = x + 60.0f * K; y = y + -120.0f * K; z = z + 30.0f * K;
            want += "height(W," + pt({x, y, z}) + ",5);break;";
        }
        expect_calls(want, "creature busy: three breaks");
    }
    // flying: out of reach, three steps, nothing else
    {
        setup(1000, true, 0x20); CF(28) = 1.0f;
        run__3WebG6Point3i(web, P, 10);
        float x = 100.0f, y = 200.0f, z = 50.0f;
        std::string want;
        for (int i = 0; i < 3; ++i) {
            x = x + 60.0f * K; y = y + -120.0f * K; z = z + 30.0f * K;
            want += "height(W," + pt({x, y, z}) + ",5);";
        }
        expect_calls(want, "out of reach");
        expect(W(12) == 0 && W(0) == 3, "not caught");
    }
    // flying: caught on the first step; the steering part then runs
    {
        setup(1000, true, 0x20); CF(28) = 5000.0f; W(56) = 99999;   // w56 not below now: no countdown
        run__3WebG6Point3i(web, P, 10);
        float x = 100.0f, y = 200.0f, z = 50.0f;
        std::string want;
        for (int i = 0; i < 3; ++i) {
            x = x + 60.0f * K; y = y + -120.0f * K; z = z + 30.0f * K;
            want += "height(W," + pt({x, y, z}) + ",5);";
        }
        want += "atan(" + num(190.0f - 9.0f) + "," + num(130.0f - 7.0f) + ");";
        expect_calls(want, "caught: three steps, then steering");
        expect(W(12) == 1 && W(44) == 35 && W(0) == 5, "caught fields");
        expect(WF(4) == 1.0f + 0.5f, "w4 grows below the radius");
        expect(CF(756) == 160.0f, "creature marked");
        expect(*reinterpret_cast<short*>(creature + 748) == static_cast<short>(40000 - 32767), "creature turned");
        expect(WF(16) == 130.0f && WF(20) == 190.0f && WF(24) == 40.0f + 20.0f * 0.5f, "web follows the creature");
    }
    // attached, countdown
    for (unsigned count : {3u, 1u})
        for (unsigned c1400 : {1u, 2u})
            for (unsigned flags : {0x60u, 0x20u}) {
                setup(1000, true, flags); W(12) = 1; W(0) = count; W(56) = 5; C(1400) = c1400; WF(4) = 9999.0f;
                run__3WebG6Point3i(web, P, 10);
                std::string want = "atan(" + num(190.0f - 9.0f) + "," + num(130.0f - 7.0f) + ");";
                const bool tick = (flags & 0x40u) != 0 && c1400 == 1;
                if (tick && count == 1) want += "break;";
                expect_calls(want, "attached countdown");
                expect(W(0) == (tick ? count - 1 : count), "countdown value");
                expect(W(56) == (tick ? 40u : 5u), "next tick time");
                expect(WF(4) == 9999.0f, "w4 does not grow past the radius");
            }
    // attached, time already past
    setup(1000, true, 0x60); W(12) = 1; W(56) = 10; run__3WebG6Point3i(web, P, 10);
    expect(W(56) == 10 && W(0) == 3, "no countdown when w56 is not below now");
    // attached, no node, and attached with creature flag clear
    setup(1000, false, 0x20); W(12) = 1; run__3WebG6Point3i(web, P, 10); expect_calls("break;", "attached, no node");
    setup(1000, true, 0x1f); W(12) = 1; run__3WebG6Point3i(web, P, 10); expect_calls("break;", "attached, flag clear");
    if (!failures) std::puts("web run ok");
    return failures != 0;
}
