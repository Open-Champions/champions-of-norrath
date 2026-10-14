// Observed __tf8Skelight at 0x316410, 80 bytes. The 12-byte type-info object is __ti8Skelight (0x8fb678).
// Its first word gates initialisation: when zero, __tf8Creature runs and then __rtti_si is called with
// the object, the class-name text "8Skelight" and the base type info __ti8Creature. The object is
// returned either way. Inferred: single inheritance from Creature (the base pair). This unit defines
// the 12 bytes of storage, as the other authored __tf units do; the image has it zero-initialised.
extern "C" {
unsigned char __ti8Skelight[12];
void* __tf8Creature();
void* __rtti_si(void* typeinfo, const char* name, const void* base);
extern unsigned char __ti8Creature[];
}

typedef unsigned __attribute__((may_alias)) AliasedWord;

extern "C" void* __tf8Skelight() {
    if (*reinterpret_cast<AliasedWord*>(__ti8Skelight) == 0u) {
        __tf8Creature();
        __rtti_si(__ti8Skelight, "8Skelight", __ti8Creature);
    }
    return __ti8Skelight;
}
