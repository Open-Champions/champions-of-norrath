// Observed: NewPlayAttack__18VISpellEventDriverP12VISpellEvent at 0x0117d380 is 592 bytes in the
// pinned SLUS-20565 image. Arguments: (this, record). Result: -1 in one case, 0 in every other.
//   1. id = the int at this+24; -1 returns 0 at once. sprite = GetActorSpritePtr__C7VIScenei(the word
//      at this+20, id); null returns 0.
//   2. When the int at sprite+52 is not 5: stores -1 at this+68, +52, +56, +60 and +64 and returns -1.
//   3. By the int m at record+4: 0 and 1 store SetAttackAction__9VICSprite19VICSpriteAttachSlotR7VI
//      Scene(sprite, m, scene) at this+52; 2 and 3 store SetAttackAltAction__9VICSprite19VICSprite
//      AttachSlotR7VIScene(sprite, m - 2, scene) there; any other value calls nothing.
//   4. Builds a stack colour of four floats (record+20, record+24, record+28, 1.0f) and calls
//      SetWeaponTrail__9VICSpriteiR10VIColor32F(sprite, the int at record+8, &colour).
//   5. Stores -1 at this+56, +60, +64 and +68, hashes "Spell FX 1".."Spell FX 4" with VIHashResourceID,
//      calls FindTrigger__9VIHSpriteiRi(sprite, hash, &this[56 + 4 * n]) for each, and then for each
//      slot that is not -1 calls ResetTrigger__9VIHSpritei(sprite, the slot value). Returns 0.
// Inferred: record type 15 of the event table, the attack counterpart of NewSetAction: it starts the
// primary (0, 1) or alternate (2, 3) attack action of the actor's sprite, sets the weapon trail colour
// from three record floats (alpha 1.0) and resets the four named triggers like NewSetAction does.
// Unknown: what the attach slots 0 and 1 are, what the int at record+8 selects in SetWeaponTrail, what
// sprite+52 == 5 stands for, and whether the colour is red, green, blue in that order. Nothing here ran
// on the EE.
struct Color4 {
    float a, b, c, d;
};
extern "C" {
void* GetActorSpritePtr__C7VIScenei(void* scene, int actor);
int SetAttackAction__9VICSprite19VICSpriteAttachSlotR7VIScene(void* sprite, int slot, void* scene);
int SetAttackAltAction__9VICSprite19VICSpriteAttachSlotR7VIScene(void* sprite, int slot, void* scene);
void SetWeaponTrail__9VICSpriteiR10VIColor32F(void* sprite, int index, const Color4* color);
unsigned VIHashResourceID(const char* name);
void FindTrigger__9VIHSpriteiRi(void* sprite, unsigned hash, int* out);
void ResetTrigger__9VIHSpritei(void* sprite, int trigger);

int NewPlayAttack__18VISpellEventDriverP12VISpellEvent(unsigned char* self, unsigned char* record) {
    int* slots = reinterpret_cast<int*>(self);
    int id = slots[6];
    if (id == -1) {
        return 0;
    }
    unsigned char* sprite = static_cast<unsigned char*>(GetActorSpritePtr__C7VIScenei(*reinterpret_cast<void**>(self + 20), id));
    if (sprite == 0) {
        return 0;
    }
    if (*reinterpret_cast<int*>(sprite + 52) != 5) {
        slots[17] = -1;
        slots[13] = -1;
        slots[14] = -1;
        slots[15] = -1;
        slots[16] = -1;
        return -1;
    }
    int mode = *reinterpret_cast<int*>(record + 4);
    if (mode == 1) {
        slots[13] = SetAttackAction__9VICSprite19VICSpriteAttachSlotR7VIScene(sprite, 1, *reinterpret_cast<void**>(self + 20));
    } else if (mode == 0) {
        slots[13] = SetAttackAction__9VICSprite19VICSpriteAttachSlotR7VIScene(sprite, 0, *reinterpret_cast<void**>(self + 20));
    } else if (mode == 2) {
        slots[13] = SetAttackAltAction__9VICSprite19VICSpriteAttachSlotR7VIScene(sprite, 0, *reinterpret_cast<void**>(self + 20));
    } else if (mode == 3) {
        slots[13] = SetAttackAltAction__9VICSprite19VICSpriteAttachSlotR7VIScene(sprite, 1, *reinterpret_cast<void**>(self + 20));
    }
    Color4 color;
    color.a = *reinterpret_cast<float*>(record + 20);
    color.b = *reinterpret_cast<float*>(record + 24);
    color.c = *reinterpret_cast<float*>(record + 28);
    color.d = 1.0f;
    SetWeaponTrail__9VICSpriteiR10VIColor32F(sprite, *reinterpret_cast<int*>(record + 8), &color);
    slots[14] = -1;
    slots[15] = -1;
    slots[16] = -1;
    slots[17] = -1;
    FindTrigger__9VIHSpriteiRi(sprite, VIHashResourceID("Spell FX 1"), &slots[14]);
    FindTrigger__9VIHSpriteiRi(sprite, VIHashResourceID("Spell FX 2"), &slots[15]);
    FindTrigger__9VIHSpriteiRi(sprite, VIHashResourceID("Spell FX 3"), &slots[16]);
    FindTrigger__9VIHSpriteiRi(sprite, VIHashResourceID("Spell FX 4"), &slots[17]);
    if (slots[14] != -1) {
        ResetTrigger__9VIHSpritei(sprite, slots[14]);
    }
    if (slots[15] != -1) {
        ResetTrigger__9VIHSpritei(sprite, slots[15]);
    }
    if (slots[16] != -1) {
        ResetTrigger__9VIHSpritei(sprite, slots[16]);
    }
    if (slots[17] != -1) {
        ResetTrigger__9VIHSpritei(sprite, slots[17]);
    }
    return 0;
}
}
