// Observed msg_run__11FloorSwitch at 0x2133e8, 796 bytes. Each call:
//  1. worldFindDoorBoundingBox(world, door, low, high) with door the signed halfword at
//     object+76; 6.0 is added to the high corner's z; objectFindInBox fills a 64-entry
//     list of object pointers from that box. The string tag "keyObject" is looked up on
//     this object (findTagString, index 0).
//  2. "Pressed" is decided by scanning the list up to its first null pointer: objects whose
//     flag word at +12 has bit 0 set are skipped; the first other object counts when this
//     object has no keyObject tag, or when that object's "name" tag exists and equals the
//     keyObject tag (strcmp is zero). The scan stops at the first one that counts.
//  3. On the signed byte at object+80 (call it the state):
//     - state 0, pressed: the object is added to the run list; eRand picks 1 or 2 from its
//       low bit; the sequence "game_floorplt_dprss%d.anm" is found in the "fx.lmp" lump,
//       played on the object at rate 1.0; the state becomes 1; step 4 runs.
//     - state 0, not pressed: if the float at +84 is positive, it is lowered by 0.25 and
//       worldMoveDoor(world, door, (0, 0, -the new value), 0) and then
//       worldSetDoorHeight(world, door, (float)halfword at +78 - the new value) are
//       called, and if the new value is not positive the object is added to the medium
//       run list. (A non-positive value at entry does nothing.)
//     - state 1, not pressed: the state becomes 0; step 4 runs.
//     - state 1, pressed: if the float at +84 is below 1.0, it is raised by 0.25, the same
//       worldMoveDoor and worldSetDoorHeight calls are made, and if the new value is at
//       least 1.0 the object is added to the medium run list.
//     - any other state: nothing.
//  4. (only after a state change) let on be 1 if the state is now 1. With the integer tag
//     "onOnly" (findTagInt, index 0) zero, the argument is on, otherwise 1. With the integer
//     tag "offOnly" zero, objectDoMyActions(object, argument) is called with that
//     argument, otherwise with 0.
// The tag names, the sequence format, the lump name and the strings are the image's own
// text (at 0x470640, 0x485358, 0x470650, 0x485380, 0x4853a0, 0x4853a8); this source
// holds its own copies. The sprintf buffer is 64 bytes in the image's frame.
// Unknown: what the float at +84 and the halfword at +78 measure, and what the actions
// run by objectDoMyActions do.

extern "C" {
struct Point3 { float x, y, z; };
void worldFindDoorBoundingBox__FP12_worldHeaderiP6Point3T2(void* world, int index, Point3* low, Point3* high);
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** out, int capacity);
char* findTagString__10GameObjectPci(void* object, const char* tag, int index);
int findTagInt__10GameObjectPci(void* object, const char* tag, int index);
int strcmp(const char* a, const char* b);
void objectAddToRunList__FP10GameObject(void* object);
void objectAddToMediumRunList__FP10GameObject(void* object);
int eRand__10GameObject(void* object);
int sprintf(char* destination, const char* format, ...);
char* lumpFindResource__FPcT0(const char* lump, const char* name);
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float rate);
int worldMoveDoor__FP12_worldHeaderiG6Point3s(void* world, int index, Point3 point, short extra);
int worldSetDoorHeight__FP12_worldHeaderif(void* world, int index, float height);
void objectDoMyActions__FP10GameObjecti(void* object, int on);
extern int world;
}

typedef unsigned Word __attribute__((may_alias));
typedef short Half __attribute__((may_alias));
typedef float Single __attribute__((may_alias));

static inline void* world_header() { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(world)); }

static void slide_door(unsigned char* object) {
    Point3 point = {0.0f, 0.0f, -*reinterpret_cast<Single*>(object + 84)};
    worldMoveDoor__FP12_worldHeaderiG6Point3s(world_header(), *reinterpret_cast<Half*>(object + 76), point, 0);
    worldSetDoorHeight__FP12_worldHeaderif(world_header(), *reinterpret_cast<Half*>(object + 76),
                                           static_cast<float>(*reinterpret_cast<Half*>(object + 78)) - *reinterpret_cast<Single*>(object + 84));
}

extern "C" void msg_run__11FloorSwitch(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    void* found[64];
    Point3 low, high;
    bool pressed = false;
    worldFindDoorBoundingBox__FP12_worldHeaderiP6Point3T2(world_header(), *reinterpret_cast<Half*>(object + 76), &low, &high);
    high.z = high.z + 6.0f;
    objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, found, 64);
    char* key = findTagString__10GameObjectPci(self, "keyObject", 0);
    for (int i = 0; found[i] != 0; ++i) {
        void* other = found[i];
        if ((*reinterpret_cast<Word*>(static_cast<unsigned char*>(other) + 12) & 1) != 0) continue;
        if (key == 0) {
            pressed = true;
            break;
        }
        if (findTagString__10GameObjectPci(other, "name", 0) != 0 &&
            strcmp(findTagString__10GameObjectPci(other, "name", 0), key) == 0) {
            pressed = true;
            break;
        }
    }
    bool changed = false;
    signed char state = static_cast<signed char>(object[80]);
    if (state == 0) {
        if (pressed) {
            changed = true;
            objectAddToRunList__FP10GameObject(self);
            char name[64];
            sprintf(name, "game_floorplt_dprss%d.anm", (eRand__10GameObject(self) & 1) + 1);
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("fx.lmp", name), self, 1.0f);
            object[80] = 1;
        } else if (*reinterpret_cast<Single*>(object + 84) > 0.0f) {
            *reinterpret_cast<Single*>(object + 84) = *reinterpret_cast<Single*>(object + 84) - 0.25f;
            slide_door(object);
            if (*reinterpret_cast<Single*>(object + 84) <= 0.0f) objectAddToMediumRunList__FP10GameObject(self);
        }
    } else if (state == 1) {
        if (!pressed) {
            object[80] = 0;
            changed = true;
        } else if (*reinterpret_cast<Single*>(object + 84) < 1.0f) {
            *reinterpret_cast<Single*>(object + 84) = *reinterpret_cast<Single*>(object + 84) + 0.25f;
            slide_door(object);
            if (1.0f <= *reinterpret_cast<Single*>(object + 84)) objectAddToMediumRunList__FP10GameObject(self);
        }
    }
    if (changed) {
        int on = static_cast<signed char>(object[80]) == 1;
        int only_on = findTagInt__10GameObjectPci(self, "onOnly", 0);
        int only_off = findTagInt__10GameObjectPci(self, "offOnly", 0);
        int argument = only_on == 0 ? on : 1;
        objectDoMyActions__FP10GameObjecti(self, only_off == 0 ? argument : 0);
    }
}
