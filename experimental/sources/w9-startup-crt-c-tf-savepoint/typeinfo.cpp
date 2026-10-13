// Observed __tf9Savepoint at 0x00312060, 76 bytes. The 12-byte typeinfo object is __ti9Savepoint
// at 0x008eafc8; it is zero in the file image and is defined here. Its first word gates
// initialization. A zero word calls __tf10GameObject, then __rtti_si with this object, the
// class-name text "9Savepoint", and the base typeinfo __ti10GameObject. A nonzero first word
// returns the object and makes neither call. The object is returned either way.
// That the class has the single base named by the base typeinfo is read from the
// single-inheritance registration call alone; nothing else about the class is claimed.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti9Savepoint[12];
void* __tf10GameObject();
void* __rtti_si(void* typeinfo, const char* name, const void* base);
extern unsigned char __ti10GameObject[];
}

typedef unsigned __attribute__((may_alias)) AliasedWord;

extern "C" void* __tf9Savepoint() {
    if (*reinterpret_cast<AliasedWord*>(__ti9Savepoint) == 0u) {
        __tf10GameObject();
        __rtti_si(__ti9Savepoint, "9Savepoint", __ti10GameObject);
    }
    return __ti9Savepoint;
}
