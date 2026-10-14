// Observed cutStop__Fv at 0x0018ce88, 1224 bytes. Nothing here was executed on
// the EE. Offsets are bytes; "word" means a 32-bit value.
// 1. soundResetStream(2). When bit 0x100 of soundGetStreamPlayStatus(0) is
//    clear: if bit 0x80 of a second soundGetStreamPlayStatus(0) is set,
//    soundResetStream(0), else soundClearStreamQue(0).
// 2. engineScissory2 = 447; cutMode = 0; thereIsACutPlayer = 0; cutSkipMode =
//    0; engineScissory1 = 0; then engineRemoveTask for cutRunFrameTask__Fv and
//    for cutDrawFrame__Fv; then objectCancelAllAlwaysDraw__Fv().
// 3. For each of nmActors records of 44 bytes in the array pointed to by
//    `actor` (record: +0 object pointer, +8 name text, byte +43 a flag):
//    - When the name equals "Player" (the literal at 0x004843c0) and the
//      object pointer is nonzero: if player[0] (the 4-record table `player`,
//      0x00408600, 23984 bytes per record, object pointer at +21872, a float at
//      +172) has a nonzero object pointer and a float above 0.0, the 12 bytes
//      at +16 of the actor's object are copied to +16 of that object, the
//      halfword at +56 is copied from the actor's object to it, and
//      objectUpdateInGrid is called on it. Then for each j from 1 to
//      nmPlayers - 1 (for a nonzero actor object pointer, a nonzero player
//      object pointer and a player float above 0.0): the 12 bytes at +16 are
//      copied, a spot is found with objectFindEmptySpot(that position, 36, 14,
//      null), the position at +16 becomes the spot, the halfword at +56 becomes
//      the actor object's halfword plus 8000 * j, worldFindHeight(world, the
//      new position, 12.0) is stored at +24 (before objectUpdateInGrid is
//      called on the player object).
//    - Then, for every actor: when the flag byte is nonzero and the object
//      pointer is nonzero, a virtual call is made through the table at object
//      +72: the entry at table + 8 holds a halfword adjustment (offset 0) and
//      a function pointer (offset 4); the function is called with the object
//      plus the adjustment and the argument 3. When the flag byte is zero and
//      the object pointer is nonzero, the word at object +12 is stored with
//      bits 0x00088000 cleared, and if bit 0x20 of the old value was set, bit
//      0 of the byte at object +570 is cleared.
// 4. When frontEndRunning is zero, for each of nmPlayers entries of `player`
//    whose object pointer is nonzero and whose float at +172 is above 0.0:
//    animRemoveNonpermanentAnimations(object + 288).
// 5. worldSetFarClip(1800.0); worldSetFrustum(50.0, 1.33, 80.0, 1.0) (the second
//    value is the file word 0x3faa3d70); lightGlobalCharLightScale(1.0).
// 6. When bit 8 of `flags` is set: objectIterateRunLists with
//    unremoveMonster__FP10GameObject; otherwise wakeMonsters__8creaturev().
//    Then, when bit 0x10 of `flags` is set, engineScreenSlide = 0 (the word is
//    rewritten unchanged otherwise).
// 7. For the two 8-byte slots of anim (0x00790f60): when the first word is
//    nonzero, animationLoading is cleared and then cdWait__Fv is called if it
//    was set,
//    memFree__FPv is called on the block and both words are zeroed.
// 8. When dialogActor is nonzero: the doubleword at dialogActor + 160 has bit
//    28 cleared and dialogActor becomes null.
// Nothing is returned. What the actor records and the flags mean is not
// established; the name "Player" is only the string compared.
extern "C" {
void soundResetStream__Fi(int stream);
int soundGetStreamPlayStatus__Fi(int stream);
void soundClearStreamQue__Fi(int stream);
void engineRemoveTask__FPFv_v(void (*task)());
void cutRunFrameTask__Fv();
void cutDrawFrame__Fv();
void objectCancelAllAlwaysDraw__Fv();
int strcmp(const char* a, const char* b);
void objectUpdateInGrid__FP10GameObject(void* object);
void objectFindEmptySpot__FG6Point3iiPc(float* result, const float* position, int first, int second, char* name);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, const float* position, float extra);
void animRemoveNonpermanentAnimations__FP14AnimationState(void* state);
void worldSetFarClip__Ff(float distance);
void worldSetFrustum__Fffff(float first, float second, float third, float fourth);
void lightGlobalCharLightScale__Ff(float scale);
void objectIterateRunLists__FPFP10GameObject_v(void (*callback)(void*));
void unremoveMonster__FP10GameObject(void* object);
void wakeMonsters__8creaturev();
void cdWait__Fv();
void memFree__FPv(void* block);
extern int engineScissory1;
extern int engineScissory2;
extern int engineScreenSlide;
extern unsigned char cutMode;
extern unsigned char cutSkipMode;
extern unsigned char thereIsACutPlayer;
extern unsigned char animationLoading;
extern unsigned char* actor;
extern int nmActors;
extern unsigned char player[];
extern int nmPlayers;
extern void* world;
extern int frontEndRunning;
extern int flags;
extern unsigned char* dialogActor;
struct CutAnimSlot { void* block; int second; };
extern CutAnimSlot anim[2];
}

typedef float __attribute__((may_alias)) AliasedFloat;
typedef unsigned short __attribute__((may_alias)) AliasedUShort;
typedef unsigned __attribute__((may_alias)) AliasedUnsigned;

struct VirtualEntry {
    short adjust;
    short unused;
    void (*function)(void* self, int argument);
};

#if defined(__mips__)
static_assert(__builtin_offsetof(VirtualEntry, function) == 4, "function pointer at +4");
#endif

static inline unsigned char* objectOf(const unsigned char* record) {
    return *reinterpret_cast<unsigned char* const*>(record + 21872);
}
static inline float levelOf(const unsigned char* record) {
    return *reinterpret_cast<const AliasedFloat*>(record + 172);
}
static inline void copyPosition(unsigned char* to, const unsigned char* from) {
    *reinterpret_cast<AliasedFloat*>(to + 16) = *reinterpret_cast<const AliasedFloat*>(from + 16);
    *reinterpret_cast<AliasedFloat*>(to + 20) = *reinterpret_cast<const AliasedFloat*>(from + 20);
    *reinterpret_cast<AliasedFloat*>(to + 24) = *reinterpret_cast<const AliasedFloat*>(from + 24);
}

extern "C" void cutStop__Fv() {
    soundResetStream__Fi(2);
    if ((soundGetStreamPlayStatus__Fi(0) & 0x100) == 0) {
        if ((soundGetStreamPlayStatus__Fi(0) & 0x80) != 0) soundResetStream__Fi(0);
        else soundClearStreamQue__Fi(0);
    }
    engineScissory2 = 447;
    cutMode = 0;
    thereIsACutPlayer = 0;
    cutSkipMode = 0;
    engineScissory1 = 0;
    engineRemoveTask__FPFv_v(cutRunFrameTask__Fv);
    engineRemoveTask__FPFv_v(cutDrawFrame__Fv);
    objectCancelAllAlwaysDraw__Fv();

    for (int i = 0; i < nmActors; ++i) {
        unsigned char* record = actor + 44 * i;
        if (strcmp(reinterpret_cast<const char*>(record + 8), "Player") == 0) {
            if (*reinterpret_cast<unsigned char**>(record) != nullptr) {
                unsigned char* first = objectOf(player);
                if (first != nullptr && levelOf(player) > 0.0f) {
                    unsigned char* source = *reinterpret_cast<unsigned char**>(actor + 44 * i);
                    copyPosition(first, source);
                    unsigned char* moved = objectOf(player);
                    source = *reinterpret_cast<unsigned char**>(actor + 44 * i);
                    *reinterpret_cast<AliasedUShort*>(moved + 56) = *reinterpret_cast<AliasedUShort*>(source + 56);
                    objectUpdateInGrid__FP10GameObject(moved);
                }
            }
            for (int j = 1; j < nmPlayers; ++j) {
                unsigned char* source = *reinterpret_cast<unsigned char**>(actor + 44 * i);
                if (source == nullptr) continue;
                unsigned char* slot = player + 23984 * j;
                if (objectOf(slot) == nullptr || !(levelOf(slot) > 0.0f)) continue;
                copyPosition(objectOf(slot), source);
                float request[3];
                request[0] = *reinterpret_cast<AliasedFloat*>(source + 16);
                request[1] = *reinterpret_cast<AliasedFloat*>(source + 20);
                request[2] = *reinterpret_cast<AliasedFloat*>(source + 24);
                float spot[3];
                objectFindEmptySpot__FG6Point3iiPc(spot, request, 36, 14, nullptr);
                unsigned char* object = objectOf(slot);
                *reinterpret_cast<AliasedFloat*>(object + 16) = spot[0];
                *reinterpret_cast<AliasedFloat*>(object + 20) = spot[1];
                *reinterpret_cast<AliasedFloat*>(object + 24) = spot[2];
                source = *reinterpret_cast<unsigned char**>(actor + 44 * i);
                object = objectOf(slot);
                *reinterpret_cast<AliasedUShort*>(object + 56) =
                    static_cast<unsigned short>(*reinterpret_cast<AliasedUShort*>(source + 56) + 8000 * j);
                float where[3];
                where[0] = *reinterpret_cast<AliasedFloat*>(object + 16);
                where[1] = *reinterpret_cast<AliasedFloat*>(object + 20);
                where[2] = *reinterpret_cast<AliasedFloat*>(object + 24);
                const float height = worldFindHeight__FP12_worldHeaderG6Point3f(world, where, 12.0f);
                object = objectOf(slot);
                *reinterpret_cast<AliasedFloat*>(object + 24) = height;
                objectUpdateInGrid__FP10GameObject(object);
            }
        }
        record = actor + 44 * i;
        unsigned char* object = *reinterpret_cast<unsigned char**>(record);
        if (static_cast<signed char>(record[43]) != 0) {
            if (object != nullptr) {
                const VirtualEntry* entry = reinterpret_cast<const VirtualEntry*>(
                    *reinterpret_cast<unsigned char* const*>(object + 72) + 8);
                entry->function(object + entry->adjust, 3);
            }
        } else if (object != nullptr) {
            const unsigned old = *reinterpret_cast<AliasedUnsigned*>(object + 12);
            *reinterpret_cast<AliasedUnsigned*>(object + 12) = old & 0xfff77fffu;
            if ((old & 0x20) != 0) object[570] = static_cast<unsigned char>(object[570] & 0xfe);
        }
    }

    if (frontEndRunning == 0) {
        for (int j = 0; j < nmPlayers; ++j) {
            unsigned char* slot = player + 23984 * j;
            unsigned char* object = objectOf(slot);
            if (object != nullptr && levelOf(slot) > 0.0f) animRemoveNonpermanentAnimations__FP14AnimationState(object + 288);
        }
    }
    worldSetFarClip__Ff(1800.0f);
    worldSetFrustum__Fffff(50.0f, 0x1.547ae0p+0f, 80.0f, 1.0f);
    lightGlobalCharLightScale__Ff(1.0f);
    if ((flags & 8) != 0) objectIterateRunLists__FPFP10GameObject_v(unremoveMonster__FP10GameObject);
    else wakeMonsters__8creaturev();
    if ((flags & 0x10) != 0) engineScreenSlide = 0;
    for (int k = 0; k < 2; ++k) {
        if (anim[k].block != nullptr) {
            if (animationLoading != 0) {
                animationLoading = 0;
                cdWait__Fv();
            }
            memFree__FPv(anim[k].block);
            anim[k].block = nullptr;
            anim[k].second = 0;
        }
    }
    if (dialogActor != nullptr) {
        unsigned long long* word = reinterpret_cast<unsigned long long*>(dialogActor + 160);
        *word = *word & 0xffffffffefffffffULL;
        dialogActor = nullptr;
    }
}
