// Host contract for msg_draw__7Ulthork; not an EE input. 32-bit words are offsets from a host base;
// callees log. The authored function was also compared with the original in a private R5900 run.
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
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* d, void* m, void* s, Point3 p, short a, void* m2, void* an, float f, int g, int h, float i, void* mat) {
    calls += "fx(" + num(off(d) - 0x1000) + "," + num(off(m)) + "," + num(off(s) - 0x1000) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(a) + "," +
             num(off(m2)) + "," + num(off(an) - 0x1000) + "," + num(f) + "," + num(g) + "," + num(h) + "," + num(i) + "," + (mat ? "mat" : "null") + ");";
}
void modelSetShadowScale__Ff(float s) { calls += "shadow(" + num(s) + ");"; }
void* draw__8CreatureP12_modelHeaderP8_textureT1(void*, void* m, void* t, void* t2) {
    calls += "draw(" + num(off(m)) + "," + num(off(t)) + "," + (t2 ? "t2" : "null") + ");";
    return arena + 0x7000;
}
void modelResetOptionalParams__Fv() { calls += "mreset;"; }
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* m, void* t, int a, void* m2, void* rec, unsigned long b, int c, int d) {
    calls += "attach(" + num(off(m)) + "," + num(off(t)) + "," + num(a) + "," + num(off(m2)) + "," + num(off(rec)) + "," + num(static_cast<int>(b)) + "," + num(c) + "," + num(d) + ");";
}
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
int main() {
    int bad = 0;
    for (unsigned weapon : {0u, 3u}) {
        for (short state : {6, 5}) {
            for (int tex : {0, 3, -2}) {
                std::memset(arena, 0, sizeof arena); w10_actor_o_host_base = arena; calls.clear();
                put32(36, 0x300); putA(0x300 + 12, 0x400); putA(0x300 + 20 + tex * 8, 0x500 + tex);
                putA(0x300 + 3 * 8 + 4, 0x600); putA(0x300 + 3 * 8 + 12, 0x610);
                put32(960, weapon);
                obj[1104] = static_cast<unsigned char>(tex);
                float pt[3] = {4, 5, 6}; std::memcpy(obj + 16, pt, 12);
                short h = 7; std::memcpy(obj + 56, &h, 2); std::memcpy(obj + 572, &state, 2);
                msg_draw__7Ulthork(obj);
                std::string want;
                if (weapon) want += std::string("fx(1032,1536,964,4:5:6,7,1024,288,0.5,") + (state == 6 ? "1" : "0") + ",0,1,null);";
                want += "shadow(2);draw(1024," + num(0x500 + tex) + ",null);mreset;";
                if (weapon) want += "attach(1536,1552,0,1024,28672,-1,-1,0);";
                if (calls != want) { std::printf("fail weapon=%u state=%d tex=%d\n  want %s\n  got  %s\n", weapon, state, tex, want.c_str(), calls.c_str()); bad = 1; }
            }
        }
    }
    return bad;
}
