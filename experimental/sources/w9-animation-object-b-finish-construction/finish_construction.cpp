// Observed finishConstruction__8Creature at 0x1091b0, extent 316.
// Last step of a creature constructor. Words used on the creature: +32 a float
// (set below), +41 a byte, +72 the virtual table (entries of 8 bytes: a halfword
// delta added to the object address, then a function pointer four bytes on),
// +160 a 64-bit flag word, +288 the animation state. In order:
//   1. If bit 0 of the 64-bit word at +160 is set, call the table entry at +144
//      (no argument besides the adjusted object).
//   2. Call the entry at +128. If it returns nonzero it is called a second time and
//      that result (not the first) is the model passed to the bounding-box query
//      with index -1, which fills six integers. The float at +32 becomes
//      (float)(sixth - third) and is raised to 60.0f when it is below that; the
//      byte at +41 becomes (int)(float at +32) + 1.
//   3. findTagString(creature, "dead", 0). When it finds something: bit 32 of the
//      +160 word is set, the entry at +184 is called with the argument 10, and the
//      animation state at +288 is set to the end of its blend
//      (animSetToEndFullyBlended). Otherwise nothing more happens.
// Returns what the end-of-blend call returns, or zero when the tag is absent; the
// type of that result is not known.
// Inferred: the +128 entry is the model getter; +144 initializes a boss flavor.
// Unknown: what the +184 entry does with 10, and what the +32 / +41 fields mean.
// norrath_creature_finish_construction is the name the constructor sources import;
// it is defined as a second symbol for the same code. The earlier authored
// norrath_actor_finish_construction implements the same original routine.
struct VirtualEntry {
    short delta;
    short reserved;
    void* function;
};
struct FinishedCreature {
    unsigned char before_height[32];
    float height;
    unsigned char before_byte[41 - 36];
    unsigned char height_byte;
    unsigned char before_vtable[72 - 42];
    const VirtualEntry* vtable;
    unsigned char before_flags[160 - 76];
    unsigned long long flags;
    unsigned char before_animation[288 - 168];
    unsigned char animation_state[1];
};
#if defined(__mips__)
static_assert(__builtin_offsetof(FinishedCreature, height) == 32 && __builtin_offsetof(FinishedCreature, height_byte) == 41 &&
                  __builtin_offsetof(FinishedCreature, vtable) == 72 && __builtin_offsetof(FinishedCreature, flags) == 160 &&
                  __builtin_offsetof(FinishedCreature, animation_state) == 288 && sizeof(VirtualEntry) == 8,
              "observed offsets");
#endif

extern "C" {
char* findTagString__10GameObjectPci(void* object, char* tag, int index);
void norrath_model_bounding_box(void* model, int index, int* a, int* b, int* c, int* d, int* e, int* f);
unsigned norrath_animation_set_to_end_fully_blended(void* state);
}

namespace {
template <typename Function>
Function entry_function(FinishedCreature* creature, unsigned offset, void*& self) {
    const VirtualEntry* entry = reinterpret_cast<const VirtualEntry*>(reinterpret_cast<const char*>(creature->vtable) + offset);
    self = reinterpret_cast<char*>(creature) + entry->delta;
    return reinterpret_cast<Function>(entry->function);
}
}

extern "C" unsigned finishConstruction__8Creature(FinishedCreature* creature) {
    void* self;
    if ((creature->flags & 1u) != 0) entry_function<void (*)(void*)>(creature, 144, self)(self);
    if (entry_function<void* (*)(void*)>(creature, 128, self)(self) != nullptr) {
        void* model = entry_function<void* (*)(void*)>(creature, 128, self)(self);
        int bounds[6];
        norrath_model_bounding_box(model, -1, &bounds[0], &bounds[1], &bounds[2], &bounds[3], &bounds[4], &bounds[5]);
        creature->height = static_cast<float>(bounds[5] - bounds[2]);
        if (creature->height < 60.0f) creature->height = 60.0f;
        creature->height_byte = static_cast<unsigned char>(static_cast<int>(creature->height) + 1);
    }
    static char dead_tag[] = "dead";
    if (findTagString__10GameObjectPci(creature, dead_tag, 0) == nullptr) return 0;
    creature->flags |= 0x100000000ull;
    entry_function<void (*)(void*, int)>(creature, 184, self)(self, 10);
    return norrath_animation_set_to_end_fully_blended(creature->animation_state);
}

#if defined(__mips__)
extern "C" unsigned norrath_creature_finish_construction(void* object) __attribute__((alias("finishConstruction__8Creature")));
#endif
