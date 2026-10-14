// Observed __tf8SuperOrc at 0x30f7a8, 80 bytes (weak). The 12-byte typeinfo object __ti8SuperOrc is zero in the
// file image and is defined here. Its first word gates initialization: when zero, the function
// calls __tf8Creature, then __rtti_si with this object, the class-name text "8SuperOrc", and the base
// typeinfo __ti8Creature. A nonzero first word makes neither call. The object is returned either way.
// That the class has the single base named by the base typeinfo is read from the
// single-inheritance registration call alone; nothing else about the class is claimed.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti8SuperOrc[12];
void* __tf8Creature();
void* __rtti_si(void* typeinfo, const char* name, const void* base);
extern unsigned char __ti8Creature[];
}

typedef unsigned __attribute__((may_alias)) AliasedWord;

extern "C" void* __tf8SuperOrc() {
    if (*reinterpret_cast<AliasedWord*>(__ti8SuperOrc) == 0u) {
        __tf8Creature();
        __rtti_si(__ti8SuperOrc, "8SuperOrc", __ti8Creature);
    }
    return __ti8SuperOrc;
}
