// Host contract for msg_draw__11VampireLord; not an EE input. 32-bit words are offsets from a host
// base; callees log. The authored function was also compared with the original in a private R5900 run.
#define W10_ACTOR_P_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_p_host_base;
#include "msg_draw.cpp"

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static int tokens[4];
extern "C" {
void modelSetShadowBrightness__Ff(float b) { calls += "shadow(" + num(b) + ");"; }
void* lumpFindResource__FPcT0(const char* l, const char* r) { calls += std::string("lump(") + l + "," + r + ");"; return &tokens[r[4] == 'v' ? 0 : 1]; }
void* draw__8CreatureP12_modelHeaderP8_textureT1(void*, void* m, void* t, void* t2) {
    calls += std::string("draw(") + (m == &tokens[0] ? "bat.vif" : m == &tokens[1] ? "bat.tex" : num(off(m))) + "," +
             (t == &tokens[0] ? "bat.vif" : t == &tokens[1] ? "bat.tex" : num(off(t))) + "," + (t2 ? "t2" : "null") + ");";
    return arena + 0x7000;
}
void modelResetOptionalParams__Fv() { calls += "mreset;"; }
void modelSetRelightAttachment__FG6Point3(Point3 p) { calls += "relight(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + ");"; }
void lightSetAmbientOffset__FPs(short* p) { calls += "ambient(" + num(off(p) - 0x1000) + ");"; }
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* m, void* t, int a, void* m2, void* rec, unsigned long b, int c, int d) {
    calls += "attach(" + num(off(m)) + "," + num(off(t)) + "," + num(a) + "," + num(off(m2)) + "," + num(off(rec)) + "," + num(static_cast<int>(b)) + "," + num(c) + "," + num(d) + ");";
}
void lightResetOptionalParams__Fv() { calls += "lreset;"; }
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
int main() {
    int bad = 0;
    for (unsigned bat : {0u, 1u, 0x80000000u}) {
        for (unsigned attach : {0u, 1u, 9u}) {
            std::memset(arena, 0, sizeof arena); w10_actor_p_host_base = arena; calls.clear();
            put32(36, 0x300); putA(0x300 + 12, 0x400); putA(0x300 + 20, 0x500); putA(0x300 + 172, 0x600); putA(0x300 + 180, 0x610);
            float pt[3] = {4, 5, 6}; std::memcpy(obj + 16, pt, 12);
            put32(964, bat); put32(960, attach);
            msg_draw__11VampireLord(obj);
            std::string want = "shadow(0);";
            if (bat) want += "lump(bat.lmp,bat.vif);lump(bat.lmp,bat.tex);draw(bat.vif,bat.tex,null);mreset;";
            else {
                want += "draw(1024,1280,null);mreset;";
                if (attach) want += "relight(4:5:6);ambient(1120);attach(1536,1552,0,1024,28672,-1,-1,0);lreset;mreset;";
            }
            if (calls != want) { std::printf("fail bat=%u attach=%u\n  want %s\n  got  %s\n", bat, attach, want.c_str(), calls.c_str()); bad = 1; }
        }
    }
    return bad;
}
