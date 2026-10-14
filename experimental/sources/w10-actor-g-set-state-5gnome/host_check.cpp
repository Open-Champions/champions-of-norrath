// Host contract for setState__5Gnomei; not an EE input. 32-bit words in the object are offsets
// from a host base (W10_ACTOR_G_HOST), so the real field offsets are used. Each case checks the
// callee sequence with its arguments and the final halfword at +572.
#define W10_ACTOR_G_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10g_host_base;
static std::string calls;
static void vcallFn(void* o) { char b[48]; std::snprintf(b, sizeof b, "vcall(%ld);", static_cast<long>(static_cast<unsigned char*>(o) - w10g_host_base - 0x1000)); calls += b; }
void* w10g_host_fn(unsigned) { return reinterpret_cast<void*>(vcallFn); }
#include "set_state.cpp"

static unsigned char arena[0x4000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static int procReturn, procNew = -1;
static unsigned rolls[4]; static int rollIndex;
static std::string num(double v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
static long off(const void* p) { return static_cast<long>(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
int setStateProcessing__8CreaturePi(void*, int* state) { calls += "proc" + num(*state) + ";"; if (procNew >= 0) *state = procNew; return procReturn; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rolls[rollIndex++ & 3]; }
void resetRouting__8Creature(void*) { calls += "reset;"; }
void getSmartRandomDestination__8Creaturei(Point3* r, void*, int range) { calls += "smart" + num(range) + ";"; r->x = 1; r->y = 2; r->z = 3; }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* s, void* h, float b, int f) {
    unsigned bits; std::memcpy(&bits, &b, 4);
    calls += "trans(" + num(off(s) - 0x1000) + "," + num(off(h)) + "," + (bits == 0x3dcccccc ? "blend" : "BADBLEND") + "," + num(f) + ");";
}
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* s, void* h, float a, float b, int f) {
    unsigned x, y; std::memcpy(&x, &a, 4); std::memcpy(&y, &b, 4);
    calls += "shot(" + num(off(s) - 0x1000) + "," + num(off(h)) + "," + (x == 0x3dcccccc && y == 0x3dcccccc ? "blend" : "BADBLEND") + "," + num(f) + ");";
}
void animGlobalSync__FP14AnimationState(void* s) { calls += "sync" + num(off(s) - 0x1000) + ";"; }
}
static int bad;
static void reset() {
    std::memset(arena, 0, sizeof arena);
    w10g_host_base = arena;
    for (unsigned i = 0; i < 0x200; i += 4) { unsigned v = 0x2000 + i; std::memcpy(arena + 0x200 + i, &v, 4); }  // header table: entry at +i holds 0x2000+i
    unsigned t = 0x200; std::memcpy(obj + 36, &t, 4);
    unsigned vt = 0x800; std::memcpy(obj + 72, &vt, 4);
    unsigned fn = 777; std::memcpy(arena + 0x800 + 192 + 4, &fn, 4);
    short adj = 16; std::memcpy(arena + 0x800 + 192, &adj, 2);
    unsigned w808 = 1000; std::memcpy(obj + 808, &w808, 4);
    calls.clear(); procReturn = 0; procNew = -1; rollIndex = 0;
    rolls[0] = 1000007; rolls[1] = 13; rolls[2] = 4; rolls[3] = 9;
}
static short half(int o) { short v; std::memcpy(&v, obj + o, 2); return v; }
static unsigned wordv(int o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static void expectCalls(const char* name, const std::string& want) { if (calls != want) { std::printf("fail %s:\n  got  %s\n  want %s\n", name, calls.c_str(), want.c_str()); bad = 1; } }
#define H(x) num(0x2000 + (x))
int main() {
    reset(); setState__5Gnomei(obj, 0);
    expectCalls("0", "proc0;trans(288," + H(28) + ",blend,3);rand;");
    if (wordv(256) != 1000 + 1000007 % 120 || half(572) != 0) { std::puts("fail 0 fields"); bad = 1; }
    reset(); setState__5Gnomei(obj, 1);
    expectCalls("1", "proc1;reset;smart200;trans(288," + H(156) + ",blend,2051);rand;sync288;");
    float d[3]; std::memcpy(d, obj + 272, 12);
    if (wordv(256) != 1000 + 1000007 % 120 + 120 || d[0] != 1 || d[1] != 2 || d[2] != 3 || half(572) != 1) { std::puts("fail 1 fields"); bad = 1; }
    reset(); setState__5Gnomei(obj, 3);
    expectCalls("3", "proc3;reset;trans(288," + H(156) + ",blend,2051);sync288;");
    reset(); setState__5Gnomei(obj, 5); expectCalls("5", "proc5;trans(288," + H(100) + ",blend,3);");
    reset(); setState__5Gnomei(obj, 9); expectCalls("9", "proc9;trans(288," + H(28) + ",blend,3);");
    reset(); setState__5Gnomei(obj, 13); expectCalls("13", "proc13;trans(288," + H(124) + ",blend,3);");
    reset(); setState__5Gnomei(obj, 6);
    expectCalls("6", "proc6;trans(288," + H(100) + ",blend,3);rand;shot(288," + H(60 + 8 * (1000007 % 3)) + ",blend,8);rand;");
    if (wordv(256) != 13 % 15) { std::puts("fail 6 field"); bad = 1; }
    reset(); setState__5Gnomei(obj, 10);
    expectCalls("10", "proc10;trans(288," + H(28) + ",blend,6);vcall(16);");
    reset(); setState__5Gnomei(obj, 8); expectCalls("8 plain", "proc8;shot(288," + H(92) + ",blend,16);");
    reset(); short s6 = 6; std::memcpy(obj + 572, &s6, 2); setState__5Gnomei(obj, 8);
    expectCalls("8 with 6, no bit", "proc8;shot(288," + H(92) + ",blend,16);");
    if (half(572) != 8) { std::puts("fail 8 store"); bad = 1; }
    reset(); std::memcpy(obj + 572, &s6, 2); unsigned short f8 = 8; std::memcpy(obj + 294, &f8, 2); setState__5Gnomei(obj, 8);
    expectCalls("8 with 6 and bit", "proc8;"); if (half(572) != 6) { std::puts("fail 8 keeps 6"); bad = 1; }
    reset(); short s9 = 9; std::memcpy(obj + 572, &s9, 2); setState__5Gnomei(obj, 8);
    expectCalls("8 with 9", "proc8;"); if (half(572) != 9) { std::puts("fail 8 keeps 9"); bad = 1; }
    for (int s : {2, 4, 7, 11, 12, 14, 99, -1}) {
        reset(); setState__5Gnomei(obj, s); expectCalls("quiet", "proc" + num(s) + ";");
        if (half(572) != static_cast<short>(s)) { std::puts("fail quiet store"); bad = 1; }
    }
    reset(); procReturn = 1; setState__5Gnomei(obj, 5); expectCalls("returns", "proc5;"); if (half(572) != 0) { std::puts("fail: no store after nonzero processing"); bad = 1; }
    reset(); procNew = 9; setState__5Gnomei(obj, 2); expectCalls("rewritten", "proc2;trans(288," + H(28) + ",blend,3);"); if (half(572) != 9) { std::puts("fail rewritten store"); bad = 1; }
    if (!bad) std::puts("gnome setState ok");
    return bad;
}
