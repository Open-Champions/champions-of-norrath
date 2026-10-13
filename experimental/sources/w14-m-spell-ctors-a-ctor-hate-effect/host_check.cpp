// Host contract for __15SpellHateEffectiiP8CreatureP10GameObjectP5Skill; not an EE input. The EE image stores
// 32-bit addresses; this host build uses 64-bit pointers, so the checks compare stored words with low() and do
// not depend on pointer width for the object fields. Covers the two conditional list links, their arguments, the
// frame value, and the fields.
#include "spell_hate_effect_ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_SpellHateEffect[8];
unsigned syncRandState;
int frameNm;
static std::string calls;
static unsigned char* objectBase;
static void* gameObjectArg;
static void* creatureArg;
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data) {
    const long offset = static_cast<unsigned char*>(head) - objectBase;
    const char* who = link == gameObjectArg ? "G" : link == creatureArg ? "C" : "?";
    const char* owner = data == gameObjectArg ? "G" : data == creatureArg ? "C" : "?";
    char b[64];
    std::snprintf(b, sizeof b, "link(+%ld,%s,%s);", offset, who, owner);
    calls += b;
}

int main() {
    int bad = 0;
    alignas(16) unsigned char object[96];
    alignas(16) unsigned char skillBuffer[32];
    alignas(16) unsigned char creature[16];
    alignas(16) unsigned char go[16];
    const unsigned seed = 0xabcdef01u;
    const unsigned next = seed * 0x19660du + 0x3c6ef35fu;
    struct Case { int first; int kind; bool hasCreature; bool hasGo; int frame; };
    const Case cases[] = {
        {0, 0x12, true, true, 5},
        {3, -2, true, false, 5},
        {-1, 0x80, false, true, 7},
        {2, 0x101, false, false, 9},
    };
    for (const Case& c : cases) {
        std::memset(object, 0xee, sizeof object);
        calls.clear();
        frameNm = c.frame;
        syncRandState = seed;
        void* creaturePtr = c.hasCreature ? creature : nullptr;
        void* goPtr = c.hasGo ? go : nullptr;
        objectBase = object; creatureArg = creaturePtr; gameObjectArg = goPtr;
        __15SpellHateEffectiiP8CreatureP10GameObjectP5Skill(object, c.first, c.kind, creaturePtr, goPtr, skillBuffer);

        unsigned w4, w8, w12, w16, w20, w24, w28, w32, w36, w40, w44;
        std::memcpy(&w4, object + 4, 4); std::memcpy(&w8, object + 8, 4); std::memcpy(&w12, object + 12, 4);
        std::memcpy(&w16, object + 16, 4); std::memcpy(&w20, object + 20, 4); std::memcpy(&w24, object + 24, 4);
        std::memcpy(&w28, object + 28, 4); std::memcpy(&w32, object + 32, 4); std::memcpy(&w36, object + 36, 4);
        std::memcpy(&w40, object + 40, 4); std::memcpy(&w44, object + 44, 4);
        if (syncRandState != next || w16 != (next >> 16)) { std::puts("fail: random"); bad = 1; }
        if (object[0] != static_cast<unsigned char>(c.kind) || w4 != 0u || w8 != 0u) { std::puts("fail: base fields"); bad = 1; }
        if (w12 != low(creaturePtr) || w20 != low(vt_SpellHateEffect)) { std::puts("fail: creature or table"); bad = 1; }
        if (w24 != 0u || w28 != 0u) { std::puts("fail: list heads"); bad = 1; }
        if (w32 != static_cast<unsigned>(c.frame + c.first * 60) || static_cast<int>(w36) != c.first) { std::puts("fail: frame or first"); bad = 1; }
        if (static_cast<int>(w40) != c.kind || w44 != low(skillBuffer)) { std::puts("fail: kind or skill"); bad = 1; }
        std::string want;
        if (c.hasGo) want += "link(+24,G,G);";
        if (c.hasCreature) want += "link(+28,C,C);";
        if (calls != want) { std::printf("fail: calls '%s' want '%s'\n", calls.c_str(), want.c_str()); bad = 1; }
    }
    if (bad == 0) std::puts("ok");
    return bad;
}
