// Host contract for msg_draw__17UndeadKnightClone; not an EE input. 32-bit words are offsets from a
// host base; callees log. With every E and W zero the expected sprite points follow from the
// generator alone, which the test replays independently. The authored function was also compared
// with the original in a private R5900 run (150 randomized objects, full instruction coverage).
#define W10_ACTOR_O_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <initializer_list>
unsigned char* w10_actor_o_host_base;
#include "msg_draw.cpp"

static unsigned char arena[0x10000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
int asyncRandState;
void* haloTexture5 = arena + 0x4444;
static std::string calls;
struct Sprite { Point3 at; float a, b; int c, d, e, f; void* tex; short g; int h; unsigned stateAfter; };
static std::vector<Sprite> sprites;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static std::string pt(const Point3& p) { return num(p.x) + ":" + num(p.y) + ":" + num(p.z); }
extern "C" {
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3* p, float a, float b, int c, int d, int e, int f, void* t, short g, int h) {
    sprites.push_back({*p, a, b, c, d, e, f, t, g, h, static_cast<unsigned>(asyncRandState)});
}
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* m, Point3 p, short yaw, void* an, int index, Point3* first, Point3* second, float scale) {
    calls += "pos(" + num(off(m)) + "," + pt(p) + "," + num(yaw) + "," + num(off(an) - 0x1000) + "," + num(index) + "," + (second ? "second" : "null") + "," + num(scale) + ");";
    *first = Point3{7, 8, 9};
}
void modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f(void* m, Point3 p, short yaw, void* an, int index, void* matrix, float scale) {
    calls += "matrix(" + num(off(m)) + "," + pt(p) + "," + num(yaw) + "," + num(off(an) - 0x1000) + "," + num(index) + "," + num(scale) + ");";
    std::memset(matrix, 0x11, 48);
}
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(void* m, void* t, int mode, Point3 p, void* matrix, void* an, unsigned long long flags, void* rec, int last) {
    calls += "draw(" + num(off(m)) + "," + num(off(t)) + "," + num(mode) + "," + pt(p) + "," + num(static_cast<unsigned char*>(matrix)[47]) + "," + num(off(an) - 0x1000) + "," + num(static_cast<double>(flags)) + "," + (rec ? "rec" : "null") + "," + num(last) + ");";
    return 0;
}
}
unsigned w10_actor_o_host_call(unsigned fw, void* adjusted, int n, unsigned long, unsigned long) {
    calls += "v" + num(fw) + "(" + num(off(adjusted) - 0x1000) + "," + num(n) + ");"; return 0x2468;
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static void putf(unsigned o, float f) { std::memcpy(obj + o, &f, 4); }
int main() {
    int bad = 0;
    for (unsigned seed : {1u, 0x12345678u, 0xffffffffu}) {
        for (unsigned w2556 : {0u, 1u}) {
            for (unsigned w2696 : {0u, 1u}) {
                std::memset(arena, 0, sizeof arena); w10_actor_o_host_base = arena; calls.clear(); sprites.clear();
                put32(36, 0x300); putA(0x300 + 12, 0x400); putA(0x300 + 180, 0x500); putA(0x300 + 188, 0x510); putA(0x300 + 196, 0x520); putA(0x300 + 204, 0x530);
                put32(72, 0x800); putA(0x800 + 128, 0x10000 | 0); putA(0x800 + 132, 0x66);
                putf(16, 1.0f); putf(20, 2.0f); putf(24, 3.0f); putf(28, 5.0f); putf(32, 2.0f); putf(224, 0.5f);
                short h = -3; std::memcpy(obj + 56, &h, 2);
                put32(2556, w2556); put32(2696, w2696);
                unsigned long long mask = 0x0000000500000007ull; std::memcpy(obj + 552, &mask, 8);
                asyncRandState = static_cast<int>(seed);
                msg_draw__17UndeadKnightClone(obj);
                if (sprites.size() != 64) { std::puts("fail: sprite count"); return 1; }
                unsigned state = seed;
                auto draw = [&]() { state = state * 0x19660du + 0x3c6ef35fu; const float v = static_cast<float>(static_cast<int>(state >> 16)) * (1.0f / 65536.0f); return v + v - 1.0f; };
                for (int i = 0; i < 64; ++i) {
                    float x, y, z;
                    do { x = draw(); y = draw(); z = draw(); } while (1.0f < x * x + y * y + z * z);
                    const Sprite& s = sprites[i];
                    const float wx = (0.0f + x * 1.5f) * 0.8f, wy = (0.0f + y * 1.5f) * 0.8f, wz = (0.0f + z * 1.5f) * 0.8f;
                    const float tol = 1e-5f;
                    auto near = [&](float a, float b) { return a - b < tol && b - a < tol; };
                    if (!near(s.at.x, wx + 1.0f) || !near(s.at.y, wy + 2.0f) || !near(s.at.z, wz + 3.0f + 1.5f)) { std::printf("fail: point %d\n", i); bad = 1; }
                    if (s.a != 30.0f || s.b != 500.0f || s.c != 0 || s.d != 0 || s.e != 0 || s.f != 200 || s.tex != haloTexture5 || s.g != 0 || s.h != 0) { std::puts("fail: sprite constants"); bad = 1; }
                    if (s.stateAfter != state) { std::printf("fail: state after %d\n", i); bad = 1; }
                }
                if (static_cast<unsigned>(asyncRandState) != state) { std::puts("fail: final state"); bad = 1; }
                std::string want;
                if (w2556) want += "v102(0,0);pos(9320,1:2:3,-3,288,0,null,0.5);matrix(1024,1:2:3,-3,288,0,0.5);draw(1280,1296,1,7:8:9,17,288,2.14748e+10,null,0);";
                if (w2696) want += "v102(0,0);pos(9320,1:2:3,-3,288,1,null,0.5);matrix(1024,1:2:3,-3,288,1,0.5);draw(1312,1328,1,7:8:9,17,288,2.14748e+10,null,0);";
                if (calls != want) { std::printf("fail: calls\n  want %s\n  got  %s\n", want.c_str(), calls.c_str()); bad = 1; }
            }
        }
    }
    return bad;
}
