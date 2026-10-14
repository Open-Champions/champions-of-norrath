// Observed msg_run__7AntTrap at 0x225708, 516 bytes.
// First msg_run__8AutoProp(object) runs (defined elsewhere). Then:
//  - If bit 0 of the word at object+192 is set, a box is built around the point at
//    object+16 with the float at +28 as the half-size on every axis, objectFindInBox fills
//    a 128-entry list of object pointers from it, and every listed object whose flag
//    word at +12 has bit 3 (value 8) clear is passed to gameAttackResolver(listed,
//    this, d, d, 10000, 0, 0, 8, null, false) with d = 2 * gameDifficultyScale().
//  - If frameNm is a multiple of 30 (signed remainder) and bit 7 (value 0x80) of the
//    halfword at +158 is set, the same box search runs again, and for every listed
//    object whose flag word has bit 5 (0x20) set and bit 3 (0x8) clear it calls
//    animAddTransitionTo(object+152, the word at +408, the float word 0x3dcccccc, 6).
// The list ends at the first null pointer. The two stack lists are 128 entries each;
// like the image, nothing here bounds the count objectFindInBox writes beyond the
// capacity it is given.
// Unknown: what the +192 and +158 bits mean, what the +408 word points at, and what the
// resolver does with the numbers.

extern "C" {
struct Point3 { float x, y, z; };
void msg_run__8AutoProp(void* object);
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** out, int capacity);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(
    void* target, void* source, float a, float b, int c, int d, int e, int f, void* description, bool flag);
float gameDifficultyScale__Fv();
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int mode);
extern int frameNm;
}

typedef unsigned Word __attribute__((may_alias));
typedef unsigned short UHalf __attribute__((may_alias));
typedef float Single __attribute__((may_alias));

static inline float single_from_bits(unsigned bits) { return __builtin_bit_cast(float, bits); }

extern "C" void msg_run__7AntTrap(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    msg_run__8AutoProp(self);
    if ((*reinterpret_cast<Word*>(object + 192) & 1) != 0) {
        void* found[128];
        const float reach = *reinterpret_cast<Single*>(object + 28);
        const float x = *reinterpret_cast<Single*>(object + 16);
        const float y = *reinterpret_cast<Single*>(object + 20);
        const float z = *reinterpret_cast<Single*>(object + 24);
        Point3 low = {x - reach, y - reach, z - reach};
        Point3 high = {x + reach, y + reach, z + reach};
        objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, found, 128);
        for (int i = 0; found[i] != 0; ++i) {
            void* other = found[i];
            if ((*reinterpret_cast<Word*>(static_cast<unsigned char*>(other) + 12) & 8) == 0) {
                float damage = gameDifficultyScale__Fv();
                damage = damage + damage;
                gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(other, self, damage, damage, 10000, 0, 0, 8, 0, false);
            }
        }
    }
    if (frameNm % 30 == 0 && (*reinterpret_cast<UHalf*>(object + 158) & 0x80) != 0) {
        void* found[128];
        const float reach = *reinterpret_cast<Single*>(object + 28);
        const float x = *reinterpret_cast<Single*>(object + 16);
        const float y = *reinterpret_cast<Single*>(object + 20);
        const float z = *reinterpret_cast<Single*>(object + 24);
        Point3 low = {x - reach, y - reach, z - reach};
        Point3 high = {x + reach, y + reach, z + reach};
        objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, found, 128);
        for (int i = 0; found[i] != 0; ++i) {
            unsigned flags = *reinterpret_cast<Word*>(static_cast<unsigned char*>(found[i]) + 12);
            if ((flags & 0x20) != 0 && (flags & 8) == 0)
                animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(
                    object + 152, reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<Word*>(object + 408))),
                    single_from_bits(0x3dccccccu), 6);
        }
    }
}
