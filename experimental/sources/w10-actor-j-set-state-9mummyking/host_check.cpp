// Host contract for setState__9MummyKingi; not an EE input. 32-bit words are offsets from a host base;
// callees log. The authored function was also compared with the original in a private R5900 run
// (300 scenarios over states -7..20, all but four padding words of the original covered).
#define W10_ACTOR_J_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_j_host_base;
#include "set_state.cpp"

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static int procResult, procNew = -1000;
static unsigned rollValue = 1000;
static void* allocResult;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
int setStateProcessing__8CreaturePi(void*, int* s) { calls += "proc" + num(*s) + ";"; if (procNew != -1000) *s = procNew; return procResult; }
void resetRouting__8Creature(void*) { calls += "reset;"; }
void getSmartRandomDestination__8Creaturei(void* r, void*, int range) { calls += "smart" + num(range) + ";"; float p[3] = {1, 2, 3}; std::memcpy(r, p, 12); }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rollValue; }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* s, void* h, float t, int f) {
    calls += "trans(" + num(off(s) - 0x1000) + "," + num(off(h)) + "," + num(t) + "," + num(f) + ");";
}
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* s, void* h, float a, float b, int f) {
    calls += "shot(" + num(off(s) - 0x1000) + "," + num(off(h)) + "," + num(a) + "," + num(b) + "," + num(f) + ");";
}
void animGlobalSync__FP14AnimationState(void*) { calls += "sync;"; }
void* blockAlloc__Fi(int n) { calls += "alloc" + num(n) + ";"; return allocResult; }
void __9DustDevilG6Point3(void*, Point3 p) { calls += "devil(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + ");"; }
void dropTreasure__8CreatureG6Point3iffffffPc(void*, Point3 p, int a, float b, float c, float d, float e, float f, float g, char* name) {
    calls += "drop(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(f) + "," + num(g) + "," + name + ");";
}
}
unsigned w10_actor_j_host_call(unsigned fw, void* adjusted, int n, unsigned long, unsigned long) {
    calls += "v" + num(fw) + "(" + num(off(adjusted)) + "," + num(n) + ");"; return 0;
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static unsigned get32(unsigned o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static unsigned long long get64(unsigned o) { unsigned long long v; std::memcpy(&v, obj + o, 8); return v; }
static short get16(unsigned o) { short v; std::memcpy(&v, obj + o, 2); return v; }
static void reset(short current) {
    std::memset(arena, 0, sizeof arena); w10_actor_j_host_base = arena; calls.clear();
    procResult = 0; procNew = -1000; rollValue = 1000; allocResult = nullptr;
    put32(36, 0x300); put32(72, 0x400); putA(0x400 + 192, 8); putA(0x400 + 196, 0x77);
    for (unsigned i = 0; i < 40; ++i) putA(0x300 + 4 * i, 0x2000 + 4 * i);       // R[4*i] = 0x2000 + 4*i = a recognisable header word
    std::memcpy(obj + 572, &current, 2);
    put32(808, 5000);
    float pt[3] = {4, 5, 6}; std::memcpy(obj + 16, pt, 12);
    short h = 9; std::memcpy(obj + 56, &h, 2);
}
static int check(const char* label, const std::string& want) {
    if (calls == want) return 0;
    std::printf("fail %s\n  want %s\n  got  %s\n", label, want.c_str(), calls.c_str()); return 1;
}
int main() {
    int bad = 0;
    // R[offset] = 0x2000 + offset
    reset(0); put32(160, 0xffffffffu); put32(164, 0x12345678u); setState__9MummyKingi(obj, 0);
    bad |= check("0", "proc0;rand;trans(288,8260,0.1,3);");
    if (get32(256) != 5000 + 1000 % 120 || get64(160) != 0x12345678fffffdffull || get16(572) != 0) { std::puts("fail 0 stores"); bad = 1; }
    reset(0); setState__9MummyKingi(obj, 1);
    bad |= check("1", "proc1;reset;smart200;rand;trans(288,8300,0.1,2051);sync;");
    if (get32(256) != 5000 + 1000 % 120 + 300 || get32(272) == 0 || get16(572) != 1) { std::puts("fail 1 stores"); bad = 1; }
    reset(5); setState__9MummyKingi(obj, 3);
    bad |= check("3 other state", "proc3;reset;trans(288,8300,0.1,2051);sync;");
    for (short s : {3, 6, 13, 15, 8}) { reset(s); setState__9MummyKingi(obj, 3); bad |= check("3 skipped", "proc3;reset;"); }
    reset(0); setState__9MummyKingi(obj, 5); bad |= check("5", "proc5;trans(288,8260,0.1,3);");
    reset(0); setState__9MummyKingi(obj, 6); bad |= check("6", "proc6;shot(288,8220,0.1,0.1,2056);");
    reset(17); setState__9MummyKingi(obj, 9); bad |= check("9 from 17", "proc9;"); if (get16(572) != 17) { std::puts("fail 9 keeps state"); bad = 1; }
    reset(2); setState__9MummyKingi(obj, 9); bad |= check("9", "proc9;trans(288,8260,0.1,3);");
    reset(6); put32(294, 0); { short b = 8; std::memcpy(obj + 294, &b, 2); } setState__9MummyKingi(obj, 8);
    bad |= check("8 hold", "proc8;"); if (get16(572) != 6) { std::puts("fail 8 keeps state"); bad = 1; }
    reset(6); setState__9MummyKingi(obj, 8); bad |= check("8 shot", "proc8;shot(288,8252,0.1,0.1,16);");
    reset(9); setState__9MummyKingi(obj, 8); bad |= check("8 from 9", "proc8;"); if (get16(572) != 9) bad = 1;
    reset(0); setState__9MummyKingi(obj, 10);
    bad |= check("10", "proc10;drop(4:5:6,9,36,2,1.6,0.6,1.4,0.25,mummytongue);trans(288,8236,0.1,6);v119(" + num(0x1008) + ",0);");
    reset(0); rollValue = 3; setState__9MummyKingi(obj, 13); bad |= check("13", "proc13;rand;shot(288,8228,0.1,0.1,2056);"); if (obj[1160] != 1) { std::puts("fail 13 byte"); bad = 1; }
    reset(0); setState__9MummyKingi(obj, 14); bad |= check("14", "proc14;trans(288,8284,0.1,3);");
    if (get32(256) != 5120 || (get64(160) & 0x200) == 0) { std::puts("fail 14 stores"); bad = 1; }
    reset(0); setState__9MummyKingi(obj, 15); bad |= check("15", "proc15;shot(288,8276,0.1,0.1,2056);"); if (get32(1132) != 5300) { std::puts("fail 15 store"); bad = 1; }
    reset(0); setState__9MummyKingi(obj, 16); bad |= check("16", "proc16;trans(288,8292,0.1,3);");
    if (get32(256) != 5000 || get32(1104) != 5600 || get32(1128) != 0 || (get64(160) & 0x200) == 0) { std::puts("fail 16 stores"); bad = 1; }
    reset(0); allocResult = arena + 0x6000; setState__9MummyKingi(obj, 17); bad |= check("17", "proc17;trans(288,8284,0.1,3);alloc88;devil(4:5:6);");
    if (get32(1152) != 5600 || get32(1156) != 0 || get32(256) != 5000 || (get64(160) & 0x200) == 0) { std::puts("fail 17 stores"); bad = 1; }
    reset(0); setState__9MummyKingi(obj, 17); bad |= check("17 no block", "proc17;trans(288,8284,0.1,3);alloc88;");
    for (int n : {2, 4, 7, 11, 12, 18, -1}) { reset(0); setState__9MummyKingi(obj, n); bad |= check("idle state", "proc" + num(n) + ";"); if (get16(572) != static_cast<short>(n)) bad = 1; }
    reset(3); procResult = 1; put32(160, 0xffffffffu); setState__9MummyKingi(obj, 0); bad |= check("handled", "proc0;");
    if (get16(572) != 3 || get32(160) != 0xffffffffu) { std::puts("fail handled stores"); bad = 1; }
    reset(0); procNew = 6; setState__9MummyKingi(obj, 0); bad |= check("processing changes the state", "proc0;shot(288,8220,0.1,0.1,2056);"); if (get16(572) != 6) bad = 1;
    return bad;
}
