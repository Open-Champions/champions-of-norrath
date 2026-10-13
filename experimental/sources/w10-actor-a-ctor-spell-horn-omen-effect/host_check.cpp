// Host contract for __19SpellHornOmenEffectiP8CreatureP5Skill; not an EE input.
// Checks field values through the struct (pointers are wider on the host, so offsets are only
// asserted in the EE build). The original's byte offsets are compared in a private R5900 run.
#include "spell_horn_omen_effect_ctor.cpp"
#include <cstdio>
#include <cstring>

unsigned char vt_Spell[48];
unsigned char vt_SpellHornOmenEffect[48];
unsigned syncRandState;
int frameNm;
static const SpellHornOmenEffectObject* watched;
static unsigned char seenKind;
static const void* seenTable;
extern "C" void* P_CreateParticleSystem__Fv() {
    seenKind = watched->kind; seenTable = watched->table;  // state when the particle system is created
    return reinterpret_cast<void*>(0x1234);
}
int main() {
    int bad = 0;
    SpellHornOmenEffectObject o;
    std::memset(&o, 0xee, sizeof o);
    syncRandState = 0x12345678u; frameNm = 100;
    watched = &o;
    __19SpellHornOmenEffectiP8CreatureP5Skill(&o, 0x1a7, reinterpret_cast<void*>(0x30), reinterpret_cast<void*>(0x40));
    unsigned next = 0x12345678u * 1664525u + 1013904223u;
    if (syncRandState != next) { std::puts("fail: random state step"); bad = 1; }
    if (o.random != (next >> 16)) { std::puts("fail: stored random is high half"); bad = 1; }
    if (o.kind != 0xa7) { std::puts("fail: kind byte truncation"); bad = 1; }
    if (o.zero != 0 || o.creature != reinterpret_cast<void*>(0x30) || o.skill != reinterpret_cast<void*>(0x40)) { std::puts("fail: fields"); bad = 1; }
    if (o.table != vt_SpellHornOmenEffect || seenTable != vt_SpellHornOmenEffect || seenKind != 0xa7) { std::puts("fail: derived table / state before particle call"); bad = 1; }
    if (o.expiryFrame != 700 || o.particles != reinterpret_cast<void*>(0x1234)) { std::puts("fail: expiry or particle word"); bad = 1; }
    return bad;
}
