// Observed __12RecallEffectG6Point3 at 0x001e5500, 188 bytes: the RecallEffect constructor. The first argument is
// the object, the second the address of a Point3 (three words) passed by value in the source. In order:
//  1. The Point3 is copied to a local. The GameObject constructor (0x001411f0) runs on the object with that local,
//     a null resource map and a null argument list.
//  2. Word +72 receives the address of _vt$12RecallEffect (0x0044a6f0). Word +12 is read, ORed with 0x4009 and stored
//     back; that store happens before the allocation call below starts.
//  3. blockAlloc__Fi(96) is called. A non-null block is passed to the GlowingCylinder constructor
//     (__15GlowingCylinderG6Point3iiii, 0x001e5828) together with a copy of the three words at object+16 and the
//     integers 70, 86, 126 and 250. The block pointer is not stored in the object, and the constructor's result is
//     not used.
//  4. Byte +84 becomes 1, words +80 and +88 become zero, and word +92 receives the single at 0x00480b14 (bit pattern
//     0x40d0b2e4, about 6.5218).
// Nothing is returned. What the object's fields mean, what the four integers are, and why the cylinder pointer is
// not kept are not established here. Fresh compilation is not an original-byte match.
extern "C" {
unsigned norrath_game_object_construct(void* object, const void* point, void* resource_map, char** arguments);
void* blockAlloc__Fi(int bytes);
void __15GlowingCylinderG6Point3iiii(void* cylinder, const void* point, int a, int b, int c, int d);
extern unsigned char vt_12RecallEffect[] __asm__("_vt$12RecallEffect");
}

namespace {
struct Point3Words {
    unsigned x, y, z;
};
}

extern "C" void __12RecallEffectG6Point3(void* object, const void* point) {
    unsigned* words = static_cast<unsigned*>(object);
    const Point3Words position = *static_cast<const Point3Words*>(point);
    (void)norrath_game_object_construct(object, &position, nullptr, nullptr);
    words[18] = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_12RecallEffect));
    words[3] = words[3] | 0x4009u;
    void* block = blockAlloc__Fi(96);
    if (block != nullptr) {
        const Point3Words center = {words[4], words[5], words[6]};
        __15GlowingCylinderG6Point3iiii(block, &center, 70, 86, 126, 250);
    }
    static_cast<unsigned char*>(object)[84] = 1;
    words[20] = 0u;
    words[22] = 0u;
    words[23] = 0x40d0b2e4u;
}
