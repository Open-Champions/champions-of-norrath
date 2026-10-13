// Observed __tf12RecallEffect at 0x00310560, 80 bytes (weak). The 12-byte typeinfo object __ti12RecallEffect is zero in
// the file image and is defined here. Its first word gates initialization: when zero, the function calls
// __tf10GameObject, then __rtti_si with this object, the class-name text "12RecallEffect" and the base typeinfo
// __ti10GameObject (a small-data symbol in the image). A nonzero first word makes neither call. The object is returned either way. That
// the class has the single base named by the base typeinfo is read from the single-inheritance
// registration call alone; nothing else about the class is claimed. Nothing here ran on the EE.
extern "C" {
unsigned char __ti12RecallEffect[12];
void* __tf10GameObject();
void* __rtti_si(void* typeinfo, const char* name, const void* base);
extern unsigned char __ti10GameObject[];
}

typedef unsigned __attribute__((may_alias)) AliasedWord;

extern "C" void* __tf12RecallEffect() {
    if (*reinterpret_cast<AliasedWord*>(__ti12RecallEffect) == 0u) {
        __tf10GameObject();
        __rtti_si(__ti12RecallEffect, "12RecallEffect", __ti10GameObject);
    }
    return __ti12RecallEffect;
}
