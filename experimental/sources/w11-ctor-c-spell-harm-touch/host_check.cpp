// Host contract for __14SpellHarmTouchiP8CreatureP5Skill; not an EE input.
#include "spell_harm_touch_ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_Spell[8], vt_SpellHarmTouch[8];
unsigned syncRandState;
static std::string calls; static const void* tableAtLump; static const SpellHarmTouchObject* watched;
static void* seenHeader; static void* seenObject; static float seenVolume; static int lumpCalls;
extern "C" void* lumpFindResource__FPcT0(char* lump, char* resource) {
    calls += "lump:"; calls += lump; calls += ":"; calls += resource; calls += ";"; ++lumpCalls;
    tableAtLump = watched->table;
    return reinterpret_cast<void*>(0x7007);
}
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) {
    calls += "play;"; seenHeader = h; seenObject = o; seenVolume = v;
}
int main() {
    int bad = 0;
    unsigned char raw[64];
    std::memset(raw, 0xee, sizeof raw);
    SpellHarmTouchObject* o = reinterpret_cast<SpellHarmTouchObject*>(raw);
    watched = o;
    syncRandState = 0x12345678u;
    __14SpellHarmTouchiP8CreatureP5Skill(o, 0x1a7, reinterpret_cast<void*>(0x30), reinterpret_cast<void*>(0x40));
    const unsigned next = 0x12345678u * 1664525u + 1013904223u;
    if (syncRandState != next) { std::puts("fail: random state step"); bad = 1; }
    if (o->random != (next >> 16)) { std::puts("fail: stored random is the high half"); bad = 1; }
    if (o->kind != 0xa7) { std::puts("fail: kind byte truncation"); bad = 1; }
    if (o->zero != 0 || o->creature != reinterpret_cast<void*>(0x30) || o->skill != reinterpret_cast<void*>(0x40)) { std::puts("fail: fields"); bad = 1; }
    if (o->scale != 0x3f29372au) { std::puts("fail: +24"); bad = 1; }
    if (o->table != vt_SpellHarmTouch) { std::puts("fail: final table"); bad = 1; }
    if (calls != "lump:fx.lmp:spel_de_harm.anm;play;" || lumpCalls != 1) { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; }
    if (seenHeader != reinterpret_cast<void*>(0x7007) || seenObject != reinterpret_cast<void*>(0x30) || seenVolume != 1.0f) { std::puts("fail: play arguments"); bad = 1; }
    for (unsigned i = sizeof(SpellHarmTouchObject); i < sizeof raw; ++i)
        if (raw[i] != 0xee) { std::puts("fail: bytes beyond the object written"); bad = 1; break; }
    return bad;
}
