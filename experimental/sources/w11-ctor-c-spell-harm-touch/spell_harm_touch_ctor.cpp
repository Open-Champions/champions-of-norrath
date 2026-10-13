// Observed __14SpellHarmTouchiP8CreatureP5Skill at 0x0029b0d0, 160 bytes. Constructor of the spell
// object: arguments are the object, an int whose low byte is stored (K), a Creature pointer C and a
// Skill pointer S. The Spell base constructor is inlined; the object layout as in the horn-omen
// effect spell (see w10-actor-a-ctor-spell-horn-omen-effect):
//   +0 byte K, +4 word 0, +8 word S, +12 word C,
//   +16 word  the high half of a step of the sync random state (state * 1664525 + 1013904223),
//   +20 word  the Spell table first, then the _vt$14SpellHarmTouch table,
//   +24 float 0x3f29372a (about 0.661).
// Then lumpFindResource__FPcT0 is called with the two texts "fx.lmp" and "spel_de_harm.anm", and its
// result is passed to soundPlaySequence__FP15AnimationHeaderP10GameObjectf with C and 1.0f. That is a
// tail call in the image, so the object word +20 is written in the delay slot of the first call.
// Nothing is returned (the image returns whatever the sound call returns, which no caller is known
// to read). Inferred: the resource is an animation/sound sequence played on the creature.
// Unknown: what +24 is used for. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_Spell[] __asm__("_vt$5Spell");
extern unsigned char vt_SpellHarmTouch[] __asm__("_vt$14SpellHarmTouch");
extern unsigned syncRandState;
void* lumpFindResource__FPcT0(char* lump, char* resource);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
}

struct SpellHarmTouchObject {
    unsigned char kind;
    unsigned char pad[3];
    int zero;
    void* skill;
    void* creature;
    unsigned random;
    const void* table;
    unsigned scale;
};

extern "C" void __14SpellHarmTouchiP8CreatureP5Skill(SpellHarmTouchObject* self, int kind, void* creature, void* skill) {
    self->kind = static_cast<unsigned char>(kind);
    self->table = vt_Spell;
    syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
    const unsigned rnd = syncRandState >> 16;
    self->skill = skill;
    self->creature = creature;
    self->zero = 0;
    self->random = rnd;
    self->scale = 0x3f29372au;
    self->table = vt_SpellHarmTouch;
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
        lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_de_harm.anm")), creature, 1.0f);
}

#if defined(__mips__)
static_assert(__builtin_offsetof(SpellHarmTouchObject, scale) == 24, "float word at +24");
#endif
