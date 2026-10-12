// Host contract for initAnimation__Fv. Not an EE plan input.
#include "init_animation.cpp"
#include <cstring>
#include <cstdint>
extern "C" {
unsigned char* dialogCreature;
void* dialogIdleAnimation;
signed char animUseBuffer;
AnimRow anim[4];
}
struct Call { int kind; void* a; void* b; float f; int m; };
static Call calls[8];
static int ncalls;
static void* findResult;
static const char* findName;
extern "C" void* objectFindAnimation__FP10GameObjectPc(unsigned char* o, const char* n) {
    calls[ncalls++] = Call{1, o, 0, 0, 0}; findName = n; return findResult; }
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* s, void* h, float f, int m) {
    calls[ncalls++] = Call{2, s, h, f, m}; }
extern "C" void animInitAnimation__FP15AnimationHeader(void* h) { calls[ncalls++] = Call{3, h, 0, 0, 0}; }
extern "C" void animAddToEnd__FP14AnimationStateP15AnimationHeaderi(void* s, void* h, int m) {
    calls[ncalls++] = Call{4, s, h, 0, m}; }
static unsigned bits(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
alignas(16) static unsigned char creature[512];
int main() {
    int hdrA, hdrB, hdrIdle, hdrFound;
    // null creature: nothing
    dialogCreature = nullptr; initAnimation__Fv();
    if (ncalls != 0) return 1;
    dialogCreature = creature;
    animUseBuffer = 2;
    anim[2].header = &hdrA; anim[2].flag = 0; anim[2].count = 1000;
    anim[1].header = &hdrB; anim[1].flag = 1;
    // flag zero, no talk animation: one lookup only
    findResult = nullptr; dialogIdleAnimation = &hdrIdle; ncalls = 0;
    initAnimation__Fv();
    if (ncalls != 1 || calls[0].kind != 1 || calls[0].a != creature || std::strcmp(findName, "talk")) return 2;
    // flag zero, found, idle absent: one transition
    findResult = &hdrFound; dialogIdleAnimation = nullptr; ncalls = 0;
    initAnimation__Fv();
    if (ncalls != 2 || calls[1].kind != 2 || calls[1].a != creature + 288 || calls[1].b != &hdrFound ||
        bits(calls[1].f) != 0x3e999999u || calls[1].m != 3) return 3;
    // flag zero, found, idle present
    dialogIdleAnimation = &hdrIdle; ncalls = 0; creature[292] = 1;
    std::memset(creature + 288 + 32, 0, 4);
    initAnimation__Fv();
    if (ncalls != 3 || calls[2].kind != 2 || calls[2].b != &hdrIdle || bits(calls[2].f) != 0x3ecccccc ||
        calls[2].m != 3) return 4;
    float stored; std::memcpy(&stored, creature + 288 + 32, 4);
    float expect = -(1000.0f * __builtin_bit_cast(float, 0x3898eadbu) * 60.0f);
    if (stored != expect || !(stored < 0.0f)) return 5;
    // negative state index selects the row below
    creature[292] = 0xff; std::memset(creature + 288 - 32, 0, 4);
    initAnimation__Fv();
    std::memcpy(&stored, creature + 288 - 32, 4);
    if (stored != expect) return 6;
    // flag nonzero with idle
    animUseBuffer = 1; ncalls = 0;
    initAnimation__Fv();
    if (ncalls != 3 || calls[0].kind != 3 || calls[0].a != &hdrB || calls[1].kind != 2 || calls[1].b != &hdrB ||
        bits(calls[1].f) != 0x3e999999u || calls[1].m != 38 || calls[2].kind != 4 || calls[2].b != &hdrIdle ||
        calls[2].m != 3 || calls[2].a != creature + 288) return 7;
    // flag nonzero without idle
    dialogIdleAnimation = nullptr; ncalls = 0;
    initAnimation__Fv();
    if (ncalls != 2) return 8;
    return 0;
}
