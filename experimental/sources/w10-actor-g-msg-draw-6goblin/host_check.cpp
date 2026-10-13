// Host contract for msg_draw__6Goblin; not an EE input. Words are offsets from a host base.
#define W10_ACTOR_G_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10g_host_base;
void* w10g_host_fn(unsigned) { return nullptr; }
#include "msg_draw.cpp"
unsigned goblinWeaponResourceMap[32];
static unsigned char arena[0x4000] __attribute__((aligned(16)));
static std::string calls; static void* recordValue;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static long off(const void* p) { return static_cast<long>(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* d, void* wm, void* ws, Point3 p, short angle, void* m, void* anim, float a, int b, int c, float dd, void* mat) {
    calls += "fx(" + num(off(d) - 0x1000) + "," + num(off(wm)) + "," + num(off(ws) - 0x1000) + "," + num(p.x) + "," + num(p.y) + "," + num(p.z) + "," +
             num(angle) + "," + num(off(m)) + "," + num(off(anim) - 0x1000) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(dd) + "," + (mat ? "mat" : "null") + ");";
}
void* draw__8CreatureP12_modelHeaderP8_textureT1(void*, void* m, void* t, void* x) { calls += "draw(" + num(off(m)) + "," + num(off(t)) + "," + (x ? "x" : "null") + ");"; return recordValue; }
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* m, void* t, int a, void* m2, void* r, unsigned long b, int c, int d) {
    calls += "att(" + num(off(m)) + "," + num(off(t)) + "," + num(a) + "," + num(off(m2)) + "," + (r == recordValue ? "rec" : "BADREC") + "," + num(static_cast<unsigned>(b)) + "," + num(c) + "," + num(d) + ");";
}
}
static int bad;
static void check(bool ok, const char* what, const std::string& want) { if (!ok) { std::printf("fail: %s\n  got  %s\n  want %s\n", what, calls.c_str(), want.c_str()); bad = 1; } }
int main() {
    w10g_host_base = arena;
    unsigned char* o = arena + 0x1000;
    recordValue = arena + 0x3000;
    auto reset = [&]() {
        std::memset(arena, 0, sizeof arena); calls.clear();
        for (int i = 0; i < 32; ++i) goblinWeaponResourceMap[i] = 0x100 + i;   // slot words: word n holds 0x100 + n
        unsigned t = 0x200; std::memcpy(o + 36, &t, 4);
        unsigned model = 0x400, tex = 0x500; std::memcpy(arena + t + 12, &model, 4); std::memcpy(arena + t + 20, &tex, 4);
        float p[3] = {1, 2, 3}; std::memcpy(o + 16, p, 12);
        short angle = -7; std::memcpy(o + 56, &angle, 2);
        float f224 = 4.5f; std::memcpy(o + 224, &f224, 4);
    };
    auto setw = [&](int off, unsigned v) { std::memcpy(o + off, &v, 4); };
    auto seth = [&](int off, short v) { std::memcpy(o + off, &v, 2); };
    reset();
    { std::string want = "draw(1024,1280,null);"; msg_draw__6Goblin(o); check(calls == want, "no weapons: draw only", want); }
    reset(); setw(988, 2); seth(572, 6); msg_draw__6Goblin(o);
    { std::string want = "fx(1064,261,1000,1,2,3,-7,1024,288,0.5,1,0,4.5,null);draw(1024,1280,null);att(261,275,0,1024,rec,4.29497e+09,-1,0);";
      check(calls == want, "weapon 2 in state 6", want); }
    reset(); setw(988, 3); seth(572, 5); msg_draw__6Goblin(o);
    { std::string want = "fx(1064,263,1000,1,2,3,-7,1024,288,0.5,0,0,4.5,null);draw(1024,1280,null);att(263,277,0,1024,rec,4.29497e+09,-1,0);";
      check(calls == want, "weapon 3 outside state 6", want); }
    reset(); setw(992, 5); msg_draw__6Goblin(o);
    { std::string want = "draw(1024,1280,null);att(267,281,0,1024,rec,4.29497e+09,-1,0);"; check(calls == want, "second weapon only draws", want); }
    reset(); setw(972, 4); msg_draw__6Goblin(o);
    { std::string want = "draw(1024,1280,null);"; check(calls == want, "state 4 without +996", want); }
    reset(); setw(972, 4); setw(996, 1); msg_draw__6Goblin(o);
    { std::string want = "draw(1024,1280,null);att(259,273,0,1024,rec,4.29497e+09,-1,0);"; check(calls == want, "state 4 with +996: slot 1 pair", want); }
    reset(); setw(972, 3); setw(996, 1); msg_draw__6Goblin(o);
    { std::string want = "draw(1024,1280,null);"; check(calls == want, "+996 alone does nothing", want); }
    reset(); setw(988, 1); setw(992, 7); setw(972, 4); setw(996, 9); msg_draw__6Goblin(o);
    { std::string want = "fx(1064,259,1000,1,2,3,-7,1024,288,0.5,0,0,4.5,null);draw(1024,1280,null);att(259,273,0,1024,rec,4.29497e+09,-1,0);att(271,285,0,1024,rec,4.29497e+09,-1,0);att(259,273,0,1024,rec,4.29497e+09,-1,0);";
      check(calls == want, "all three attachments in order", want); }
    if (!bad) std::puts("goblin msg_draw ok");
    return bad;
}
