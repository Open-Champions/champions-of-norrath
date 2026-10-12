// Observed __tf11LavaTractor at 0x00316710, 80 bytes.
// The 12-byte typeinfo object is __ti11LavaTractor at 0x008eb6d8; it is zero in the file image.
// Its first word gates initialization. A zero word calls __tf11LavaVehicle, then __rtti_si
// with this object, the class-name text "11LavaTractor", and the base typeinfo
// __ti11LavaVehicle. A nonzero first word returns the object and makes neither call. The
// object is returned either way. Nothing here was executed on the EE.
extern "C" {
unsigned char __ti11LavaTractor[12];
void* __tf11LavaVehicle();
void* __rtti_si(void* typeinfo, const char* name, const void* base);
extern unsigned char __ti11LavaVehicle[];
}

typedef unsigned __attribute__((may_alias)) AliasedWord;

extern "C" void* __tf11LavaTractor() {
    if (*reinterpret_cast<AliasedWord*>(__ti11LavaTractor) == 0u) {
        __tf11LavaVehicle();
        __rtti_si(__ti11LavaTractor, "11LavaTractor", __ti11LavaVehicle);
    }
    return __ti11LavaTractor;
}
