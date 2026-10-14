// Observed sendAlertMessage__FifG6Point3P10GameObject at 0x001134e0, 308 bytes
// in the pinned SLUS_205.65 image; arguments (kind, radius, center, sender), no
// result. The point is passed by reference to a caller-made copy.
//  1. It copies the three-float center, builds the box center -/+ radius on
//     each axis (floats, single precision) and calls
//     objectFindInBox__FG6Point3T0PP10GameObjecti(&low, &high, list, 128) with a
//     list of up to 128 object pointers on the stack. The list is ended by a
//     null pointer (the callee's convention is not read here).
//  2. For each listed object, in order, it reads the pointer at object + 72,
//     takes the 8-byte entry at that pointer + 96 (a halfword adjustment at
//     +0 and a function pointer at +4) and calls the function with
//     (object + adjustment, kind, radius [float register], &center_copy, sender).
//     The call goes through that table entry, so each class decides what the
//     alert does. The loop does nothing when the first listed pointer is null.
// Inferred: a broadcast of one numeric alert kind to the objects around a point
// (used by the damage handler with kind 0 and radius 600). The table at +72 is
// the object's virtual-function table; entry 12 is what is called here.
// Unknown: the name of that virtual method and the meaning of kinds.
struct Point3 { float x, y, z; };
extern "C" {
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** list, int count);

namespace {
struct VirtualEntry { short adjust; short pad; void (*function)(void* self, int kind, float radius, const Point3* from, void* sender); };
}

void sendAlertMessage__FifG6Point3P10GameObject(int kind, float radius, const Point3* center, void* sender) {
    Point3 point = *center;
    Point3 low = {point.x - radius, point.y - radius, point.z - radius};
    Point3 high = {point.x + radius, point.y + radius, point.z + radius};
    void* list[128];
    objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 128);
    for (int i = 0; list[i] != 0; ++i) {
        unsigned char* object = static_cast<unsigned char*>(list[i]);
        const VirtualEntry* entry = reinterpret_cast<const VirtualEntry*>(*reinterpret_cast<unsigned char**>(object + 72) + 96);
        entry->function(object + entry->adjust, kind, radius, &point, sender);
    }
}
}
