// Observed __tf10StaticFire at 0x312270, 76 bytes (weak). The 12-byte typeinfo object __ti10StaticFire is zero in the
// file image and is defined here. Its first word gates initialization: when zero, the function
// calls __tf10GameObject, then __rtti_si with this object, the class-name text "10StaticFire", and the base
// typeinfo __ti10GameObject. A nonzero first word makes neither call. The object is returned either way.
// That the class has the single base named by the base typeinfo is read from the
// single-inheritance registration call alone; nothing else about the class is claimed.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti10StaticFire[12];
void* __tf10GameObject();
void* __rtti_si(void* typeinfo, const char* name, const void* base);
extern unsigned char __ti10GameObject[];
}

typedef unsigned __attribute__((may_alias)) AliasedWord;

extern "C" void* __tf10StaticFire() {
    if (*reinterpret_cast<AliasedWord*>(__ti10StaticFire) == 0u) {
        __tf10GameObject();
        __rtti_si(__ti10StaticFire, "10StaticFire", __ti10GameObject);
    }
    return __ti10StaticFire;
}
