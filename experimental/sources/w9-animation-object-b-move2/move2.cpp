// Observed move2__8CreatureG6Point3 at 0x10fc20, extent 1688. Moves a creature for one
// frame by a requested displacement and reports what it touched. The argument after
// the creature is a Point3 passed by value; the original receives a pointer to the
// caller's copy and works on a copy of its own. Object words used (creature and
// touched objects share the leading layout): +12 flags, +16/+20/+24 position,
// +28 radius, +32 height, +48 vertical speed, +56 a halfword angle, +72 virtual table
// (8-byte entries: halfword delta, function four bytes on), +160 64-bit flags,
// +176/+180/+184 floats, +188 a halfword, +568/+569 signed bytes, +570 a byte,
// +794 a halfword, +816/+820 floats. The result is a bit set: 1 the world move reported
// a hit, 2 at least one object was touched, 4 the creature landed on the ground.
// In order:
//   1. If bit 3 (0x8) of the byte at +570 is set, return 0 at once.
//   2. d.x += (+180) * 60, d.y += (+184) * 60. The angle at +56 gains the halfword
//      at +188. If the halfword at +794 is nonzero, d becomes (0, 0, 0). The floats
//      at +816/+820 take d.x and d.y; +180/+184 and the halfword at +188 are zeroed.
//   3. Water state (signed byte +568). If 64-bit flag 0x100000 is set the byte is
//      cleared. Otherwise, with the byte nonzero: when the float at +176 is below 0.01
//      the world water is perturbed (centre = position, 300.0, 12.0) only on frames
//      whose low four bits of frameNm are zero (otherwise the creature is treated as
//      still in water); when it is not below 0.01 the perturbation happens every
//      call; a zero answer clears the byte. With the byte zero: only when
//      ((frameNm ^ byte at +569) & 7) == 0, worldCheckForWater(world, position, 12.0,
//      null) is asked; if it answers nonzero the byte is set to 1, eRand is drawn
//      (result unused), the text "foly_wtrsplash_sml%d.anm" is formatted with the
//      number 1, looked up in the lump "fx.lmp", played with soundPlaySequence at
//      volume 0.6 on the creature, and the water is perturbed (position, 1500.0, 24.0).
//   4. d is scaled by 1/60 on all three axes. When d.x^2 + d.y^2 exceeds 0.001 the
//      creature is moved with objectMoveWithWorldCollision(position, d, radius, 3,
//      hit-record) and objectUpdateInGrid; result bit 1 is set when the move's
//      return value was nonzero.
//   5. A box (position -/+ radius in x and y, -/+ 1000.0 in z) is queried with
//      objectFindInBox for up to 16 objects. Each object is skipped when its flag
//      bit 0 is set or it is the creature, or when the horizontal distance plus 0.001
//      is not below the sum of the radii. For an overlapping object:
//      - When bit 4 (0x10) is set in either object's flags: the object is skipped if
//        the creature's top (z + height) is below the object's z + 3.0, or the object's
//        top (z + height) is below the creature's z. When the object's top is less than
//        5.0 above the creature's z: with object flag 0x200 it is counted as touched
//        (below); otherwise with creature flag 0x400 it falls through to the push-apart
//        test; otherwise the highest such top is remembered (starting at -100000.0)
//        and the object is not counted.
//      - Push-apart test: unless the object has flag 0x200, the creature has flag 0x400
//        or 0x1, or the distance is not above 0.001, the creature is moved by the
//        horizontal offset away from the object scaled by (r1 + r2 - distance) /
//        distance (z zero) with the same world move, then objectUpdateInGrid.
//      - A counted object sets result bit 2, is appended to the touched list, and its
//        virtual entry at +48 is called with the creature as the argument.
//      The touched list is terminated with a null pointer.
//   6. ground = worldFindStandHeight(world, position, radius), raised to the remembered
//      top if that is higher. The vertical speed at +48 loses 9.0 (the constant at
//      0x47fc3c), z gains speed / 60, and when z - 0.01 is not above the ground z
//      becomes the ground and, if the vertical speed is below zero, it is zeroed and
//      result bit 4 is set.
//   7. If bit 1 is set, the virtual entry at +80 is called with the hit record; if bit 2
//      is set, the entry at +88 is called with the touched list. The result is returned.
// Inferred: +180/+184 hold a pending displacement per frame; the entries at +48, +80
// and +88 are contact callbacks. Unknown: the hit record layout (16 bytes assumed from
// the stack gap), flag meanings, the origin of the constant 9.0, and what +176 is.
struct Point3 {
    float x, y, z;
};
struct VirtualEntry {
    short delta;
    short reserved;
    unsigned function;
};
static_assert(sizeof(VirtualEntry) == 8, "table entries are eight bytes");
// Host contracts define NORRATH_HOST_CONTRACT: a host function address does not fit the 32-bit
// entry word, so they supply the handle -> function mapping. On the target the word is the address.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" void* norrath_host_function(unsigned handle);
#endif
struct MoveObject {
    unsigned char before_flags[12];
    unsigned flags;
    Point3 position;
    float radius;
    float height;
    unsigned char before_vertical[48 - 36];
    float vertical_speed;
    unsigned char before_angle[56 - 52];
    unsigned short angle;
    unsigned char before_vtable[72 - 58];
    const VirtualEntry* vtable;
    unsigned char before_flags64[160 - 76];
    unsigned long long flags64;
    unsigned char before_176[176 - 168];
    float water_depth;
    float pending_x;
    float pending_y;
    unsigned short pending_angle;
    unsigned char before_568[568 - 190];
    signed char in_water;
    signed char splash_phase;
    unsigned char flags570;
    unsigned char before_794[794 - 571];
    short frozen;
    unsigned char before_816[816 - 796];
    float last_x;
    float last_y;
};
#if defined(__mips__)
static_assert(__builtin_offsetof(MoveObject, flags) == 12 && __builtin_offsetof(MoveObject, position) == 16 &&
                  __builtin_offsetof(MoveObject, radius) == 28 && __builtin_offsetof(MoveObject, height) == 32 &&
                  __builtin_offsetof(MoveObject, vertical_speed) == 48 && __builtin_offsetof(MoveObject, angle) == 56 &&
                  __builtin_offsetof(MoveObject, vtable) == 72 && __builtin_offsetof(MoveObject, flags64) == 160 &&
                  __builtin_offsetof(MoveObject, water_depth) == 176 && __builtin_offsetof(MoveObject, pending_x) == 180 &&
                  __builtin_offsetof(MoveObject, pending_y) == 184 && __builtin_offsetof(MoveObject, pending_angle) == 188 &&
                  __builtin_offsetof(MoveObject, in_water) == 568 && __builtin_offsetof(MoveObject, splash_phase) == 569 &&
                  __builtin_offsetof(MoveObject, flags570) == 570 && __builtin_offsetof(MoveObject, frozen) == 794 &&
                  __builtin_offsetof(MoveObject, last_x) == 816 && __builtin_offsetof(MoveObject, last_y) == 820 &&
                  true, "observed offsets");
#endif

// Float constants are written as exact bit patterns (hex floats) because the executable's
// values differ from the nearest decimal float in the last place: 1/60 = 0x3c888888,
// 0.001 = 0x3a83126e, 0.01 = 0x3c23d70a, 0.6 = 0x3f199999, 9.0 = 0x410fffff.
extern "C" {
int objectMoveWithWorldCollision__FP6Point3G6Point3fiT0(Point3* position, const Point3* move, float radius, int mode, void* record);
void objectUpdateInGrid__FP10GameObject(void* object);
void objectFindInBox__FG6Point3T0PP10GameObjecti(const Point3* low, const Point3* high, MoveObject** out, int capacity);
int worldPerturbWater__FP12_worldHeaderG6Point3ff(void* world, const Point3* centre, float radius, float strength);
int worldCheckForWater__FP12_worldHeaderG6Point3fPf(void* world, const Point3* point, float depth, float* out);
float worldFindStandHeight__FP12_worldHeaderG6Point3f(void* world, const Point3* point, float radius);
int eRand__10GameObject(void* object);
int sprintf(char* destination, const char* format, ...);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
extern void* world;
extern int frameNm;
}

namespace {
void call_entry(MoveObject* object, unsigned offset, void* argument) {
    const VirtualEntry* entry = reinterpret_cast<const VirtualEntry*>(reinterpret_cast<const char*>(object->vtable) + offset);
#if defined(NORRATH_HOST_CONTRACT)
    void* function = norrath_host_function(entry->function);
#else
    void* function = reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(entry->function));
#endif
    reinterpret_cast<void (*)(void*, void*)>(function)(reinterpret_cast<char*>(object) + entry->delta, argument);
}
}

extern "C" unsigned move2__8CreatureG6Point3(MoveObject* self, const Point3* requested) {
    Point3 d = *requested;
    if ((self->flags570 & 8u) != 0) return 0;
    d.x = d.x + self->pending_x * 60.0f;
    d.y = d.y + self->pending_y * 60.0f;
    self->angle = static_cast<unsigned short>(self->angle + self->pending_angle);
    if (self->frozen != 0) d = Point3{0.0f, 0.0f, 0.0f};
    self->last_x = d.x;
    self->last_y = d.y;
    self->pending_x = 0.0f;
    self->pending_y = 0.0f;
    self->pending_angle = 0;

    Point3* position = &self->position;
    if ((self->flags64 & 0x100000ull) != 0) {
        self->in_water = 0;
    } else if (self->in_water != 0) {
        int answer;
        if (self->water_depth < 0x1.47ae14p-7f) {
            answer = 1;
            if ((frameNm & 0xf) == 0) {
                Point3 centre = *position;
                answer = worldPerturbWater__FP12_worldHeaderG6Point3ff(world, &centre, 300.0f, 12.0f);
            }
        } else {
            Point3 centre = *position;
            answer = worldPerturbWater__FP12_worldHeaderG6Point3ff(world, &centre, 300.0f, 12.0f);
        }
        if (answer == 0) self->in_water = 0;
    } else if ((((frameNm ^ self->splash_phase) & 7)) == 0) {
        Point3 probe = *position;
        if (worldCheckForWater__FP12_worldHeaderG6Point3fPf(world, &probe, 12.0f, nullptr) != 0) {
            self->in_water = 1;
            eRand__10GameObject(self);
            char name[64];
            sprintf(name, "foly_wtrsplash_sml%d.anm", 1);
            static char lump_name[] = "fx.lmp";
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(lump_name, name), self, 0x1.333332p-1f);
            Point3 centre = *position;
            worldPerturbWater__FP12_worldHeaderG6Point3ff(world, &centre, 1500.0f, 24.0f);
        }
    }

    unsigned result = 0;
    d.x = d.x * 0x1.111110p-6f;
    d.y = d.y * 0x1.111110p-6f;
    d.z = d.z * 0x1.111110p-6f;
    float hit_record[4];
    if (0x1.0624dcp-10f < d.x * d.x + d.y * d.y) {
        const int moved = objectMoveWithWorldCollision__FP6Point3G6Point3fiT0(position, &d, self->radius, 3, hit_record);
        objectUpdateInGrid__FP10GameObject(self);
        result = moved != 0;
    }

    Point3 low = {position->x - self->radius, position->y - self->radius, position->z - 1000.0f};
    Point3 high = {position->x + self->radius, position->y + self->radius, position->z + 1000.0f};
    MoveObject* candidates[16];
    objectFindInBox__FG6Point3T0PP10GameObjecti(&low, &high, candidates, 16);
    int touched_count = 0;
    MoveObject* touched[20];
    const float top = self->position.z + self->height;
    float highest = -100000.0f;
    for (MoveObject** cursor = candidates; *cursor != nullptr; ++cursor) {
        MoveObject* other = *cursor;
        if ((other->flags & 1u) != 0) continue;
        if (other == self) continue;
        const float dx = self->position.x - other->position.x;
        const float dy = self->position.y - other->position.y;
        float distance = dx * dx;
        distance = distance + dy * dy;
        const float reach = self->radius + other->radius;
        distance = __builtin_sqrtf(distance);
        if (!(distance + 0x1.0624dcp-10f < reach)) continue;
        bool test_push = true;
        if (((other->flags | self->flags) & 0x10u) != 0) {
            if (top < other->position.z + 3.0f) continue;
            const float other_top = other->position.z + other->height;
            if (other_top < self->position.z) continue;
            if (other_top - self->position.z < 5.0f) {
                if ((other->flags & 0x200u) != 0) {
                    test_push = false;
                } else if ((self->flags & 0x400u) == 0) {
                    if (highest < other_top) highest = other_top;
                    continue;
                }
            }
        }
        if (test_push && (other->flags & 0x200u) == 0 && (self->flags & 0x401u) == 0 && 0x1.0624dcp-10f < __builtin_fabsf(distance)) {
            const float factor = (self->radius + other->radius - distance) / distance;
            Point3 push;
            push.x = dx * factor;
            push.y = dy * factor;
            push.z = 0.0f;
            objectMoveWithWorldCollision__FP6Point3G6Point3fiT0(position, &push, self->radius, 3, nullptr);
            objectUpdateInGrid__FP10GameObject(self);
        }
        result |= 2u;
        ++touched_count;
        call_entry(other, 48, self);
        touched[touched_count - 1] = other;
    }
    touched[touched_count] = nullptr;

    Point3 stand = *position;
    float ground = worldFindStandHeight__FP12_worldHeaderG6Point3f(world, &stand, self->radius);
    if (ground < highest) ground = highest;
    const float speed = self->vertical_speed - 0x1.1ffffep+3f;
    self->vertical_speed = speed;
    const float z = self->position.z + speed * 0x1.111110p-6f;
    self->position.z = z;
    if (z - 0x1.47ae14p-7f <= ground) {
        self->position.z = ground;
        if (speed < 0.0f) {
            self->vertical_speed = 0.0f;
            result |= 4u;
        }
    }
    if ((result & 1u) != 0) call_entry(self, 80, hit_record);
    if ((result & 2u) != 0) call_entry(self, 88, touched);
    return result;
}
