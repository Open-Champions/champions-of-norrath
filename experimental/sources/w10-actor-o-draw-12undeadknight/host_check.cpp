// Host contract for msg_draw__12UndeadKnight; not an EE input. 32-bit words are offsets from a host
// base; callees log. The 12-byte copy from the function's own stack residue is indeterminate on the
// host, so the position is only checked when that copy does not run. The authored function was also
// compared with the original in a private R5900 run with a stack pattern that makes both frames read
// the same residue.
#define W10_ACTOR_O_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
#include <initializer_list>
unsigned char* w10_actor_o_host_base;
#include "msg_draw.cpp"

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static Point3 pos_at_draw;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, Point3 point, short yaw, void* state, int index, Point3* first, Point3* second, float scale) {
    calls += "pos(" + num(off(model)) + "," + num(point.x) + ":" + num(point.y) + ":" + num(point.z) + "," + num(yaw) + "," + num(off(state) - 0x1000) + "," + num(index) + "," + num(scale) + ");";
    *first = Point3{1.0f, 2.0f, 3.0f};
    *second = Point3{9.0f, 9.0f, 9.0f};
}
void* draw__8CreatureP12_modelHeaderP8_textureT1(void*, void* m, void* t, void* t2) {
    calls += "draw(" + num(off(m)) + "," + num(off(t)) + "," + (t2 ? "t2" : "null") + ");";
    std::memcpy(&pos_at_draw, obj + 16, 12);
    return arena + 0x7000;
}
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* m, void* t, int a, void* m2, void* rec, unsigned long b, int c, int d) {
    calls += "attach(" + num(off(m)) + "," + num(off(t)) + "," + num(a) + "," + num(off(m2)) + "," + num(off(rec)) + "," + num(static_cast<int>(b)) + "," + num(c) + "," + num(d) + ");";
}
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
int main() {
    int bad = 0;
    for (unsigned w960 : {0u, 4u}) {
        for (unsigned w968 : {0u, 1u}) {
            for (unsigned w1112 : {0u, 1u}) {
                std::memset(arena, 0, sizeof arena); w10_actor_o_host_base = arena; calls.clear();
                put32(36, 0x300); putA(0x300 + 12, 0x400); putA(0x300 + 20, 0x500);
                putA(0x300 + 180, 0x610); putA(0x300 + 188, 0x620); putA(0x300 + 196, 0x630); putA(0x300 + 204, 0x640);
                put32(960, w960); put32(968, w968); put32(1112, w1112);
                float pt[3] = {40, 50, 60}; std::memcpy(obj + 16, pt, 12);
                short h = 7; std::memcpy(obj + 56, &h, 2); float sc = 2.5f; std::memcpy(obj + 224, &sc, 4);
                msg_draw__12UndeadKnight(obj);
                std::string want;
                if (w960) want += "pos(1024,0:0:0,0,288,9,2.5);";
                want += "draw(1024,1280,null);";
                if (w968) want += "attach(1552,1568,0,1024,28672,-1,-1,0);";
                if (w1112) want += "attach(1584,1600,0,1024,28672,-1,-1,0);";
                if (calls != want) { std::printf("fail %u %u %u\n  want %s\n  got  %s\n", w960, w968, w1112, want.c_str(), calls.c_str()); bad = 1; }
                short after; std::memcpy(&after, obj + 56, 2);
                if (after != (w960 ? 0 : 7)) { std::puts("fail: halfword 56"); bad = 1; }
                if (!w960 && (pos_at_draw.x != 40 || pos_at_draw.y != 50 || pos_at_draw.z != 60)) { std::puts("fail: position changed"); bad = 1; }
            }
        }
    }
    return bad;
}
