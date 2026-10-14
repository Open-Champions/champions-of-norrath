// Observed __tf8SkulBoat at 0x0031a258, 80 bytes.
// The 12-byte typeinfo object is __ti8SkulBoat at 0x008ebd28; it is zero in the file image.
// Its first word gates initialization. A zero word calls __tf7Vehicle, then __rtti_si
// with this object, the class-name text "8SkulBoat", and the base typeinfo
// __ti7Vehicle. A nonzero first word returns the object and makes neither call. The
// object is returned either way. Nothing here was executed on the EE.
extern "C" {
unsigned char __ti8SkulBoat[12];
void* __tf7Vehicle();
void* __rtti_si(void* typeinfo, const char* name, const void* base);
extern unsigned char __ti7Vehicle[];
}

typedef unsigned __attribute__((may_alias)) AliasedWord;

extern "C" void* __tf8SkulBoat() {
    if (*reinterpret_cast<AliasedWord*>(__ti8SkulBoat) == 0u) {
        __tf7Vehicle();
        __rtti_si(__ti8SkulBoat, "8SkulBoat", __ti7Vehicle);
    }
    return __ti8SkulBoat;
}
