// Observed _$_11LavaTractor at 0x003166a8, 100 bytes.
// Stores the low 32 bits of the address of _vt$11LavaVehicle (0x00450348) at object+72,
// then calls _$_7Vehicle with the same object and flags 0. Bit 0 of the
// incoming flag, kept from before that call, selects blockFree__FPv on the
// object afterwards. Other flag bits do nothing. Nothing is returned. Nothing
// here was executed on the EE.
extern "C" {
void _$_7Vehicle(void* object, int flags);
void blockFree__FPv(void* object);
extern unsigned char _vt$11LavaVehicle[];
}

typedef unsigned __attribute__((may_alias)) AliasedWord;

extern "C" void _$_11LavaTractor(unsigned char* object, int flags) {
    *reinterpret_cast<AliasedWord*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$11LavaVehicle));
    const int release = flags & 1;
    _$_7Vehicle(object, 0);
    if (release != 0) blockFree__FPv(object);
}
