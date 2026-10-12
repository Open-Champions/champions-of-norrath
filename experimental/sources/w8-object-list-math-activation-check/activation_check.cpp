// Observed objectActivationCheck__8Creaturei at 0x10a428, extent 228.
// Field offsets used: +12 a word of object flags, +160 a 64-bit word, +236 an
// active flag, +240 a copy of the previous active flag, +288 the creature's
// animation state. In order:
//   1. previous = active; the copy at +240 takes previous.
//   2. active = objectActiveProcessing(creature, previous, argument) != 0, where
//      the argument is 0xabe0 when bit 28 (0x10000000) of the 64-bit word at +160
//      is set and otherwise the function's own second argument.
//   3. If the flag word at +12 has any bit of 0x00c00000 and the active flag
//      changed: look up a tag string on the creature with findTagString (offset 0)
//      -- "followFoundScript" when it became active, "followLostScript" when it
//      stopped -- and start the script named by the result when one exists.
//   4. When the creature is not active, animResetFramestates on its animation
//      state. Returns the active flag, read again after those calls.
// Unknown: what the 0xabe0 value and the bit-28 word mean, and what the
// follow scripts do.
struct GameObject;

struct Creature {
    unsigned char before_flags[12];
    unsigned object_flags;
    unsigned char before_state[160 - 16];
    unsigned long long state_bits;
    unsigned char before_active[236 - 168];
    int active;
    int previous_active;
    unsigned char before_animation[288 - 244];
    unsigned char animation_state[1];
};

#if defined(__mips__)
static_assert(__builtin_offsetof(Creature, object_flags) == 12 && __builtin_offsetof(Creature, state_bits) == 160 &&
                  __builtin_offsetof(Creature, active) == 236 && __builtin_offsetof(Creature, previous_active) == 240 &&
                  __builtin_offsetof(Creature, animation_state) == 288, "observed offsets");
#endif

extern "C" int objectActiveProcessing__FP10GameObjectii(GameObject* object, int previous, int argument);
extern "C" char* findTagString__10GameObjectPci(GameObject* object, const char* tag, int index);
extern "C" void scriptStart__FPc(char* script);
extern "C" void animResetFramestates__FP14AnimationState(void* state);

extern "C" int objectActivationCheck__8Creaturei(Creature* creature, int argument) {
    const int previous = creature->active;
    creature->previous_active = previous;
    const int processing_argument = (creature->state_bits & 0x10000000ull) != 0 ? 0xabe0 : argument;
    creature->active = objectActiveProcessing__FP10GameObjectii(reinterpret_cast<GameObject*>(creature), previous, processing_argument) != 0;
    if ((creature->object_flags & 0x00c00000u) != 0 && previous != creature->active) {
        const char* tag = creature->active != 0 ? "followFoundScript" : "followLostScript";
        char* script = findTagString__10GameObjectPci(reinterpret_cast<GameObject*>(creature), tag, 0);
        if (script) scriptStart__FPc(script);
    }
    if (creature->active == 0) animResetFramestates__FP14AnimationState(creature->animation_state);
    return creature->active;
}
