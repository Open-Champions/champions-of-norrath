// Observed __tf9MummyKing at 0x00316040, 80 bytes. The 12-byte typeinfo object is __ti9MummyKing
// at 0x008eb618; it is zero in the file image and is defined here. Its first word gates
// initialization. A zero word calls __tf8Creature, then __rtti_si with this object, the
// class-name text "9MummyKing", and the base typeinfo __ti8Creature. A nonzero first word
// returns the object and makes neither call. The object is returned either way.
// That the class has the single base named by the base typeinfo is read from the
// single-inheritance registration call alone; nothing else about the class is claimed.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti9MummyKing[12];
void* __tf8Creature();
void* __rtti_si(void* typeinfo, const char* name, const void* base);
extern unsigned char __ti8Creature[];
}

typedef unsigned __attribute__((may_alias)) AliasedWord;

extern "C" void* __tf9MummyKing() {
    if (*reinterpret_cast<AliasedWord*>(__ti9MummyKing) == 0u) {
        __tf8Creature();
        __rtti_si(__ti9MummyKing, "9MummyKing", __ti8Creature);
    }
    return __ti9MummyKing;
}
