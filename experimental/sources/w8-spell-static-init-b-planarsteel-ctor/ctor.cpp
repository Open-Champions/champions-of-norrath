// __22SpellPlanarSteelEffectiP8CreatureP5Skill at 0x2c8bc0, 488 bytes, read from
// the pinned SLUS-20565 image. Nothing here has run on the EE.
//
// Observed, in the image's order. The first part is the same inlined base
// construction as in the HornOmen constructor: low byte of the integer argument
// at 0, zero at 4, third argument at 8, second argument at 12, the generator in
// syncRandState advanced (times 0x19660d plus 0x3c6ef35f) with its upper 16 bits
// at 16, vtable word at 20 (`_vt$5Spell`, then `_vt$22SpellPlanarSteelEffect`).
// Then a 48-byte light record at offset 48 is filled: bytes 8 and 9 zero; halfwords
// at 10, 12 and 18 are 75, at 14 is 200, at 16 is 255, at 20 is 75, at 22 is 200,
// at 24 is 255; floats at 28 and 36 are zero, at 32 and 40 the result of dividing
// 1.0 by 0.0 (the image divides at run time; the EE has no infinity, so the value is
// the largest float, bits 0x7fffffff, which the compiler also produces when it folds the division), and at 44 the .sdata float with bits 0x3f266666. The creature argument's position (12 bytes at creature+16) is
// copied to offset 96, and the light address is passed to lightAdd__FP5Light.
// After that the byte, skill and creature words are stored again; offset 36 gets
// frameNm plus 240, offset 32 gets zero, the halfword at 28 gets the float at
// creature+32 times 3.0 converted to an integer, the halfword at 26 gets 32767
// and the one at 24 gets zero. Finally the animation "spel_gatescroll.anm" is
// looked up in "fx.lmp" with lumpFindResource__FPcT0 and played at the creature's
// position (copied again) with volume 1.0 through
// soundPlaySequence__FP15AnimationHeaderG6Point3f.
// Inferred: the first part is an inlined Spell base construction; the 48-byte record
// is a Light because its address goes to lightAdd.
// Unknown: what the individual light fields and the halfword scale mean here.
extern "C" {
extern unsigned syncRandState;
extern int frameNm;
extern unsigned char _vt$5Spell[];
extern unsigned char _vt$22SpellPlanarSteelEffect[];
void lightAdd__FP5Light(void* light);
void* lumpFindResource__FPcT0(const char* lump, const char* name);
void soundPlaySequence__FP15AnimationHeaderG6Point3f(void* header, const float* point, float volume);

struct norrath_planar_light {
    unsigned char padding0[8];
    unsigned char byte8;
    unsigned char byte9;
    unsigned short half10;
    unsigned short half12;
    unsigned short half14;
    unsigned short half16;
    unsigned short half18;
    unsigned short half20;
    unsigned short half22;
    unsigned short half24;
    unsigned char padding26[2];
    float word28;
    float word32;
    float word36;
    float word40;
    float word44;
};

struct norrath_planar_creature_view {
    unsigned char padding0[16];
    float position[3];
    float word28;
    float word32;
};

struct norrath_planar_steel_effect {
    unsigned char first_byte;
    unsigned char padding1[3];
    unsigned zero;
    void* skill;
    norrath_planar_creature_view* creature;
    unsigned random_upper;
    const void* vtable;
    short half24;
    short half26;
    short half28;
    unsigned char padding30[2];
    unsigned word32;
    int frame_plus_240;
    unsigned char padding40[8];
    norrath_planar_light light;
    float position[3];
};

void __22SpellPlanarSteelEffectiP8CreatureP5Skill(norrath_planar_steel_effect* self, int first,
                                                   norrath_planar_creature_view* creature, void* skill) {
    self->first_byte = static_cast<unsigned char>(first);
    self->vtable = _vt$5Spell;
    self->zero = 0;
    self->creature = creature;
    self->skill = skill;
    const unsigned next = syncRandState * 0x19660du + 0x3c6ef35fu;
    syncRandState = next;
    self->vtable = _vt$22SpellPlanarSteelEffect;
    self->random_upper = next >> 16;

    const float one = 1.0f;
    const float zero = 0.0f;
    const float quotient = one / zero;
    self->light.byte9 = 0;
    self->light.word32 = quotient;
    self->light.word40 = quotient;
    self->light.word44 = 0.6499999761581421f;
    self->light.half20 = 75;
    self->light.half22 = 200;
    self->light.half24 = 255;
    self->light.half10 = 75;
    self->light.half12 = 75;
    self->light.half14 = 200;
    self->light.half16 = 255;
    self->light.half18 = 75;
    self->light.word28 = zero;
    self->light.word36 = zero;
    self->position[0] = creature->position[0];
    self->position[1] = creature->position[1];
    self->position[2] = creature->position[2];
    self->light.byte8 = 0;
    lightAdd__FP5Light(&self->light);

    self->first_byte = static_cast<unsigned char>(first);
    self->skill = skill;
    self->creature = creature;
    self->frame_plus_240 = frameNm + 240;
    self->word32 = 0;
    self->half28 = static_cast<short>(static_cast<int>(creature->word32 * 3.0f));
    self->half26 = 32767;
    self->half24 = 0;

    void* header = lumpFindResource__FPcT0("fx.lmp", "spel_gatescroll.anm");
    float point[3] = {creature->position[0], creature->position[1], creature->position[2]};
    soundPlaySequence__FP15AnimationHeaderG6Point3f(header, point, 1.0f);
}
}
