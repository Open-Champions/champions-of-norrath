// Host contract for __17VehicleBodySphereP7Vehicleff. Not an EE plan input.
#include "vehicle_body_sphere.cpp"

#include <cstdio>
#include <cstring>

Point3 shite;
unsigned char _vt$17VehicleBodySphere[144];

static int calls;
static void* seen_self;
static const Point3* seen_position;
static void* seen_map;
static char** seen_args;
extern "C" void __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* self, const Point3* position, void* map, char** args) {
    ++calls; seen_self = self; seen_position = position; seen_map = map; seen_args = args;
    unsigned w = 0x00000100;  // the base constructor leaves some flag bits set
    std::memcpy((unsigned char*)self + 12, &w, 4);
}

int main() {
    int failures = 0;
    unsigned char object[128];
    std::memset(object, 0x5c, sizeof object);
    unsigned char vehicle[8];
    __17VehicleBodySphereP7Vehicleff(object, vehicle, 1.5f, 7.25f);
    unsigned flags; std::memcpy(&flags, object + 12, 4);
    float a, b; std::memcpy(&a, object + 80, 4); std::memcpy(&b, object + 28, 4);
    unsigned vptr, vtbl; std::memcpy(&vptr, object + 76, 4); std::memcpy(&vtbl, object + 72, 4);
    if (calls != 1 || seen_self != object || seen_position != &shite || seen_map != nullptr || seen_args != nullptr) { std::fprintf(stderr, "fail: base constructor call\n"); ++failures; }
    if (flags != (0x100u | 0x4002u)) { std::fprintf(stderr, "fail: flags %x\n", flags); ++failures; }
    if (a != 1.5f || b != 7.25f) { std::fprintf(stderr, "fail: floats (a goes to +80, b to +28)\n"); ++failures; }
    if (vptr != (unsigned)(__UINTPTR_TYPE__)vehicle || vtbl != (unsigned)(__UINTPTR_TYPE__)_vt$17VehicleBodySphere) { std::fprintf(stderr, "fail: pointers\n"); ++failures; }
    unsigned char expect[128];
    std::memset(expect, 0x5c, sizeof expect);
    std::memcpy(expect + 12, &flags, 4); std::memcpy(expect + 28, &b, 4); std::memcpy(expect + 80, &a, 4);
    std::memcpy(expect + 76, &vptr, 4); std::memcpy(expect + 72, &vtbl, 4);
    if (std::memcmp(expect, object, sizeof object) != 0) { std::fprintf(stderr, "fail: other bytes written\n"); ++failures; }
    return failures ? 1 : 0;
}
