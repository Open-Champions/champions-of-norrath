// Observed __17VehicleBodySphereP7Vehicleff at 0x0021cf08, 124 bytes: the
// constructor of VehicleBodySphere. Arguments: the object, a Vehicle pointer
// and two floats (a, b; a arrives first). Steps:
//   __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(object, &shite, 0, 0)
//   with the global zero Point3 `shite` (0x007d74e0) passed by address;
//   float +28 = b; word +12 |= 0x4002; pointer +76 = vehicle; float +80 = a;
//   pointer +72 = the address of _vt$17VehicleBodySphere (0x004489e0).
// Nothing is returned (the object is not handed back in v0 by this code).
// What a and b are is not established; buildCollisionInfo__7Vehicle calls this
// with a y value and a radius, which is the only hint. Nothing here was
// executed on the EE.
struct __attribute__((may_alias)) Point3 { float x, y, z; };

extern "C" {
void __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* self, const Point3* position, void* resource_map, char** arguments);
extern Point3 shite;
extern unsigned char _vt$17VehicleBodySphere[];
}

typedef float AliasedFloat __attribute__((may_alias));
typedef unsigned AliasedWord __attribute__((may_alias));

extern "C" void __17VehicleBodySphereP7Vehicleff(unsigned char* object, unsigned char* vehicle, float a, float b) {
    __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(object, &shite, 0, 0);
    *reinterpret_cast<AliasedFloat*>(object + 28) = b;
    *reinterpret_cast<AliasedWord*>(object + 12) = *reinterpret_cast<AliasedWord*>(object + 12) | 0x4002u;
    *reinterpret_cast<AliasedWord*>(object + 76) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vehicle));
    *reinterpret_cast<AliasedFloat*>(object + 80) = a;
    *reinterpret_cast<AliasedWord*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$17VehicleBodySphere));
}
