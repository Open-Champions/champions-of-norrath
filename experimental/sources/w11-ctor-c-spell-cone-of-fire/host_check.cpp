// Host contract for __15SpellConeOfFireiP8CreatureP5Skill; not an EE input.
#include "spell_cone_of_fire_ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_Spell[8], vt_SpellConeOfFire[8];
unsigned syncRandState;
static std::string calls; static unsigned char* theObject;
static unsigned char lightAtAdd[48]; static Point3 playPos; static void* playHeader; static float playVolume;
static int persisted = -1; static void* addedLight;
extern "C" void lightAdd__FP5Light(void* l) {
    calls += "light;"; addedLight = l; std::memcpy(lightAtAdd, l, 48);
}
extern "C" void* lumpFindResource__FPcT0(char* lump, char* resource) {
    calls += "lump:"; calls += lump; calls += ":"; calls += resource; calls += ";";
    return reinterpret_cast<void*>(0x7007);
}
extern "C" int soundPlaySequence__FP15AnimationHeaderG6Point3f(void* h, Point3 p, float v) {
    calls += "play;"; playHeader = h; playPos = p; playVolume = v; return 0x4242;
}
extern "C" void soundMakePersistant__Fi(int h) { calls += "persist;"; persisted = h; }
int main() {
    int bad = 0;
    alignas(16) unsigned char raw[128];
    std::memset(raw, 0xee, sizeof raw);
    theObject = raw;
    alignas(16) unsigned char creature[64];
    std::memset(creature, 0, sizeof creature);
    const Point3 where{1.5f, -2.5f, 3.25f};
    std::memcpy(creature + 16, &where, 12);
    syncRandState = 0x12345678u;
    __15SpellConeOfFireiP8CreatureP5Skill(raw, 0x1a7, creature, reinterpret_cast<void*>(0x40));
    const unsigned next = 0x12345678u * 1664525u + 1013904223u;
    unsigned w4, w8, w16, w20, w84; std::memcpy(&w4, raw + 4, 4); std::memcpy(&w8, raw + 8, 4);
    std::memcpy(&w16, raw + 16, 4); std::memcpy(&w20, raw + 20, 4); std::memcpy(&w84, raw + 84, 4);
    if (syncRandState != next || w16 != (next >> 16)) { std::puts("fail: random"); bad = 1; }
    if (raw[0] != 0xa7 || w4 != 0u || w8 != 0x40u) { std::puts("fail: base fields"); bad = 1; }
    if (w20 != low(vt_SpellConeOfFire)) { std::puts("fail: table"); bad = 1; }
    volatile float vz = 0.0f;
    const float inf = 1.0f / vz;
    unsigned char want[48]; std::memset(want, 0xee, sizeof want);
    want[8] = 0; want[9] = 0;
    const short h[] = {255, 125, 0, 66, 255, 125, 0, 66};
    for (int i = 0; i < 8; ++i) std::memcpy(want + 10 + 2 * i, &h[i], 2);
    const float f0 = 0.0f; const unsigned f44 = 0x3f19db22u;
    std::memcpy(want + 28, &f0, 4); std::memcpy(want + 32, &inf, 4); std::memcpy(want + 36, &f0, 4);
    std::memcpy(want + 40, &inf, 4); std::memcpy(want + 44, &f44, 4);
    // bytes 0..7 and 26..27 are not written by the constructor
    if (addedLight != raw + 24) { std::puts("fail: light address"); bad = 1; }
    for (int i = 0; i < 48; ++i) {
        if (i < 8 || i == 26 || i == 27) { if (lightAtAdd[i] != 0xee) { std::printf("fail: light byte %d touched\n", i); bad = 1; } }
        else if (lightAtAdd[i] != want[i]) { std::printf("fail: light byte %d\n", i); bad = 1; }
    }
    Point3 p72; std::memcpy(&p72, raw + 72, 12);
    if (p72.x != where.x || p72.y != where.y || p72.z != where.z) { std::puts("fail: point copy at +72"); bad = 1; }
    if (calls != "light;lump:fx.lmp:spel_ew_cfire.anm;play;persist;") { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; }
    if (playPos.x != where.x || playPos.y != where.y || playPos.z != where.z) { std::puts("fail: play position"); bad = 1; }
    if (playHeader != reinterpret_cast<void*>(0x7007) || playVolume != 1.0f) { std::puts("fail: play arguments"); bad = 1; }
    if (w84 != 0x4242u || persisted != 0x4242) { std::puts("fail: handle"); bad = 1; }
    for (int i = 88; i < 128; ++i) if (raw[i] != 0xee) { std::puts("fail: tail written"); bad = 1; break; }
    return bad;
}
