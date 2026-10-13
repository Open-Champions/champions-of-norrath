#define NORRATH_HOST_ARENA 1
#include "process.cpp"
#include <string.h>
#include <string>

static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return a == 0 ? nullptr : arena + a; }
extern "C" unsigned norrath_host_word(const void* p) { return p ? (unsigned)((const unsigned char*)p - arena) : 0; }
static std::string tr; static void* alloc_ret; static int proj_clears;
static void camera(int a, int b, int c, int d) { tr += "cam" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + ";"; }
extern "C" void* norrath_host_function(unsigned) { return (void*)camera; }
static std::string o(const void* p) { return p ? std::to_string((const unsigned char*)p - arena) : "0"; }
static unsigned char* drv = arena + 0x100;
#define STUB1(name, tag) extern "C" void name(void* d) { tr += tag + o(d) + ";"; }
STUB1(ProcessLightningEvent__18VISpellEventDriver, "L")
STUB1(ProcessWaitEvent__18VISpellEventDriver, "W")
STUB1(ProcessInactiveEvents__18VISpellEventDriver, "I")
extern "C" void ProcessProjectileEvent__18VISpellEventDriver(void* d) { tr += "P" + o(d) + ";"; if (proj_clears) *(int*)((unsigned char*)d + 36) = 0; }
#define STUB3(name, tag) extern "C" void name(void* d, void* n, int f) { (void)d; tr += tag + o(n) + "," + std::to_string(f) + ";"; }
#define STUB2(name, tag) extern "C" void name(void* d, void* n) { (void)d; tr += tag + o(n) + ";"; }
STUB3(ProcessCreateEmitter__18VISpellEventDriverP18VISpellActiveEventb, "pe")
STUB2(ProcessSetEmitterMotif__18VISpellEventDriverP18VISpellActiveEvent, "pm")
STUB3(ProcessCreateSprite__18VISpellEventDriverP18VISpellActiveEventb, "ps")
STUB3(ProcessCreateSound__18VISpellEventDriverP18VISpellActiveEventb, "pn")
STUB2(ProcessFadeSprite__18VISpellEventDriverP18VISpellActiveEvent, "pf")
STUB2(NewSetAction__18VISpellEventDriverP12VISpellEvent, "N2:")
STUB2(NewCreateEmitter__18VISpellEventDriverP12VISpellEvent, "N3:")
STUB2(NewSetEmitterMotif__18VISpellEventDriverP12VISpellEvent, "N6:")
STUB2(NewPlayTargetProjectile__18VISpellEventDriverP12VISpellEvent, "N8:")
STUB2(NewCreateSprite__18VISpellEventDriverP12VISpellEvent, "N9:")
STUB2(NewDestroySprite__18VISpellEventDriverP12VISpellEvent, "N10:")
STUB2(NewCreateSound__18VISpellEventDriverP12VISpellEvent, "N11:")
STUB2(NewDestroySound__18VISpellEventDriverP12VISpellEvent, "N12:")
STUB2(NewPlayTargetLightning__18VISpellEventDriverP12VISpellEvent, "N13:")
STUB2(NewFadeSprite__18VISpellEventDriverP12VISpellEvent, "N14:")
STUB2(NewPlayAttack__18VISpellEventDriverP12VISpellEvent, "N15:")
STUB2(NewShakeCamera__18VISpellEventDriverP12VISpellEvent, "N17:")
extern "C" void* __builtin_new(unsigned n) { tr += "new" + std::to_string(n) + ";"; return alloc_ret; }
extern "C" void __18VISpellActiveEventP12VISpellEventP7VIScene(void* s, void* e, void* sc) { tr += "ctor" + o(s) + "," + o(e) + "," + o(sc) + ";"; }
extern "C" long long GetTime__7VIScene(void* sc) { tr += "time" + o(sc) + ";"; return 0x123456789abcdef0ll; }
extern "C" void Remove__7VILNode(void* n) { tr += "rm" + o(n) + ";"; }
extern "C" void AddTail__7VILListP7VILNode(void* l, void* n) { tr += "add" + o(l) + "," + o(n) + ";"; }
extern "C" void AddSpellEventDriver__19VISpellEffectDriverP16VISpellEventListi(void* e, void* l, int a) { tr += "asd" + o(e) + "," + o(l) + "," + std::to_string(a) + ";"; }

enum { EFF = 0x1000, EFFDEF = 0x1200, SCENE = 0x1400, TABLE = 0x1600, RECS = 0x1800, N0 = 0x2000, N1 = 0x2200, N2 = 0x2400, EV0 = 0x2800, EV1 = 0x2900, L2 = 0x3000, NEWP = 0x3400 };
static unsigned char* rec0 = arena + RECS;
static void setup(int rtype, int count = 1) {
    memset(arena, 0, sizeof arena); tr.clear(); alloc_ret = arena + NEWP; proj_clears = 0;
    *(unsigned*)(drv + 16) = EFF; *(unsigned*)(drv + 20) = SCENE; *(int*)(drv + 24) = 2; *(int*)(drv + 28) = 0;
    *(unsigned*)(drv + 72) = TABLE; *(int*)(arena + TABLE + 4) = count; *(unsigned*)(arena + TABLE + 8) = RECS;
    *(int*)rec0 = rtype;
    *(unsigned*)(drv + 76) = N2; *(unsigned*)(drv + 88) = L2;          // empty active list: head is the tail node (next == 0)
    *(unsigned*)(arena + EFF) = EFFDEF; *(int*)(arena + EFF + 12) = 0x31; *(int*)(arena + EFF + 96) = 2; *(unsigned*)(arena + EFF + 84) = 1; *(int*)(arena + EFF + 88) = 0x5151;
}
static void twoNodes(int t0, int id0, int t1, int id1) {         // N0 -> N1 -> tail N2
    *(unsigned*)(drv + 76) = N0; *(unsigned*)(arena + N0) = N1; *(unsigned*)(arena + N1) = N2;
    *(unsigned*)(arena + N0 + 116) = EV0; *(unsigned*)(arena + N1 + 116) = EV1;
    *(int*)(arena + EV0) = t0; *(int*)(arena + EV0 + 4) = id0; *(int*)(arena + EV1) = t1; *(int*)(arena + EV1 + 4) = id1;
}
static int proc() { return Process__18VISpellEventDriver(drv); }

int main() {
    // nothing to do: the table is exhausted and everything is empty
    setup(0, 0);
    if (proc() != 0 || tr != "I" + o(drv) + ";") return 1;
    // projectile flag: processed, and still set -> returns 1 at once; cleared -> continues
    setup(0, 0); *(int*)(drv + 36) = 5;
    if (proc() != 1 || tr != "P" + o(drv) + ";") return 2;
    setup(0, 0); *(int*)(drv + 36) = 5; proj_clears = 1;
    if (proc() != 0 || tr != "P" + o(drv) + ";I" + o(drv) + ";") return 3;
    // lightning and wait event processing; a wait event stops the table and makes the result 1
    setup(0, 1); *(int*)(drv + 48) = 1; *(unsigned*)(drv + 32) = 0x3800;
    if (proc() != 1 || tr != "L" + o(drv) + ";W" + o(drv) + ";I" + o(drv) + ";" || *(int*)(drv + 28) != 0) return 4;
    // active list: the tail node is not visited, event types 3, 6, 9, 11, 14 are dispatched, others ignored
    setup(0, 0); twoNodes(3, 0, 14, 0);
    if (proc() != 1 || tr != "I" + o(drv) + ";pe" + std::to_string(N0) + ",1;pf" + std::to_string(N1) + ";") return 5;
    setup(0, 0); twoNodes(6, 0, 9, 0);
    proc(); if (tr != "I" + o(drv) + ";pm" + std::to_string(N0) + ";ps" + std::to_string(N1) + ",1;") return 6;
    setup(0, 0); twoNodes(11, 0, 4, 0);
    proc(); if (tr != "I" + o(drv) + ";pn" + std::to_string(N0) + ",1;") return 7;
    // simple table record types call the matching New function with (driver, record) and advance the index
    struct { int t; const char* tag; } simple[] = {{2, "N2:"}, {3, "N3:"}, {6, "N6:"}, {8, "N8:"}, {9, "N9:"}, {10, "N10:"}, {11, "N11:"}, {12, "N12:"}, {13, "N13:"}, {14, "N14:"}, {15, "N15:"}, {17, "N17:"}};
    for (auto& s : simple) {
        setup(s.t, 2);
        if (proc() != 1 || tr != "I" + o(drv) + ";" + s.tag + std::to_string(RECS) + ";" || *(int*)(drv + 28) != 1) return 8;
    }
    // unknown record types only advance; reaching the count ends the run with 0
    for (int t : {0, 18, -1}) {
        setup(t, 1);
        if (proc() != 0 || tr != "I" + o(drv) + ";" || *(int*)(drv + 28) != 1) return 9;
    }
    // type 1: creates the wait event and stamps the time at +176
    setup(1, 2);
    if (proc() != 1 || tr != "I" + o(drv) + ";new184;ctor" + std::to_string(NEWP) + "," + std::to_string(RECS) + "," + std::to_string(SCENE) + ";time" + std::to_string(SCENE) + ";") return 10;
    if (*(unsigned*)(drv + 32) != NEWP || *(long long*)(arena + NEWP + 176) != 0x123456789abcdef0ll || *(int*)(drv + 28) != 1) return 11;
    // type 4: finds the type-3 node with the record id (node 1 here), moves it to the list at +88
    setup(4, 2); twoNodes(5, 9, 3, 9); *(int*)(rec0 + 4) = 9; *(unsigned*)(arena + N1) = N2; *(unsigned*)(arena + N2) = 0;
    *(unsigned*)(arena + N1) = 0x2600; *(unsigned*)(arena + 0x2600) = 0;     // N0 -> N1 -> N3 -> (0); the node N1 has a next
    *(unsigned*)(arena + N1 + 116) = EV1;
    if (proc() != 1) return 12;
    if (tr.find("rm" + std::to_string(N1) + ";add" + std::to_string(0x100 + 88) + "," + std::to_string(N1) + ";") == std::string::npos || *(int*)(drv + 28) != 1) return 13;
    // type 4 without a match only advances
    setup(4, 2); twoNodes(5, 9, 3, 7); *(int*)(rec0 + 4) = 9; *(unsigned*)(arena + N1) = 0x2600; *(unsigned*)(arena + 0x2600) = 0;
    if (proc() != 1 || tr.find("rm") != std::string::npos || *(int*)(drv + 28) != 1) return 14;
    // type 5: updates +132 and the float at +136 of the matching node
    setup(5, 2); twoNodes(3, 9, 5, 5); *(int*)(rec0 + 4) = 9; *(int*)(rec0 + 8) = 77; *(float*)(rec0 + 20) = 2.5f; *(unsigned*)(arena + N1) = 0x2600; *(unsigned*)(arena + 0x2600) = 0;
    proc(); if (*(int*)(arena + N0 + 132) != 77 || *(float*)(arena + N0 + 136) != 2.5f || *(int*)(drv + 28) != 1) return 15;
    // type 7: bit 1 adds the first list entry, bit 2 adds the listed drivers
    setup(7, 2); *(int*)(rec0 + 4) = 3; *(unsigned*)(arena + EFFDEF) = 0; *(unsigned*)(arena + EFF) = EFFDEF; *(int*)(arena + EFF + 12) = 0x31; *(int*)(arena + EFF + 16) = 2;
    *(int*)(arena + EFF + 20) = 100; *(int*)(arena + EFF + 24) = 101;
    proc();
    if (tr != "I" + o(drv) + ";asd" + std::to_string(EFF) + "," + std::to_string(EFFDEF + 24) + ",49;asd" + std::to_string(EFF) + "," + std::to_string(EFFDEF + 36) + ",100;asd" + std::to_string(EFF) + "," + std::to_string(EFFDEF + 36) + ",101;") return 16;
    // type 16: effect+96 must equal the int at driver+24, the callback word at effect+84 must be set
    setup(16, 2); *(int*)(rec0 + 4) = 1;
    proc(); if (tr != "I" + o(drv) + ";cam1,80,200,20817;" || *(int*)(drv + 28) != 1) return 17;
    setup(16, 2); *(int*)(rec0 + 4) = 2;
    proc(); if (tr.find("cam1,220,140,20817;") == std::string::npos) return 18;
    setup(16, 2); *(int*)(rec0 + 4) = 0; *(unsigned char*)(rec0 + 8) = 7; *(unsigned char*)(rec0 + 12) = 9; *(int*)(rec0 + 16) = 33;
    proc(); if (tr.find("cam7,9,33,20817;") == std::string::npos) return 19;
    setup(16, 2); *(int*)(rec0 + 4) = 5;
    proc(); if (tr.find("cam1,100,0,20817;") == std::string::npos) return 20;
    setup(16, 2); *(int*)(arena + EFF + 96) = 3;
    proc(); if (tr.find("cam") != std::string::npos || *(int*)(drv + 28) != 1) return 21;
    setup(16, 2); *(unsigned*)(arena + EFF + 84) = 0;
    proc(); if (tr.find("cam") != std::string::npos || *(int*)(drv + 28) != 1) return 22;
    // the end test: exhausted table but a node in the second list, or a lightning pointer, keeps the result 1
    setup(0, 0); *(unsigned*)(arena + L2) = 0x3500;
    if (proc() != 1) return 23;
    setup(0, 0); *(int*)(drv + 48) = 1;
    if (proc() != 1) return 24;
    setup(0, 1); *(int*)(drv + 28) = 2;
    if (proc() != 1) return 25;
    return 0;
}
