// Observed numInRegion__8creatureG6Point3f at 0x00108528, 240 bytes.
// Arguments: a Point3 passed by address and a single radius. A box from the point minus the radius
// to the point plus the radius (per axis) is passed with a 128-entry output array and the capacity
// 128 to objectFindInBox__FG6Point3T0PP10GameObjecti. The listed objects are walked until a null
// entry (the walk is not bounded by the capacity); the result is the number of listed objects
// whose word at +12, masked with 0x68, equals 0x20. An empty list gives 0.
// Unknown: what the bits 0x20 and 0x68 of the object word at +12 mean, and whether the finder
// always writes a terminating null.
#include "../w11-creature-a-common/words.hpp"

extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(w11_creature_a::Point3 minimum, w11_creature_a::Point3 maximum, void** output, int capacity);

extern "C" int numInRegion__8creatureG6Point3f(w11_creature_a::Point3 position, float radius) {
    using namespace w11_creature_a;
    void* found[128];
    Point3 minimum, maximum;
    minimum.x = position.x - radius;
    minimum.y = position.y - radius;
    minimum.z = position.z - radius;
    maximum.x = position.x + radius;
    maximum.y = position.y + radius;
    maximum.z = position.z + radius;
    found[0] = nullptr;
    objectFindInBox__FG6Point3T0PP10GameObjecti(minimum, maximum, found, 128);
    int count = 0;
    for (void** entry = found; *entry != nullptr; ++entry) {
        if ((at<unsigned>(*entry, 12) & 0x68u) == 0x20u) ++count;
    }
    return count;
}
