// Host contract for msg_draw__10WeaponRack; not an EE input. 32-bit words are offsets from a host
// base; callees log. The authored function was also compared with the original in a private R5900 run.
#define W10_ACTOR_P_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_p_host_base;
#include "msg_draw.cpp"

static unsigned char arena[0x4000] __attribute__((aligned(16)));
unsigned char globalItem[16 * 8];
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static int proxyIndex;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
int getProxyMeshId__4itemPQ24item11description(void* d) { calls += "proxy(" + num(off(d) - 0x1000) + ");"; return proxyIndex; }
void matMakeTransYaw__FP8Matrix34G6Point3s(void*, Point3 p, short yaw) { calls += "yaw(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(yaw) + ");"; }
void setupHalo__4itemPQ24item11descriptionf(void* d, float r) { calls += "halo(" + num(off(d) - 0x1000) + "," + num(r) + ");"; }
void matTrans__FP8Matrix34fff(void*, float x, float y, float z) { calls += "trans(" + num(x) + "," + num(y) + "," + num(z) + ");"; }
void matRot__FP8Matrix34is(void*, int axis, short a) { calls += "rot(" + num(axis) + "," + num(a) + ");"; }
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* m, void* t, int a, Point3 p, void* mat, void* an, unsigned long mask, void* rec, int b) {
    calls += "draw(" + num(off(m)) + "," + num(off(t)) + "," + num(a) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + (mat ? "mat" : "null") + "," +
             (an ? "an" : "null") + "," + num(static_cast<int>(mask)) + "," + (rec ? "rec" : "null") + "," + num(b) + ");";
    return 0;
}
}
static void put32(unsigned char* base, unsigned o, unsigned v) { std::memcpy(base + o, &v, 4); }
int main() {
    int bad = 0;
    for (int idx : {0, 3}) {
        for (unsigned kind : {0u, 1u, 2u, 3u, 255u}) {
            std::memset(arena, 0, sizeof arena); calls.clear(); w10_actor_p_host_base = arena; proxyIndex = idx;
            float pt[3] = {4, 5, 60}; std::memcpy(obj + 16, pt, 12);
            unsigned short yaw = 65000; std::memcpy(obj + 56, &yaw, 2);
            put32(obj, 80, 0x2000); arena[0x2000 + 30] = static_cast<unsigned char>(kind);
            put32(globalItem, idx * 16 + 8, 0x300); put32(globalItem, idx * 16 + 12, 0x340);
            msg_draw__10WeaponRack(obj);
            std::string want = "proxy(80);proxy(80);yaw(4:5:46,2464);halo(80,5);";
            want += kind < 2 ? "trans(0,0,28);rot(0,-16384);" : "rot(0,16384);";
            want += "draw(768,832,3,4:5:60,mat,null,-1,null,0);";
            if (calls != want) { std::printf("fail idx=%d kind=%u\n  want %s\n  got  %s\n", idx, kind, want.c_str(), calls.c_str()); bad = 1; }
        }
    }
    return bad;
}
