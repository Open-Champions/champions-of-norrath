// Host contract for msg_run__7WebTrap; not an EE input. 32-bit words are offsets from a host base,
// table calls are routed through w10_actor_p_host_call, and callees log. The authored function was
// also compared with the original in a private R5900 run (400 random scenarios).
#define W10_ACTOR_P_HOST 1
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w10_actor_p_host_base;
#include "msg_run.cpp"

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;     // the trap
static const unsigned NODE = 0x2000, KOBJ = 0x2400, HIT = 0x3000, TABLE = 0x3800;
unsigned char* world = arena + 0x7000;
int frameNm = 1000;
static std::string calls;
struct Step { unsigned hit; float fraction; };
static std::vector<Step> script;
static unsigned scriptIndex;
static unsigned typeOfHit = 0x7003;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static std::string pt(const Point3& p) { return num(p.x) + ":" + num(p.y) + ":" + num(p.z); }
unsigned w10_actor_p_host_call(unsigned fn, void* adjusted, int n, unsigned long a1, unsigned long) {
    if (fn == 0x1111) { calls += "type;"; return typeOfHit; }
    calls += "slot(" + num(fn) + "," + num(off(adjusted) - 0x1000) + "," + num(n) + "," + num(a1) + ");";
    return 0;
}
extern "C" {
float worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(void* w, Point3 s, Point3 d, float range, void* ignore, void** hit, int a, int b) {
    calls += std::string("scan(") + (w == world ? "world" : "bad") + "," + pt(s) + "," + pt(d) + "," + num(range) + "," + (ignore == obj ? "self" : num(off(ignore))) + "," + num(a) + "," + num(b) + ");";
    Step st = scriptIndex < script.size() ? script[scriptIndex] : Step{0, 1.0f};
    ++scriptIndex;
    *hit = st.hit ? static_cast<void*>(arena + st.hit) : nullptr;
    return st.fraction;
}
void* __tf10BlackWidow() { calls += "tfBW;"; return reinterpret_cast<void*>(0x7001); }
void* __tf11SpiderQueen() { calls += "tfSQ;"; return reinterpret_cast<void*>(0x7002); }
int __eq__C9type_infoRC9type_info(const void* l, const void* r) { calls += "eq;"; return l == r; }
void __throw_bad_typeid() { calls += "throw;"; }
void listOnewayLink__FP8ListHeadT0Pv(void* h, void* l, void* d) { calls += "link(" + num(off(h) - 0x1000) + "," + num(off(l)) + "," + (l == d ? "same" : "diff") + ");"; }
int iatan2__Fff(float y, float x) { calls += "atan(" + num(y) + "," + num(x) + ");"; return 1000; }
}
static void put32(unsigned at, unsigned v) { std::memcpy(arena + at, &v, 4); }
static void putf(unsigned at, float v) { std::memcpy(arena + at, &v, 4); }
static void setPt(unsigned at, float x, float y, float z) { putf(at, x); putf(at + 4, y); putf(at + 8, z); }
static float getf(unsigned at) { float v; std::memcpy(&v, arena + at, 4); return v; }
static unsigned get32(unsigned at) { unsigned v; std::memcpy(&v, arena + at, 4); return v; }
static int bad;
static void reset() {
    std::memset(arena, 0, sizeof arena); calls.clear(); w10_actor_p_host_base = arena; frameNm = 1000; scriptIndex = 0; script.clear();
    setPt(0x1000 + 16, 0, 0, 0); setPt(0x1000 + 84, 10, 0, 0);
    put32(0x1000 + 72, TABLE); { short adj = 4; std::memcpy(arena + TABLE + 8, &adj, 2); } put32(TABLE + 12, 0x2222);
    put32(HIT + 12, 0x20); put32(HIT + 72, HIT + 0x100); put32(HIT + 0x100 + 4, 0x1111);
    put32(NODE + 12, KOBJ);
    obj[132] = 30; put32(0x1000 + 80, 0xffffffffu);
}
static void expect(const char* name, bool ok, const std::string& extra = "") {
    if (!ok) { std::printf("fail: %s %s\n  calls %s\n", name, extra.c_str(), calls.c_str()); bad = 1; }
}
int main() {
    // 1. No hit at all: one scan, then the midpoint branch; counters move; no destruction.
    reset(); script = {{0, 1.0f}};
    msg_run__7WebTrap(obj);
    expect("no hit scan", calls == "scan(world,0:0:24,10:0:0,1200,self,0,0);");
    expect("no hit mid", getf(0x1000 + 108) == 5 && getf(0x1000 + 120) == 5 && getf(0x1000 + 112) == 0 && obj[132] == 50 && get32(0x1000 + 144) == 2 && get32(0x1000 + 76) == 0);
    // 2. A hit with an unknown type sticks the trap and links the object.
    reset(); script = {{HIT, 0.5f}}; typeOfHit = 0x7003;
    msg_run__7WebTrap(obj);
    expect("stick", get32(0x1000 + 76) == 1 && get32(0x1000 + 152) == 4 && getf(0x1000 + 96) == 5 && getf(0x1000 + 100) == 0 && getf(0x1000 + 104) == 0);
    expect("stick calls", calls.find("type;tfBW;eq;type;tfSQ;eq;link(136," + num(HIT) + ",same);") != std::string::npos);
    // 3. A BlackWidow hit is passed through: the next scan ignores it and no link is made.
    reset(); script = {{HIT, 0.5f}, {0, 1.0f}}; typeOfHit = 0x7001;
    msg_run__7WebTrap(obj);
    expect("black widow", calls.find("link") == std::string::npos && calls.find("scan(world,5:0:24,5:0:-24,1200," + num(HIT) + ",0,0);") != std::string::npos && get32(0x1000 + 76) == 0);
    // 4. A SpiderQueen hit is passed through after both type checks.
    reset(); script = {{HIT, 0.5f}, {0, 1.0f}}; typeOfHit = 0x7002;
    msg_run__7WebTrap(obj);
    expect("spider queen", calls.find("type;tfBW;eq;type;tfSQ;eq;scan") != std::string::npos && calls.find("link") == std::string::npos);
    // 5. A hit without the 0x60 flags is never type-checked.
    reset(); script = {{HIT, 0.5f}, {0, 1.0f}}; put32(HIT + 12, 0x9f);
    msg_run__7WebTrap(obj);
    expect("plain hit", calls.find("type;") == std::string::npos && calls.find("scan(world,5:0:24,5:0:-24,1200," + num(HIT) + ",0,0);") != std::string::npos);
    // 6. Stuck with an attachment: the attached object's distance/angle are written, A follows it.
    reset(); put32(0x1000 + 76, 1); put32(0x1000 + 136, NODE); obj[132] = 50; put32(0x1000 + 80, 0xffffffffu);
    put32(KOBJ + 12, 0x20); setPt(KOBJ + 16, 3, 4, 7); setPt(0x1000 + 96, 0, 0, 0); putf(KOBJ + 28, 5.0f); putf(0x1000 + 156, 1.0f);
    msg_run__7WebTrap(obj);
    expect("attached", getf(KOBJ + 756) == 15.0f && static_cast<short>(get32(KOBJ + 748) & 0xffff) == static_cast<short>(-16384 - 1000) && getf(0x1000 + 108) == 3 &&
           getf(0x1000 + 112) == 4 && getf(0x1000 + 116) == 7 && getf(0x1000 + 120) == 3 && getf(0x1000 + 156) == 1.5f);
    expect("attached calls", calls == "atan(3,4);");
    // 7. Byte 49 clears the stored distance; the timer branch for a finished fade pulls the points back.
    reset(); put32(0x1000 + 76, 1); put32(0x1000 + 136, NODE); obj[132] = 49; put32(KOBJ + 12, 0x20); put32(KOBJ + 756, 99); put32(0x1000 + 80, 5);
    setPt(0x1000 + 108, 16, 0, 0); setPt(0x1000 + 120, 10, 6, 0); put32(0x1000 + 144, 10);
    msg_run__7WebTrap(obj);
    expect("clear", get32(KOBJ + 756) == 0 && obj[132] == 48 && get32(0x1000 + 144) == 8, "");
    expect("pulled", std::fabs(getf(0x1000 + 108) - 1.0f) < 0.001f && std::fabs(getf(0x1000 + 124) - -9.0f) < 0.001f);
    // 8. When the byte reaches zero the trap destroys itself through table offset 8 with flag 3.
    reset(); obj[132] = 1; put32(0x1000 + 80, 5);
    msg_run__7WebTrap(obj);
    expect("destroy", calls.find("slot(8738,4,1,3);") != std::string::npos && obj[132] == 0);
    reset(); obj[132] = 2; put32(0x1000 + 80, 5);
    msg_run__7WebTrap(obj);
    expect("keep", calls.find("slot") == std::string::npos && obj[132] == 1);
    return bad;
}
