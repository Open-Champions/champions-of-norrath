// Host contract for msg_draw__8SuperOrc; not an EE input. 32-bit words are offsets from a host base;
// callees log. The authored function was also compared with the original in a private R5900 run.
#define W10_ACTOR_N_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_n_host_base;
#include "msg_draw.cpp"

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* d, void* m, void* s, Point3 p, short a, void* m2, void* an, float f, int g, int h, float i, void* mat) {
    calls += "fx(" + num(off(d) - 0x1000) + "," + num(off(m)) + "," + num(off(s) - 0x1000) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(a) + "," + num(off(m2)) + "," +
             num(off(an) - 0x1000) + "," + num(f) + "," + num(g) + "," + num(h) + "," + num(i) + "," + (mat ? "mat" : "null") + ");";
}
void* draw__8CreatureP12_modelHeaderP8_textureT1(void*, void* m, void* t, void* t2) {
    calls += "draw(" + num(off(m)) + "," + num(off(t)) + "," + (t2 ? "t2" : "null") + ");";
    return arena + 0x7000;
}
void modelSetRelightAttachment__FG6Point3(Point3 p) { calls += "relight(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + ");"; }
void lightSetAmbientOffset__FPs(short* p) { calls += "ambient(" + num(off(p) - 0x1000) + ");"; }
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* m, void* t, int a, void* m2, void* rec, unsigned long b, int c, int d) {
    calls += "attach(" + num(off(m)) + "," + num(off(t)) + "," + num(a) + "," + num(off(m2)) + "," + num(off(rec)) + "," + num(static_cast<int>(b)) + "," + num(c) + "," + num(d) + ");";
}
void lightResetOptionalParams__Fv() { calls += "lreset;"; }
void modelResetOptionalParams__Fv() { calls += "mreset;"; }
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
int main() {
    int bad = 0;
    for (short state : {6, 5}) {
        std::memset(arena, 0, sizeof arena); w10_actor_n_host_base = arena; calls.clear();
        put32(36, 0x300); putA(0x300 + 12, 0x400); putA(0x300 + 20, 0x500); putA(0x300 + 124, 0x600); putA(0x300 + 132, 0x610);
        float pt[3] = {4, 5, 6}; std::memcpy(obj + 16, pt, 12);
        short h = 7; std::memcpy(obj + 56, &h, 2); std::memcpy(obj + 572, &state, 2);
        float scale = 2.5f; std::memcpy(obj + 224, &scale, 4);
        msg_draw__8SuperOrc(obj);
        std::string want = std::string("fx(1040,1536,972,4:5:6,7,1024,288,0.5,") + (state == 6 ? "1" : "0") +
            ",0,2.5,null);draw(1024,1280,null);relight(4:5:6);ambient(1064);attach(1536,1552,0,1024,28672,-1,-1,0);lreset;mreset;";
        if (calls != want) { std::printf("fail state=%d\n  want %s\n  got  %s\n", state, want.c_str(), calls.c_str()); bad = 1; }
    }
    return bad;
}
