// Observed _$_11LavaVehicle at 0x00316760, 100 bytes (weak).
// Stores the LavaVehicle table pointer (_vt$11LavaVehicle, the .data object at 0x450348) at object+72, calls the Vehicle destructor (_$_7Vehicle) on the
// object with flag 0, then calls blockFree__FPv on the object when bit 0 of the incoming flags is set.
// The free is a tail call in the image.
// Inferred: this is the LavaVehicle destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_LavaVehicle[] __asm__("_vt$11LavaVehicle");
void vehicle_destructor(void* self, int flags) __asm__("_$_7Vehicle");
void blockFree__FPv(void* block);
}

extern "C" void lava_vehicle_destructor(void* self, int flags) __asm__("_$_11LavaVehicle");
extern "C" void lava_vehicle_destructor(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_LavaVehicle));
    vehicle_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
