// Observed ISeeDeadPeople__8Creaturef at 0x0010c1c8, 364 bytes.
// Argument: the creature and a single radius. A box from the creature position (+16) minus the
// radius to the position plus the radius (per axis) is passed with a 256-entry output array and
// the capacity 256 to objectFindInBox__FG6Point3T0PP10GameObjecti. The output array is not
// cleared first; the walk runs until a null entry. An object is skipped unless its word at +12,
// masked with 0x2a, equals 0x28, and its halfword at +294 has bit 0x80 set. For the remaining
// objects the type-info of the object and of the creature are obtained (each through the function
// word at byte 4 of the table at +72, called with no arguments, with a null check that calls
// __throw_bad_typeid) and compared with __eq__C9type_infoRC9type_info; the first object that
// matches is returned. The result is 0 when the walk ends.
// Inferred: bit 0x80 at +294 marks a corpse and the match finds one of the creature's own kind
// (the name of the function).
// Unknown: the meaning of the +12 bits and of the halfword at +294.
#include "../w11-creature-a-common/words.hpp"

extern "C" {
void objectFindInBox__FG6Point3T0PP10GameObjecti(w11_creature_a::Point3 minimum, w11_creature_a::Point3 maximum, void** output, int capacity);
int __eq__C9type_infoRC9type_info(const void* left, const void* right);
void __throw_bad_typeid();
}

namespace {
inline const void* typeInfoOf(void* object) {
    using namespace w11_creature_a;
    if (object == nullptr) __throw_bad_typeid();
    const unsigned char* table = at<const unsigned char*>(object, 72);
    return reinterpret_cast<const void* (*)()>(*reinterpret_cast<const Ptr*>(table + 4))();
}
}  // namespace

extern "C" void* ISeeDeadPeople__8Creaturef(void* self, float radius) {
    using namespace w11_creature_a;
    void* found[256];
    Point3 minimum, maximum;
    const Point3 position = at<Point3>(self, 16);
    minimum.x = position.x - radius;
    minimum.y = position.y - radius;
    minimum.z = position.z - radius;
    maximum.x = position.x + radius;
    maximum.y = position.y + radius;
    maximum.z = position.z + radius;
    objectFindInBox__FG6Point3T0PP10GameObjecti(minimum, maximum, found, 256);
    for (void** entry = found; *entry != nullptr; ++entry) {
        void* object = *entry;
        if ((at<unsigned>(object, 12) & 0x2au) != 0x28u) continue;
        if ((at<unsigned short>(object, 294) & 0x80u) == 0) continue;
        if (__eq__C9type_infoRC9type_info(typeInfoOf(object), typeInfoOf(self)) != 0) return object;
    }
    return nullptr;
}
