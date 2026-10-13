// Host contract for setState__3NPCi; not an EE input. Callees are stubs that log; each case
// compares the call sequence and the stored state/target with what the original does.
#include <cstdio>
#include <cstring>
#include <string>
static std::string calls;
static int procResult, procNewState = -1000;
static unsigned long lumpResult, findResult;
extern "C" {
int setStateProcessing__8CreaturePi(void*, int* s) { calls += "proc" + std::to_string(*s) + ";"; if (procNewState != -1000) *s = procNewState; return procResult; }
void getSmartRandomDestination__8Creaturei(void* r, void*, int range) { calls += "smart" + std::to_string(range) + ";"; float p[3] = {1, 2, 3}; std::memcpy(r, p, 12); }
void* lumpFindResource__FPcT0(char*, char* n) { calls += std::string("lump(") + n + ");"; return reinterpret_cast<void*>(lumpResult); }
void* objectFindAnimation__FP10GameObjectPc(void*, char* n) { calls += std::string("find(") + n + ");"; return reinterpret_cast<void*>(findResult); }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void*, void* h, float t, int f) {
    calls += "trans(" + std::to_string(reinterpret_cast<unsigned long>(h)) + "," + std::to_string(t) + "," + std::to_string(f) + ");";
}
void animGlobalSync__FP14AnimationState(void*) { calls += "sync;"; }
}
#include "set_state.cpp"
int main() {
    alignas(16) static unsigned char o[1280];
    int bad = 0;
    struct C { int state; int proc; int changed; short cur; int w1136; const char* name; unsigned long lump, find; const char* expect; short stored; };
    const C cases[] = {
        {5, 1, -1000, 9, 0, "", 0, 0, "proc5;", 9},
        {7, 0, -1000, 9, 0, "", 0, 0, "proc7;", 7},
        {1, 0, -1000, 0, 0, "", 4, 0, "proc1;smart200;lump(walk.anm);trans(4,0.100000,2051);sync;", 1},
        {1, 0, -1000, 1, 0, "", 4, 0, "proc1;smart200;", 1},
        {2, 0, 1, 0, 0, "", 4, 0, "proc2;smart200;lump(walk.anm);trans(4,0.100000,2051);sync;", 1},
        {0, 0, -1000, 3, 1, "FirLow", 8, 0, "proc0;lump(idle1.anm);trans(8,0.100000,3);", 0},
        {0, 0, -1000, 3, 1, "FirLow", 0, 9, "proc0;lump(idle1.anm);find(idle);trans(9,0.100000,3);", 0},
        {0, 0, -1000, 3, 0, "firlow", 8, 0, "proc0;find(idle);", 0},
        {0, 0, -1000, 3, 1, "other", 8, 6, "proc0;find(idle);trans(6,0.100000,3);", 0},
        {-1, 0, -1000, 3, 1, "", 8, 6, "proc-1;", -1},
    };
    for (const C& c : cases) {
        std::memset(o, 0xcc, sizeof o);
        const short cur = c.cur; std::memcpy(o + 572, &cur, 2);
        std::memcpy(o + 1136, &c.w1136, 4); std::strcpy(reinterpret_cast<char*>(o + 1068), c.name);
        calls.clear(); procResult = c.proc; procNewState = c.changed; lumpResult = c.lump; findResult = c.find;
        setState__3NPCi(o, c.state);
        short st; std::memcpy(&st, o + 572, 2);
        float t[3]; std::memcpy(t, o + 272, 12);
        const bool wrote = calls.find("smart") != std::string::npos;
        if (calls != c.expect || st != c.stored || (wrote && (t[0] != 1 || t[1] != 2 || t[2] != 3))) { std::printf("fail %d: %s\n", c.state, calls.c_str()); bad = 1; }
    }
    return bad;
}
