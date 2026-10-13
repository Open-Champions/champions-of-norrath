// Host contract for executeAction__FP6Action. Not an EE plan input.
// Build: c++ -std=c++17 -w host_check.cpp
// Object and table addresses are 32-bit offsets into one host arena; every callee is a
// recording stub, and virtual slots are stub functions identified by their word.
#define NORRATH_HOST_ARENA
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#define sprintf w_sprintf
#define strstr w_strstr
#define strlen w_strlen
#include "execute_action.cpp"
#undef sprintf
#undef strstr
#undef strlen

static unsigned char arena[1 << 20];
extern "C" unsigned char* norrath_host_base = arena;
static std::string ev;
static std::string num(long v) { return std::to_string(v); }
static std::string fl(float f) { char b[32]; std::snprintf(b, sizeof b, "%g", f); return b; }
static unsigned W(const void* p) { return (unsigned)((const unsigned char*)p - arena); }
extern "C" void norrath_host_slot_call(unsigned function, unsigned char* self, int argument) {
    ev += "slot" + num(function) + "(" + num(W(self)) + "," + num(argument) + ");";
}

extern "C" {
signed char cutPlayMode, cutSkipMode;
int musicSize, displayTextDisable, displayMusicPos, displayDialogPos, nmDialogNodes, animationLoadPos;
unsigned dlgDir, dialogPacket, actor, dialogActor, anim[4];
int comp = -1; unsigned idleHeader, talkHeader, objectToFind;
int lumpEntries; unsigned tinfoWord = 1;

void worldSetFarClip__Ff(float f) { ev += "far(" + fl(f) + ");"; }
void soundContinueStream__Fi(int s) { ev += "cont" + num(s) + ";"; }
int soundGetDialogSize__FPc(char* n) { ev += std::string("size(") + n + ");"; return 5000; }
int soundGetDialogSize__Fi(int i) { ev += "sizei" + num(i) + ";"; return 900 + i; }
void soundStartDialogStream__FiPci(int s, char* n, int t) { ev += "dlg(" + num(s) + "," + n + "," + num(t) + ");"; }
void soundStartMusic__Fii(int a, int b) { ev += "music(" + num(a) + "," + num(b) + ");"; }
int animFindComponentWithFlags__FP14AnimationStatei(unsigned char* s, int f) { ev += "find(" + num(W(s)) + "," + num(f) + ");"; return comp; }
void animRemove__FP14AnimationStateP15AnimationHeaderf(unsigned char* s, unsigned char* h, float f) { ev += "rm(" + num(W(s)) + "," + num(W(h)) + "," + fl(f) + ");"; }
unsigned char* objectFindObject__FPc(char* n) { ev += std::string("obj(") + n + ");"; return objectToFind ? arena + objectToFind : nullptr; }
unsigned char* objectFindAnimation__FP10GameObjectPc(unsigned char* o, char* n) {
    ev += std::string("anim(") + num(W(o)) + "," + n + ");";
    unsigned h = std::strcmp(n, "idle") == 0 ? idleHeader : talkHeader;
    return h ? arena + h : nullptr;
}
void animInitAnimation__FP15AnimationHeader(unsigned char* h) { ev += "init" + num(W(h)) + ";"; }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(unsigned char* s, unsigned char* h, float f, int n) { ev += "trans(" + num(W(s)) + "," + num(W(h)) + "," + fl(f) + "," + num(n) + ");"; }
void animAddToEnd__FP14AnimationStateP15AnimationHeaderi(unsigned char* s, unsigned char* h, int n) { ev += "end(" + num(W(s)) + "," + num(W(h)) + "," + num(n) + ");"; }
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(unsigned char* s, unsigned char* h, float a, float b, int n) { ev += "shot(" + num(W(s)) + "," + num(W(h)) + "," + fl(a) + "," + fl(b) + "," + num(n) + ");"; }
void listDestroy__FP8ListHead(unsigned char* l) { ev += "ldestroy" + num(W(l)) + ";"; }
void listOnewayLink__FP8ListHeadT0Pv(unsigned char* l, unsigned char* a, unsigned char* b) { ev += "llink(" + num(W(l)) + "," + num(W(a)) + "," + num(W(b)) + ");"; }
void objectUpdateInGrid__FP10GameObject(unsigned char* o) { ev += "upd" + num(W(o)) + ";"; }
void objectRemoveFromGrid__FP10GameObject(unsigned char* o) { ev += "rem" + num(W(o)) + ";"; }
void objectAlwaysDraw__FP10GameObject(unsigned char* o) { ev += "always" + num(W(o)) + ";"; }
int w_sprintf(char* d, const char* f, ...) { va_list ap; va_start(ap, f); int n = std::vsnprintf(d, 80, f, ap); va_end(ap); return n; }
char* w_strstr(const char* a, const char* b) { return const_cast<char*>(std::strstr(a, b)); }
unsigned w_strlen(const char* s) { return (unsigned)std::strlen(s); }
unsigned char* lumpLoad__FPc(char* n) { ev += std::string("load(") + n + ");"; return arena + 0x30000; }
unsigned char* lumpFind__FPvPc(unsigned char*, char* n) { ev += std::string("lfind(") + n + ");"; return arena + 0x30100; }
unsigned char* lumpFindByType__FPvPci(unsigned char*, char* t, int i) { ev += std::string("lbt(") + t + "," + num(i) + ");"; return i < lumpEntries ? arena + 0x30200 + 8 * i : nullptr; }
void* __tf3NPC() { return (void*)(arena + 1); }
int __eq__C9type_infoRC9type_info(void* a, void* b) { return a == b; }
void __throw_bad_typeid() { ev += "throw;"; }
}

static void put32(unsigned at, unsigned v) { std::memcpy(arena + at, &v, 4); }
static unsigned get32(unsigned at) { unsigned v; std::memcpy(&v, arena + at, 4); return v; }
static void put16(unsigned at, unsigned short v) { std::memcpy(arena + at, &v, 2); }
static void putf(unsigned at, float v) { std::memcpy(arena + at, &v, 4); }
static float getf(unsigned at) { float v; std::memcpy(&v, arena + at, 4); return v; }
static void rec(unsigned at, int type, int kind, int a, int b, int c) {
    putf(at, 1.5f); put16(at + 4, (unsigned short)type); put16(at + 6, (unsigned short)kind);
    put32(at + 8, a); put32(at + 12, b); put32(at + 16, c);
}
static unsigned long long get64(unsigned at) { unsigned long long v; std::memcpy(&v, arena + at, 8); return v; }
static void put64(unsigned at, unsigned long long v) { std::memcpy(arena + at, &v, 8); }

static const unsigned REC = 0x100, ACT = 0x1000, OBJ0 = 0x2000, OBJ1 = 0x3000, VT = 0x4000;
static int fail(int n, const char* m) { std::printf("fail %d: %s\n  events: %s\n", n, m, ev.c_str()); return n; }
#define EXPECT(n, cond) do { if (!(cond)) return fail(n, #cond); } while (0)
#define EVENTS(n, text) do { std::string want_ = std::string("") + (text); if (ev != want_) { std::printf("expected: %s\n", want_.c_str()); return fail(n, "events"); } } while (0)

static void world() {
    std::memset(arena, 0, sizeof arena);
    ev.clear();
    actor = ACT; put32(ACT, OBJ0); put32(ACT + 44, OBJ1);
    for (unsigned o : {OBJ0, OBJ1}) { put32(o + 12, 0x20); put32(o + 72, VT); }
    // virtual table: slot at +24 and +184 each {short adjust; function word at +4}; +4 is the type word function
    put16(VT + 24, 4); put32(VT + 28, 24);
    put16(VT + 184, 8); put32(VT + 188, 184);
    dialogActor = 0; cutPlayMode = 0; cutSkipMode = 0; comp = -1; idleHeader = talkHeader = 0; objectToFind = 0;
    dialogPacket = 0; dlgDir = 0; nmDialogNodes = 0; displayDialogPos = 0; displayMusicPos = 0; animationLoadPos = 0;
    displayTextDisable = 5; musicSize = 0; anim[0] = anim[1] = anim[2] = anim[3] = 0; lumpEntries = 0;
}

int main() {
    // type -100: only kind 6 acts
    world(); rec(REC, -100, 6, 7, 0, 0); executeAction__FP6Action(arena + REC); EVENTS(1, "far(84);");
    world(); rec(REC, -100, 5, 7, 0, 0); executeAction__FP6Action(arena + REC); EVENTS(2, "");
    // type -99 kind 1
    world(); rec(REC, -99, 1, 0, 0, 0); executeAction__FP6Action(arena + REC); EXPECT(3, displayTextDisable == 1);
    // other negative types and an unmatched kind
    world(); rec(REC, -98, 0, 0, 0, 0); executeAction__FP6Action(arena + REC); EVENTS(4, "");
    world(); rec(REC, -99, 3, 0, 0, 0); executeAction__FP6Action(arena + REC); EVENTS(5, ""); EXPECT(6, displayTextDisable == 5);
    // object kinds
    world(); put32(OBJ0 + 12, 0x20); put64(OBJ0 + 160, 0x0f); rec(REC, 0, 12, 0, 0, 0);
    executeAction__FP6Action(arena + REC); EXPECT(7, get64(OBJ0 + 160) == (0x0full ^ 0x200000ull));
    world(); put32(OBJ0 + 12, 0); put64(OBJ0 + 160, 0x0f); rec(REC, 0, 12, 0, 0, 0);
    executeAction__FP6Action(arena + REC); EXPECT(8, get64(OBJ0 + 160) == 0x0full);
    world(); rec(REC, 0, 11, 0x12345, 0, 0); executeAction__FP6Action(arena + REC);
    EXPECT(9, (arena[OBJ0 + 56] | arena[OBJ0 + 57] << 8) == 0x2345);
    world(); rec(REC, 1, 9, 0, 0, 0); executeAction__FP6Action(arena + REC); EVENTS(10, "always" + num(OBJ1) + ";");
    world(); arena[OBJ0 + 570] = 0x01; rec(REC, 0, 7, 0, 0, 0); executeAction__FP6Action(arena + REC);
    EXPECT(11, arena[OBJ0 + 570] == 0x09); EVENTS(12, "rem" + num(OBJ0) + ";");
    world(); arena[OBJ0 + 570] = 0xff; rec(REC, 0, 8, 0, 0, 0); executeAction__FP6Action(arena + REC);
    EXPECT(13, arena[OBJ0 + 570] == 0xf7); EVENTS(14, "upd" + num(OBJ0) + ";");
    world(); rec(REC, 0, 3, 0, 0, 0); executeAction__FP6Action(arena + REC); EVENTS(15, "slot24(" + num(OBJ0 + 4) + ",1);");
    world(); rec(REC, 0, 4, 0, 0, 0); executeAction__FP6Action(arena + REC); EVENTS(16, "slot24(" + num(OBJ0 + 4) + ",0);");
    world(); arena[OBJ0 + 570] = 0x0f; rec(REC, 0, 0, 3, 4, 5); executeAction__FP6Action(arena + REC);
    EVENTS(17, "slot184(" + num(OBJ0 + 8) + ",1);"); EXPECT(18, arena[OBJ0 + 570] == 0x07);
    EXPECT(19, getf(OBJ0 + 272) == 3.0f && getf(OBJ0 + 276) == 4.0f && getf(OBJ0 + 280) == 5.0f);
    world(); rec(REC, 0, 10, 3, 4, 5); dialogActor = 0; executeAction__FP6Action(arena + REC);
    EXPECT(20, getf(OBJ0 + 16) == 3.0f && getf(OBJ0 + 20) == 4.0f && getf(OBJ0 + 24) == 5.0f && getf(OBJ0 + 60) == 5.0f);
    EVENTS(21, "upd" + num(OBJ0) + ";slot184(" + num(OBJ0 + 8) + ",0);");
    world(); rec(REC, 0, 10, 3, 4, 5); dialogActor = OBJ0; executeAction__FP6Action(arena + REC);   // the dialog actor is not told
    EXPECT(22, getf(OBJ0 + 60) == 5.0f); EVENTS(23, "upd" + num(OBJ0) + ";");
    world(); put32(OBJ0 + 12, 0); rec(REC, 0, 10, 3, 4, 5); executeAction__FP6Action(arena + REC);   // flag clear: no copy, no slot
    EXPECT(24, getf(OBJ0 + 60) == 0.0f); EVENTS(25, "upd" + num(OBJ0) + ";");
    world(); rec(REC, 1, 1, 0, 0, 0); executeAction__FP6Action(arena + REC);   // relink to actor 0's object
    EVENTS(26, "ldestroy" + num(OBJ1 + 576) + ";llink(" + num(OBJ1 + 576) + "," + num(OBJ0) + "," + num(OBJ0) + ");slot184(" + num(OBJ1 + 8) + ",3);");
    world(); rec(REC, 1, 1, -1, 0, 0); executeAction__FP6Action(arena + REC);   // negative index: only the destroy
    EVENTS(27, "ldestroy" + num(OBJ1 + 576) + ";");
    world(); rec(REC, 2, 1, 0, 0, 0); executeAction__FP6Action(arena + REC); EVENTS(28, "");   // type 2: no such actor object
    world(); rec(REC, 0, 13, 0, 0, 0); executeAction__FP6Action(arena + REC); EVENTS(29, "");   // kind out of range
    // dialog music (type -99, kind 7)
    world(); dlgDir = 0x5000; arena[0x5000 + 80] = 1; displayMusicPos = 1; cutPlayMode = 1; rec(REC, -99, 7, 0, 0, 0);
    executeAction__FP6Action(arena + REC);
    EVENTS(30, "cont0;sizei1;music(2,136);"); EXPECT(31, musicSize == 901 && displayMusicPos == 2);
    world(); dlgDir = 0x5000; arena[0x5000 + 80] = 1; displayMusicPos = 1; cutPlayMode = 1; cutSkipMode = 1; rec(REC, -99, 7, 0, 0, 0);
    executeAction__FP6Action(arena + REC); EVENTS(32, ""); EXPECT(33, displayMusicPos == 2);
    world(); dlgDir = 0x5000; arena[0x5000 + 80] = 0; displayMusicPos = 1; cutPlayMode = 1; rec(REC, -99, 7, 0, 0, 0);
    executeAction__FP6Action(arena + REC); EVENTS(34, ""); EXPECT(35, displayMusicPos == 1);
    world(); dlgDir = 0x5000; arena[0x5000 + 80] = 1; displayMusicPos = 1; cutPlayMode = 0; rec(REC, -99, 7, 0, 0, 0);
    executeAction__FP6Action(arena + REC); EVENTS(36, ""); EXPECT(37, displayMusicPos == 2);
    // dialog line (type -99, kind 0)
    world(); dialogPacket = 0; rec(REC, -99, 0, 0, 0, 0); executeAction__FP6Action(arena + REC); EXPECT(38, displayTextDisable == 5);
    world(); dialogPacket = 0x6000; dlgDir = 0; rec(REC, -99, 0, 0, 0, 0); executeAction__FP6Action(arena + REC); EXPECT(39, displayTextDisable == 0);
    {   // with sound, two nodes, first line: size of this line, start of the next, then the position advances
        world(); dialogPacket = 0x6000; dlgDir = 0x5000; nmDialogNodes = 2; displayDialogPos = 0; cutPlayMode = 1;
        std::strcpy((char*)arena + 0x6000 + 49, "one"); std::strcpy((char*)arena + 0x6000 + 1096 + 49, "two");
        rec(REC, -99, 0, 0, 0, 0); executeAction__FP6Action(arena + REC);
        EVENTS(40, "cont2;size(one.vag);dlg(2,two.vag,40);"); EXPECT(41, displayDialogPos == 1 && displayTextDisable == 0);
    }
    {   // last node: no next line; the position advances past it
        world(); dialogPacket = 0x6000; dlgDir = 0x5000; nmDialogNodes = 2; displayDialogPos = 1; cutPlayMode = 1;
        std::strcpy((char*)arena + 0x6000 + 1096 + 49, "two"); rec(REC, -99, 0, 0, 0, 0); executeAction__FP6Action(arena + REC);
        EVENTS(42, "cont2;size(two.vag);"); EXPECT(43, displayDialogPos == 2);
    }
    {   // position past the end: only advanced
        world(); dialogPacket = 0x6000; dlgDir = 0x5000; nmDialogNodes = 2; displayDialogPos = 2; rec(REC, -99, 0, 0, 0, 0);
        executeAction__FP6Action(arena + REC); EVENTS(44, ""); EXPECT(45, displayDialogPos == 3);
    }
    {   // a speaker named in the line: the old one loses bit 28, the new one gains it; idle and talk are queued
        world(); dialogPacket = 0x6000; dlgDir = 0x5000; nmDialogNodes = 2; displayDialogPos = 0;
        std::strcpy((char*)arena + 0x6000 + 65, "hero"); objectToFind = OBJ1; dialogActor = OBJ0; put64(OBJ0 + 160, ~0ull);
        idleHeader = 0x7000; talkHeader = 0x7100; arena[OBJ1 + 292] = 0; animationLoadPos = 0;
        comp = -1; rec(REC, -99, 0, 0, 0, 0); executeAction__FP6Action(arena + REC);
        EXPECT(46, get64(OBJ0 + 160) == (~0ull & ~0x10000000ull));
        EXPECT(47, dialogActor == OBJ1 && (get64(OBJ1 + 160) & 0x10000000ull));
        EXPECT(48, ev.find("obj(hero);") != std::string::npos);
        EXPECT(49, ev.find("trans(" + num(OBJ1 + 288) + "," + num(0x7100) + ",0.3,3);") != std::string::npos);
        EXPECT(50, ev.find("trans(" + num(OBJ1 + 288) + "," + num(0x7000) + ",0.4,3);") != std::string::npos);
        EXPECT(51, displayDialogPos == 1);
        EXPECT(52, getf(OBJ1 + 288) < 0.0f);      // the computed negated length lands on the first component
    }
    {   // a speaker that does not carry flag 0x20 clears dialogActor
        world(); dialogPacket = 0x6000; dlgDir = 0x5000; nmDialogNodes = 1; displayDialogPos = 0;
        std::strcpy((char*)arena + 0x6000 + 65, "hero"); objectToFind = OBJ1; put32(OBJ1 + 12, 0); dialogActor = OBJ0;
        rec(REC, -99, 0, 0, 0, 0); executeAction__FP6Action(arena + REC); EXPECT(53, dialogActor == 0);
    }
    {   // anim table path: row (position & 1) with a nonzero second word reuses its header
        world(); dialogPacket = 0x6000; dlgDir = 0x5000; nmDialogNodes = 3; displayDialogPos = 1; animationLoadPos = 3;
        dialogActor = OBJ0; idleHeader = 0x7000; talkHeader = 0; anim[0] = 0x7300; anim[1] = 1; anim[2] = 0x7400; anim[3] = 1;
        arena[OBJ0 + 292] = 1;
        rec(REC, -99, 0, 0, 0, 0); executeAction__FP6Action(arena + REC);
        EVENTS(54, "find(" + num(OBJ0 + 288) + ",32);anim(" + num(OBJ0) + ",idle);init" + num(0x7400) + ";trans(" + num(OBJ0 + 288) + "," +
                       num(0x7400) + ",0.3,38);end(" + num(OBJ0 + 288) + "," + num(0x7000) + ",3);");
        EXPECT(55, anim[3] == 2 && anim[1] == 0 && anim[0] == 0x7300 && anim[2] == 0x7400);   // odd position: row 0's word is cleared first
        EXPECT(56, displayDialogPos == 2);
        // the queued idle component is the last one: +12 gets 1/30, +8 is zeroed, +24 gains 30
        EXPECT(57, getf(OBJ0 + 308) > 0.0333f && getf(OBJ0 + 308) < 0.0334f && get32(OBJ0 + 304) == 0 && getf(OBJ0 + 320) == 30.0f);
    }
    // animation kind (kind 2): the a-th ".anm" entry and the blend by b
    {
        world(); unsigned list = 0x8000; put32(OBJ0 + 36, list);
        put32(list + 0, 0x8100); put32(list + 4, 0x9000); std::strcpy((char*)arena + 0x8100, "x");
        put32(list + 8, 0x8110); put32(list + 12, 0x9100); std::strcpy((char*)arena + 0x8110, "a.anm");
        put32(list + 16, 0x8120); put32(list + 20, 0x9200); std::strcpy((char*)arena + 0x8120, "b.anm");
        rec(REC, 0, 2, 1, 1, 0); executeAction__FP6Action(arena + REC);   // second ".anm" entry, transition 4
        EVENTS(70, "trans(" + num(OBJ0 + 288) + "," + num(0x9200) + ",0.2,4);");
        world(); put32(OBJ0 + 36, list); put32(list + 0, 0x8100); put32(list + 4, 0x9000); std::strcpy((char*)arena + 0x8100, "a.anm");
        rec(REC, 0, 2, 0, 0, 0); executeAction__FP6Action(arena + REC);   // one-shot
        EVENTS(71, "shot(" + num(OBJ0 + 288) + "," + num(0x9000) + ",0.2,0.2,0);");
        world(); put32(OBJ0 + 36, list); put32(list + 0, 0x8100); put32(list + 4, 0x9000); std::strcpy((char*)arena + 0x8100, "a.anm");
        rec(REC, 0, 2, 0, 3, 0); executeAction__FP6Action(arena + REC);
        EVENTS(72, "shot(" + num(OBJ0 + 288) + "," + num(0x9000) + ",0.02,0.02,0);");
        world(); put32(OBJ0 + 36, list); put32(list + 0, 0x8100); put32(list + 4, 0x9000); std::strcpy((char*)arena + 0x8100, "a.anm");
        rec(REC, 0, 2, 0, 4, 0); executeAction__FP6Action(arena + REC); EVENTS(73, "");   // b = 4 does nothing
        world(); put32(OBJ0 + 36, list); put32(list + 0, 0x8100); put32(list + 4, 0x9000); std::strcpy((char*)arena + 0x8100, "a.anm");
        rec(REC, 0, 2, 0, 7, 0); executeAction__FP6Action(arena + REC); EVENTS(74, "");   // b out of range
        world(); put32(OBJ0 + 36, list); put32(list + 0, 0x8100); put32(list + 4, 0x9000); std::strcpy((char*)arena + 0x8100, "a.anm");
        rec(REC, 0, 2, 5, 0, 0); executeAction__FP6Action(arena + REC); EVENTS(75, "");   // fewer entries than a
    }
    return 0;
}
