// Observed __tf6Switch at 0x00311be8, 132 bytes (weak). The 12-byte typeinfo object __ti6Switch
// is zero in the file image and is defined here. Its first word gates initialization. When it is
// zero, the function first makes sure the 9WorldPart typeinfo (__ti9WorldPart, defined elsewhere)
// is registered: if that object's first word is zero it calls __tf10GameObject and then
// __rtti_si with __ti9WorldPart, the text "9WorldPart" and __ti10GameObject. That step is the
// body of __tf9WorldPart carried out inline (no call to the function itself is made). Then
// __rtti_si is called with this object, the text "6Switch" and __ti9WorldPart. A nonzero first
// word makes no call. The object is returned either way.
// Inferred from the registration calls only: 6Switch derives from 9WorldPart, which derives from
// 10GameObject. Nothing here was executed on the EE.
extern "C" {
unsigned char __ti6Switch[12];
extern unsigned char __ti9WorldPart[];
extern unsigned char __ti10GameObject[];
void* __tf10GameObject();
void* __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;

extern "C" void* __tf6Switch() {
    if (*reinterpret_cast<AliasedWord*>(__ti6Switch) == 0u) {
        if (*reinterpret_cast<AliasedWord*>(__ti9WorldPart) == 0u) {
            __tf10GameObject();
            __rtti_si(__ti9WorldPart, "9WorldPart", __ti10GameObject);
        }
        __rtti_si(__ti6Switch, "6Switch", __ti9WorldPart);
    }
    return __ti6Switch;
}
