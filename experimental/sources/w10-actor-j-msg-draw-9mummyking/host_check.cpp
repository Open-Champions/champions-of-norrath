// Host contract for msg_draw__9MummyKing; not an EE input. 32-bit words are offsets from a host base;
// callees log. The authored function was also compared with the original in a private R5900 run.
#define W10_ACTOR_J_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_j_host_base;
#include "msg_draw.cpp"

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
void* draw__8CreatureP12_modelHeaderP8_textureT1(void*, void* m, void* t, void* t2) {
    calls += "draw(" + num(off(m)) + "," + num(off(t)) + "," + (t2 ? "t2" : "null") + ");"; return arena + 0x7000;
}
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* m, void* t, int a, void* m2, void* rec, unsigned long b, int c, int d) {
    calls += "attach(" + num(off(m)) + "," + num(off(t)) + "," + num(a) + "," + num(off(m2)) + "," + num(off(rec)) + "," + num(static_cast<int>(b)) + "," + num(c) + "," + num(d) + ");";
}
}
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
int main() {
    int bad = 0;
    for (unsigned k : {0u, 3u, 5u}) {
        std::memset(arena, 0, sizeof arena); w10_actor_j_host_base = arena; calls.clear();
        putA(0x1000 + 36, 0x300); putA(0x300 + 12, 0x400); putA(0x300 + 20, 0x500);
        putA(0x300 + 28, 0x600); putA(0x300 + 36, 0x610); putA(0x300 + 44, 0x620); putA(0x300 + 52, 0x630);
        putA(0x1000 + 960, k);
        msg_draw__9MummyKing(obj);
        std::string want = "draw(1024,1280,null);";
        if (k == 3) want += "attach(1536,1552,0,1024,28672,-1,-1,0);";
        if (k == 5) want += "attach(1568,1584,0,1024,28672,-1,-1,0);";
        if (calls != want) { std::printf("fail k=%u\n  want %s\n  got  %s\n", k, want.c_str(), calls.c_str()); bad = 1; }
    }
    return bad;
}
