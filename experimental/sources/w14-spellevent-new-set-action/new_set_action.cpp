// Observed: NewSetAction__18VISpellEventDriverP12VISpellEvent at 0x0117d1e8 is 408 bytes in the
// pinned SLUS-20565 image. Arguments: (this, record). Result: -1 in one case, 0 in every other.
//   1. id = the int at this+24. If it is -1 the function returns 0 and does nothing else.
//   2. sprite = GetActorSpritePtr__C7VIScenei(the word at this+20, id). If it is null: return 0.
//   3. If the int at sprite+52 is not 5: stores -1 at this+68, +52, +56, +60 and +64 and returns -1.
//   4. Otherwise r = SetAction__9VICSprite15VICSpriteAnimIDR7VIScene(sprite, the int at record+4, the
//      word at this+20); stores -1 at this+56, r at this+52, and -1 at this+60, +64 and +68. Then it
//      hashes "Spell FX 1", "Spell FX 2", "Spell FX 3" and "Spell FX 4" with VIHashResourceID and for
//      each calls FindTrigger__9VIHSpriteiRi(sprite, hash, &this[56 + 4 * n]) (n = 0..3). Finally, for
//      the slots at +56, +60, +64 and +68 in that order, when the int is not -1 it calls
//      ResetTrigger__9VIHSpritei(sprite, that value). Returns 0.
// Inferred: record type 2 of the event table. It sets the animation of the actor's sprite and looks up
// four named triggers ("Spell FX 1".."4") whose slots the driver keeps in +56..+68; a found trigger is
// reset, so it can fire again when the spell plays. The int at sprite+52 being 5 is the condition for
// a sprite that supports actions.
// Unknown: what sprite+52 == 5 stands for, what +52 holds (the action result), the VICSpriteAnimID
// type, and whether FindTrigger writes its slot when the trigger is missing. Nothing here ran on the EE.
extern "C" {
void* GetActorSpritePtr__C7VIScenei(void* scene, int actor);
int SetAction__9VICSprite15VICSpriteAnimIDR7VIScene(void* sprite, int anim, void* scene);
unsigned VIHashResourceID(const char* name);
void FindTrigger__9VIHSpriteiRi(void* sprite, unsigned hash, int* out);
void ResetTrigger__9VIHSpritei(void* sprite, int trigger);

int NewSetAction__18VISpellEventDriverP12VISpellEvent(unsigned char* self, unsigned char* record) {
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
    int action = SetAction__9VICSprite15VICSpriteAnimIDR7VIScene(sprite, *reinterpret_cast<int*>(record + 4), *reinterpret_cast<void**>(self + 20));
    slots[14] = -1;
    slots[13] = action;
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
