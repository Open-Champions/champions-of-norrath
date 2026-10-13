// Host contract for msg_draw__7WebTrap; not an EE input. 32-bit words are offsets from a host base,
// table calls are routed through w10_actor_p_host_call, and callees log. The authored function was
// also compared with the original in a private R5900 run.
#define W10_ACTOR_P_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_p_host_base;
#include "msg_draw.cpp"

static unsigned char arena[0x4000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static std::string pt(const Point3& p) { return num(p.x) + ":" + num(p.y) + ":" + num(p.z); }
unsigned w10_actor_p_host_call(unsigned fn, void* adjusted, int n, unsigned long, unsigned long) {
    calls += "slot(" + num(fn) + "," + num(off(adjusted)) + "," + num(n) + ");";
    return 0x700;
}
extern "C" {
void DrawWebSplay__FG6Point3iT0iiifP10GameObjectP12_modelHeaderP14AnimationState(Point3 f, int a, Point3 t, int b, int c, int d, float e, void* o, void* m, void* an) {
    calls += "splay(" + pt(f) + "," + num(a) + "," + pt(t) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + (o ? num(off(o)) : "null") + "," +
             (m ? num(off(m)) : "null") + "," + (an ? num(off(an)) : "null") + ");";
}
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3 p, float f, float g, int r, int gg, int b, int a, void* t, short s, int h) {
    calls += "sprite(" + pt(p) + "," + num(f) + "," + num(g) + "," + num(r) + "," + num(gg) + "," + num(b) + "," + num(a) + "," + num(off(t)) + "," + num(s) + "," + num(h) + ");";
}
}
static void put32(unsigned char* base, unsigned o, unsigned v) { std::memcpy(base + o, &v, 4); }
int main() {
    int bad = 0;
    // layout: held record at 0x2000 whose +12 names the object at 0x2400; that object's table at 0x2800.
    for (unsigned held : {0u, 0x2000u}) {
        for (unsigned flags : {0u, 0x20u, 0xffffffdfu, 0x30u}) {
            for (int sb : {5, -3}) {
                std::memset(arena, 0, sizeof arena); calls.clear(); w10_actor_p_host_base = arena;
                float v[12] = {1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                float P[3] = {1, 2, 3}, Q[3] = {4, 5, 6}, A[3] = {7, 8, 9}, B[3] = {10, 11, 12};
                std::memcpy(obj + 16, P, 12); std::memcpy(obj + 84, Q, 12); std::memcpy(obj + 108, A, 12); std::memcpy(obj + 120, B, 12);
                obj[132] = static_cast<unsigned char>(sb); put32(obj, 136, held); put32(obj, 140, 0x900); put32(obj, 144, 77); put32(obj, 152, 3);
                float f = 0.75f; std::memcpy(obj + 156, &f, 4);
                put32(arena, 0x2000 + 12, 0x2400); put32(arena, 0x2400 + 12, flags); put32(arena, 0x2400 + 72, 0x2800);
                put32(arena, 0x2800 + 128 + 4, 0x4242);
                msg_draw__7WebTrap(obj);
                std::string want;
                const std::string head = "," + num(sb) + ",";
                if (held != 0 && (flags & 0x20)) {
                    want += "slot(16962,9216,0);splay(1:2:3," + num(sb) + ",7:8:9,0,3,10,0.75,9216,1792,9504);";
                    want += "slot(16962,9216,0);splay(4:5:6," + num(sb) + ",10:11:12,0,3,10,0.75,9216,1792,9504);";
                } else {
                    want += "splay(1:2:3," + num(sb) + ",7:8:9,0,3,10,0.75,null,null,null);splay(4:5:6," + num(sb) + ",10:11:12,0,3,10,0.75,null,null,null);";
                }
                const unsigned self = off(obj);
                want += "sprite(1:2:3,0,9000,255,255,255,77,2304," + num(static_cast<short>(self)) + ",1);";
                want += "sprite(4:5:6,0,9000,255,255,255,77,2304," + num(static_cast<short>(self + 16384u)) + ",1);";
                if (calls != want) { std::printf("fail held=%x flags=%x sb=%d\n  want %s\n  got  %s\n", held, flags, sb, want.c_str(), calls.c_str()); bad = 1; }
            }
        }
    }
    return bad;
}
