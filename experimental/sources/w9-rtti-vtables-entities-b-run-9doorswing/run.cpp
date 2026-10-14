// Observed msg_run__9DoorSwing at 0x212de0, 292 bytes.
// Each call: reads the signed halfword at object+80 (a position) and the float at
// object+84 (a velocity); the float is scaled by 0.98 (word 0x3f7ae147) and stored back,
// and the halfword becomes the truncated sum of the halfword and the old float. Then,
// when the string at levelName+81 contains "gothic": if the new halfword is at least
// 17385 and the float is positive, the float becomes minus itself times the word
// 0x3dcccccc; otherwise (no "gothic") the same with threshold 25469 and the factor
// 0x3f3d70a3. Next worldMoveDoor(world, door, point, step) is called with the signed
// halfword at +76 as the door index, the point (0, 0, 0) and step the low 16 bits of the
// unsigned halfword at +80 times the signed byte at +88. Last, if the absolute value of
// the float at +84 (read again after the call) is below 1.0, the object is removed
// from the run list.
// Unknown: what units the position and velocity use, and what the byte at +88 selects.

extern "C" {
struct Point3 { float x, y, z; };
int worldMoveDoor__FP12_worldHeaderiG6Point3s(void* world, int index, Point3 point, short extra);
void objectRemoveFromRunList__FP10GameObject(void* object);
char* strstr(const char* haystack, const char* needle);
extern char levelName[];
extern int world;
}

typedef short Half __attribute__((may_alias));
typedef float Single __attribute__((may_alias));

static inline float single_from_bits(unsigned bits) { return __builtin_bit_cast(float, bits); }

extern "C" void msg_run__9DoorSwing(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    Single* velocity = reinterpret_cast<Single*>(object + 84);
    Half* position = reinterpret_cast<Half*>(object + 80);
    const float damping = single_from_bits(0x3f7ae147u);
    float sum = static_cast<float>(*position) + *velocity;
    *velocity = *velocity * damping;
    Point3 origin = {0.0f, 0.0f, 0.0f};
    *position = static_cast<short>(static_cast<int>(sum));
    if (strstr(levelName + 81, "gothic") != 0) {
        if (*position >= 17385 && *velocity > 0.0f) *velocity = -*velocity * single_from_bits(0x3dccccccu);
    } else {
        if (*position >= 25469 && *velocity > 0.0f) *velocity = -*velocity * single_from_bits(0x3f3d70a3u);
    }
    int step = static_cast<int>(*reinterpret_cast<unsigned short*>(object + 80)) * static_cast<signed char>(object[88]);
    worldMoveDoor__FP12_worldHeaderiG6Point3s(reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(world)),
                                              *reinterpret_cast<Half*>(object + 76), origin, static_cast<short>(step));
    float magnitude = __builtin_fabsf(*velocity);
    if (magnitude < 1.0f) objectRemoveFromRunList__FP10GameObject(self);
}
