// Observed __tf11LavaVehicle at 0x003167c8, 80 bytes. The 12-byte typeinfo object is __ti11LavaVehicle
// at 0x008eb6c8; it is zero in the file image and is defined here. Its first word gates
// initialization. A zero word calls __tf7Vehicle, then __rtti_si with this object, the
// class-name text "11LavaVehicle", and the base typeinfo __ti7Vehicle. A nonzero first word
// returns the object and makes neither call. The object is returned either way.
// That the class has the single base named by the base typeinfo is read from the
// single-inheritance registration call alone; nothing else about the class is claimed.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti11LavaVehicle[12];
void* __tf7Vehicle();
void* __rtti_si(void* typeinfo, const char* name, const void* base);
extern unsigned char __ti7Vehicle[];
}

typedef unsigned __attribute__((may_alias)) AliasedWord;

extern "C" void* __tf11LavaVehicle() {
    if (*reinterpret_cast<AliasedWord*>(__ti11LavaVehicle) == 0u) {
        __tf7Vehicle();
        __rtti_si(__ti11LavaVehicle, "11LavaVehicle", __ti7Vehicle);
    }
    return __ti11LavaVehicle;
}
