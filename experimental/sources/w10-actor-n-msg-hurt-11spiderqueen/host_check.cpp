// Host contract for msg_hurt__11SpiderQueenfP10GameObjecti; not an EE input. 32-bit words are
// offsets from a host base; callees log. The authored function was also compared with the
// original in a private R5900 run.
#define W10_ACTOR_N_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_n_host_base;
#include "msg_hurt.cpp"

char bossMeterName[128];
int bossMeterCurrent;
static unsigned char arena[0x4000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
extern "C" {
void msg_hurt__8CreaturefP10GameObjecti(void* s, float a, void* src, int k) { calls += "base(" + num(a) + "," + num(k) + ");"; }
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* m, Point3 p, short a, void* an, int i, Point3* o1, Point3* o2, float f) {
    calls += "pos(" + num(static_cast<unsigned char*>(m) - arena) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(a) + "," + num(static_cast<unsigned char*>(an) - obj) + "," + num(i) + "," + num(f) + ");";
    *o1 = Point3{static_cast<float>(i), 1, 2};
    *o2 = Point3{9, 9, 9};
}
void addEffect__FiG6Point3(int k, Point3 p) { calls += "effect(" + num(k) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + ");"; }
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* m, int mask) {
    calls += "chunks(" + num(static_cast<unsigned char*>(m) - arena) + "," + num(static_cast<unsigned>(mask)) + ");";
    return 0x123456789abcdef0ull;
}
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static unsigned get32(unsigned o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static void reset(float hp) {
    std::memset(arena, 0, sizeof arena); w10_actor_n_host_base = arena; calls.clear();
    put32(36, 0x300); std::memset(bossMeterName, 'x', sizeof bossMeterName); bossMeterCurrent = -1;
    unsigned block = 0x400; std::memcpy(arena + 0x300 + 12, &block, 4);
    std::memcpy(obj + 560, &hp, 4);
    float pt[3] = {4, 5, 6}; std::memcpy(obj + 16, pt, 12);
    short h = 7; std::memcpy(obj + 56, &h, 2);
    float f = 1.5f; std::memcpy(obj + 224, &f, 4);
    put32(808, 1000);
}
static int bad;
static void check(const char* label, const std::string& want) {
    if (calls != want) { std::printf("fail %s\n  want %s\n  got  %s\n", label, want.c_str(), calls.c_str()); bad = 1; }
}
int main() {
    const std::string pre = "base(2,3);";
    const std::string pos = "pos(1024,4:5:6,7,288,";
    // Dead at once when the byte at +570 is set: nothing is called.
    reset(10); obj[570] = 1; msg_hurt__11SpiderQueenfP10GameObjecti(obj, 2.0f, nullptr, 3);
    check("skip", ""); if (bossMeterCurrent != -1 || bossMeterName[0] != 'x') { std::puts("fail skip stores"); bad = 1; }
    // Above every threshold (> 140): only the base call, chunks and the meter.
    reset(141); msg_hurt__11SpiderQueenfP10GameObjecti(obj, 2.0f, nullptr, 3);
    check("high", pre + "chunks(1024,0);"); if (std::strcmp(bossMeterName, "Shelox") != 0 || bossMeterName[7] != 'x' || bossMeterCurrent != 141) { std::puts("fail high stores"); bad = 1; }
    // Exactly 140: slot 2 only.
    reset(140); msg_hurt__11SpiderQueenfP10GameObjecti(obj, 2.0f, nullptr, 3);
    check("140", pre + pos + "2,1.5);effect(3,2:1:2);chunks(1024," + num(0x08021041u) + ");");
    if (get32(1028 + 8) != 1020 || get32(244) != 0x08021041u) { std::puts("fail 140 stores"); bad = 1; }
    unsigned long long chunks; std::memcpy(&chunks, obj + 552, 8); if (chunks != 0x123456789abcdef0ull) { std::puts("fail 64-bit store"); bad = 1; }
    // Low health: every slot in turn, ORing all six masks; a filled slot is skipped.
    reset(-5); put32(1028 + 4 * 4, 77);
    msg_hurt__11SpiderQueenfP10GameObjecti(obj, 2.0f, nullptr, 3);
    check("low", pre + pos + "2,1.5);effect(3,2:1:2);" + pos + "3,1.5);effect(3,3:1:2);" + pos + "5,1.5);effect(3,5:1:2);" + pos + "6,1.5);effect(3,6:1:2);" + pos + "7,1.5);effect(3,7:1:2);" +
        "chunks(1024," + num(0x08021041u | 0x00042082u | 0x10208208u | 0x03810410u | 0x01400820u) + ");");
    if (get32(1028 + 16) != 77 || get32(1028 + 12) != 1020) { std::puts("fail low slots"); bad = 1; }
    // The thresholds step down by 20: 120 reaches slots 2 and 3, 119.5 as well, 121 only slot 2.
    reset(121); msg_hurt__11SpiderQueenfP10GameObjecti(obj, 2.0f, nullptr, 3);
    if (get32(1028 + 12) != 0 || get32(1028 + 8) == 0) { std::puts("fail 121"); bad = 1; }
    reset(120); msg_hurt__11SpiderQueenfP10GameObjecti(obj, 2.0f, nullptr, 3);
    if (get32(1028 + 12) == 0 || get32(1028 + 16) != 0) { std::puts("fail 120"); bad = 1; }
    return bad;
}
