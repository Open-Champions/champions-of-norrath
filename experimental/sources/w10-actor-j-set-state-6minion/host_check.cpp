// Host contract for setState__6Minioni; not an EE input. 32-bit words are offsets from a host base;
// callees log. The authored function was also compared with the original in a private R5900 run
// (every state -7..23, with and without the list node at +960; all but padding words covered).
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
static unsigned rollValue;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
int setStateProcessing__8CreaturePi(void*, int* s) { calls += "proc" + num(*s) + ";"; if (procNew != -1000) *s = procNew; return procResult; }
void resetRouting__8Creature(void*) { calls += "reset;"; }
void getSmartRandomDestination__8Creaturei(void* r, void*, int range) { calls += "smart" + num(range) + ";"; float p[3] = {1, 2, 3}; std::memcpy(r, p, 12); }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rollValue; }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void*, void* h, float t, int f) { calls += "trans(" + num(off(h) - 0x2000) + "," + num(t) + "," + num(f) + ");"; }
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void*, void* h, float a, float b, int f) { calls += "shot(" + num(off(h) - 0x2000) + "," + num(a) + "," + num(b) + "," + num(f) + ");"; }
void animGlobalSync__FP14AnimationState(void*) { calls += "sync;"; }
}
unsigned w10_actor_j_host_call(unsigned fw, void* adjusted, int n, unsigned long a1, unsigned long) {
    calls += "v" + num(fw) + "(" + num(off(adjusted)) + "," + num(n) + "," + num(a1) + ");"; return 0;
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static unsigned get32(unsigned o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static short get16(unsigned o) { short v; std::memcpy(&v, obj + o, 2); return v; }
static void reset(short current, bool node) {
    std::memset(arena, 0, sizeof arena); w10_actor_j_host_base = arena; calls.clear();
    procResult = 0; procNew = -1000; rollValue = 1001;
    put32(36, 0x300); put32(72, 0x400); putA(0x400 + 192, 0x10); putA(0x400 + 196, 0x99);
    for (unsigned i = 0; i < 100; ++i) putA(0x300 + 4 * i, 0x2000 + 4 * i);       // R[offset] = 0x2000 + offset
    putA(0x2000 + 12, 0x2800);                                                    // node +12 -> child object
    putA(0x2800 + 72, 0x2900); putA(0x2900 + 184, 8); putA(0x2900 + 188, 0x55);
    std::memcpy(obj + 572, &current, 2);
    put32(808, 5000); put32(960, node ? 0x2000 : 0);
}
static int check(const char* label, const std::string& want) {
    if (calls == want) return 0;
    std::printf("fail %s\n  want %s\n  got  %s\n", label, want.c_str(), calls.c_str()); return 1;
}
int main() {
    int bad = 0;
    auto note = [](int v) { return "v85(" + num(0x2808) + ",1," + num(v) + ");"; };
    for (int n : {0, 2, 4, 7, 11, 12, 20, 99, -1}) {
        reset(5, true); setState__6Minioni(obj, n);
        bad |= check("default", "proc" + num(n) + ";rand;trans(108,0.3,3);" + note(20));
        if (get16(572) != 0 || get32(256) != 5000 + 1001 % 120) { std::puts("fail default stores"); bad = 1; }
    }
    reset(0, false); setState__6Minioni(obj, 0); bad |= check("0 no node", "proc0;rand;trans(108,0.3,3);");
    reset(0, true); setState__6Minioni(obj, 1);
    bad |= check("1", "proc1;reset;smart200;rand;trans(164,0.3,2051);sync;" + note(21));
    if (get32(256) != 5000 + 1001 % 120 + 300 || get32(272) == 0) { std::puts("fail 1 stores"); bad = 1; }
    reset(0, true); setState__6Minioni(obj, 3); bad |= check("3", "proc3;trans(164,0.3,2051);sync;" + note(21));
    for (short s : {3, 6, 17, 18, 19}) { reset(s, true); setState__6Minioni(obj, 3); bad |= check("3 skip", "proc3;" + note(21)); }
    reset(0, true); setState__6Minioni(obj, 5); bad |= check("5", "proc5;trans(108,0.1,3);" + note(20));
    reset(0, true); rollValue = 3; setState__6Minioni(obj, 6); bad |= check("6", "proc6;rand;shot(44,0.1,0.1,2056);");
    if (get32(976) != 0 || get32(972) != 6) { std::puts("fail 6 stores"); bad = 1; }
    reset(0, true); setState__6Minioni(obj, 17); bad |= check("17", "proc17;shot(52,0.1,0.1,2056);");
    reset(0, true); setState__6Minioni(obj, 18); bad |= check("18", "proc18;shot(60,0.1,0.1,2056);");
    reset(0, true); setState__6Minioni(obj, 19); bad |= check("19", "proc19;shot(28,0.1,0.1,2056);");
    reset(9, true); setState__6Minioni(obj, 8); bad |= check("8", "proc8;"); if (get16(572) != 9) { std::puts("fail 8"); bad = 1; }
    reset(0, true); setState__6Minioni(obj, 9); bad |= check("9", "proc9;trans(108,0.1,3);");
    reset(0, true); setState__6Minioni(obj, 10); bad |= check("10", "proc10;trans(148,0.3,6);" + note(23) + "v153(" + num(0x1010) + ",0,0);");
    reset(0, true); setState__6Minioni(obj, 13); bad |= check("13", "proc13;shot(132,0.1,0.1,2056);" + note(19));
    if (get32(976) != 0 || get32(972) != 13) { std::puts("fail 13 stores"); bad = 1; }
    reset(0, false); setState__6Minioni(obj, 13); bad |= check("13 no node", "proc13;shot(132,0.1,0.1,2056);");
    reset(0, true); setState__6Minioni(obj, 14); bad |= check("14", "proc14;shot(124,0.1,0.1,2064);");
    reset(0, true); setState__6Minioni(obj, 15); bad |= check("15", "proc15;shot(140,0.1,0.1,2056);" + note(18));
    reset(0, true); setState__6Minioni(obj, 16); bad |= check("16", "proc16;shot(68,0.1,0.1,2056);rand;");
    if (get32(976) != 0 || get32(972) != 16 || get32(980) != 5000 + 1001 % 300 + 600) { std::puts("fail 16 stores"); bad = 1; }
    reset(3, true); procResult = 1; setState__6Minioni(obj, 0); bad |= check("handled", "proc0;"); if (get16(572) != 3) bad = 1;
    reset(0, true); procNew = 9; setState__6Minioni(obj, 0); bad |= check("changed", "proc0;trans(108,0.1,3);");
    return bad;
}
