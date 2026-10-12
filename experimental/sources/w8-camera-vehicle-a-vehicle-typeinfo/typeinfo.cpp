// Observed __tf7Vehicle at 0x00316558, 76 bytes.
// The 12-byte typeinfo object is __ti7Vehicle at 0x008eb6a8; it is zero in the file image. Its
// first word gates initialization. A zero word calls __tf10GameObject, then __rtti_si with
// this object, the class-name text "7Vehicle", and the base typeinfo __ti10GameObject. A
// nonzero first word returns the object and makes neither call. The object
// is returned either way. Nothing here was executed on the EE.
extern "C" {
unsigned char __ti7Vehicle[12];
void* __tf10GameObject();
void* __rtti_si(void* typeinfo, const char* name, const void* base);
extern unsigned char __ti10GameObject[];
}

typedef unsigned __attribute__((may_alias)) AliasedWord;

extern "C" void* __tf7Vehicle() {
    if (*reinterpret_cast<AliasedWord*>(__ti7Vehicle) == 0u) {
        __tf10GameObject();
        __rtti_si(__ti7Vehicle, "7Vehicle", __ti10GameObject);
    }
    return __ti7Vehicle;
}
