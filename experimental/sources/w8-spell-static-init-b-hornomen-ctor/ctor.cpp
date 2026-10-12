// __19SpellHornOmenEffectiP8CreatureP5Skill at 0x2ca2c8, 144 bytes, read from the
// pinned SLUS-20565 image. Nothing here has run on the EE.
//
// Observed, in the order the image does it: store the low byte of the integer
// argument at offset 0, the second pointer argument at 12, the third at 8 and
// zero at 4. Advance the 32-bit multiplicative generator held in syncRandState
// (multiply by 0x19660d, add 0x3c6ef35f) and keep its upper 16 bits at 16. The
// vtable word at 20 is first the address of `_vt$5Spell` and then that of
// `_vt$19SpellHornOmenEffect`; the compiler used here folds the first store away.
// Then call P_CreateParticleSystem and keep its result at 28, and store the frame
// counter frameNm plus 600 at 24.
// Inferred: the first half is an inlined base-class construction (Spell), because
// the base vtable is written before the derived one. The first argument is named
// rank by its only caller; that name is not used here.
// Unknown: what the byte, the two stored pointers and the particle system mean.
extern "C" {
extern unsigned syncRandState;
extern int frameNm;
extern unsigned char _vt$5Spell[];
extern unsigned char _vt$19SpellHornOmenEffect[];
void* P_CreateParticleSystem__Fv();

struct norrath_horn_omen_effect {
    unsigned char first_byte;
    unsigned char padding[3];
    unsigned zero;
    void* third_argument;
    void* second_argument;
    unsigned random_upper;
    const void* vtable;
    int frame_plus_600;
    void* particle_system;
};

void __19SpellHornOmenEffectiP8CreatureP5Skill(norrath_horn_omen_effect* self, int first, void* second, void* third) {
    self->first_byte = static_cast<unsigned char>(first);
    self->vtable = _vt$5Spell;
    self->second_argument = second;
    self->third_argument = third;
    self->zero = 0;
    const unsigned next = syncRandState * 0x19660du + 0x3c6ef35fu;
    syncRandState = next;
    self->vtable = _vt$19SpellHornOmenEffect;
    self->first_byte = static_cast<unsigned char>(first);
    self->second_argument = second;
    self->third_argument = third;
    self->random_upper = next >> 16;
    self->particle_system = P_CreateParticleSystem__Fv();
    self->frame_plus_600 = frameNm + 600;
}
}
