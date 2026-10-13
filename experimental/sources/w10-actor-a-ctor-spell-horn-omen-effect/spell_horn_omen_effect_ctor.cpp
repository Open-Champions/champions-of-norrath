// Observed __19SpellHornOmenEffectiP8CreatureP5Skill at 0x002ca2c8, 144 bytes.
// Constructor of the spell object: arguments are the object, a byte value, a Creature pointer
// and a Skill pointer. The base-class (Spell) constructor is inlined. Object layout as observed:
//   +0 byte    argument 2 (stored twice, before and after the table pointer changes)
//   +4 word    zero
//   +8 word    argument 4 (Skill pointer)
//   +12 word   argument 3 (Creature pointer)
//   +16 word   the high half of a step of the sync random state (state * 1664525 + 1013904223)
//   +20 word   table pointer: the Spell table first, then the SpellHornOmenEffect table
//   +24 word   frameNm + 600
//   +28 word   result of P_CreateParticleSystem()
// The state word syncRandState is read and written back. The original's return register holds the
// particle-system result, not the object, so no return value is claimed.
// Inferred: the field meanings from the argument types; +24 looks like an expiry frame.
// Unknown: whether the byte is a spell number or a flag. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_Spell[] __asm__("_vt$5Spell");
extern unsigned char vt_SpellHornOmenEffect[] __asm__("_vt$19SpellHornOmenEffect");
extern unsigned syncRandState;
extern int frameNm;
void* P_CreateParticleSystem__Fv();
}

struct SpellHornOmenEffectObject {
    unsigned char kind;
    unsigned char pad[3];
    int zero;
    void* skill;
    void* creature;
    unsigned random;
    const void* table;
    int expiryFrame;
    void* particles;
};

extern "C" void __19SpellHornOmenEffectiP8CreatureP5Skill(
    SpellHornOmenEffectObject* self, int kind, void* creature, void* skill) {
    self->kind = static_cast<unsigned char>(kind);
    self->table = vt_Spell;
    syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
    unsigned rnd = syncRandState >> 16;
    self->creature = creature;
    self->skill = skill;
    self->zero = 0;
    self->table = vt_SpellHornOmenEffect;
    self->kind = static_cast<unsigned char>(kind);
    self->creature = creature;
    self->skill = skill;
    self->random = rnd;
    void* ps = P_CreateParticleSystem__Fv();
    self->expiryFrame = frameNm + 600;
    self->particles = ps;
}

#if defined(__mips__)
static_assert(sizeof(SpellHornOmenEffectObject) == 32, "observed object extent is 32 bytes up to +28");
static_assert(__builtin_offsetof(SpellHornOmenEffectObject, particles) == 28, "particle-system word at +28");
#endif
