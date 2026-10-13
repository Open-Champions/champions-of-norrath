// Observed msg_poke__9DoorSwingi at 0x212950, 952 bytes. The int is a player index, or -10.
// 1. Unless the index is -10: if a string tag "key" exists on the object (findTagString,
//    index 0), the tag is fetched again and passed to scriptStart; a zero result means the
//    door stays shut, and then: with a string tag "sound" present, only if it equals "jail"
//    the sequence "door_jail_locked.anm" of the "fx.lmp" lump is played on the object at
//    rate 1.0 (a "sound" tag with any other value plays nothing); with no "sound" tag
//    "door_wd_locked.anm" is played; then playerPlaySound(index, 7) and a 180-frame info
//    message with the text-table entry "DoorLocked" (player argument -1, last argument 0)
//    end the call.
// 2. Otherwise the door opens: bit 20 (0x100000) of the flag word at object+12 is cleared
//    and the object is added to the run list. Then, only if the signed halfword at +76 (the
//    door index) is not negative:
//    - the float at +84 becomes 600.0;
//    - the same "sound" tag logic plays "door_jail_open.anm" (tag equal to "jail") or, with
//      no tag, "door_wd_open.anm";
//    - worldSetDoorCollision(world, door, 0);
//    - the door's pivot is computed from the world's door table (28-byte entries at the
//      pointer at world+84, 60-byte records at the pointer at world+36): with x, y, z the
//      ints at entry +8, +12 and +16 times 0.25 and c, s the halfwords at entry +20 and +22
//      times the float word 0x38000100, the point is (x*c - y*s, x*s + y*c) when bit 6
//      (0x40) of the word at record+36 is clear, else (x*c + y*s, x*s - y*c), the record being
//      the one numbered by the entry's first word;
//    - the byte at +88 becomes 1 if (player position - point) . (c, s) is negative, else
//      255 (-1), the player being player[index], or player[0] when the index is -10, and
//      is negated if bit 6 above is set;
//    - bit 0 of the flag word at +12 is set.
// The strings sit in the image's string areas (0x485368, 0x485370, 0x485378, 0x485380,
// 0x4705a0, 0x4705b8, 0x4705e0, 0x4705f8, 0x4705d0); this source holds its own copies.
// Unknown: what the pivot, the byte at +88 and the float at +84 are used for, and what
// the table fields really are.

extern "C" {
char* findTagString__10GameObjectPci(void* object, const char* tag, int index);
int scriptStart__FPc(char* script);
int strcmp(const char* a, const char* b);
char* lumpFindResource__FPcT0(const char* lump, const char* name);
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float rate);
void playerPlaySound__Fii(int index, int sound);
unsigned short* textGetString__FPc(const char* key);
void hudSetInfoMessage__FPUsiic(unsigned short* text, int duration, int player, char extra);
void objectAddToRunList__FP10GameObject(void* object);
void worldSetDoorCollision__FP12_worldHeaderii(void* world, int index, int enable);
extern int world;
extern unsigned char player[];
}

#include "../w8-actor-virtuals-b-common/virtual_call.hpp"

using w8avb::UWord;
using w8avb::Half;
typedef float Single __attribute__((may_alias));
typedef int SWord __attribute__((may_alias));

static inline void* world_header() { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(world)); }
static inline const unsigned char* world_bytes() { return w8avb::address_of(static_cast<unsigned>(world)); }
static inline float single_from_bits(unsigned bits) { return __builtin_bit_cast(float, bits); }

static void play_named(void* self, const char* name) {
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("fx.lmp", name), self, 1.0f);
}

extern "C" void msg_poke__9DoorSwingi(void* self, int who) {
    unsigned char* object = static_cast<unsigned char*>(self);
    if (who != -10 && findTagString__10GameObjectPci(self, "key", 0) != 0) {
        if (scriptStart__FPc(findTagString__10GameObjectPci(self, "key", 0)) == 0) {
            char* sound = findTagString__10GameObjectPci(self, "sound", 0);
            if (sound != 0) {
                if (strcmp(sound, "jail") == 0) play_named(self, "door_jail_locked.anm");
            } else {
                play_named(self, "door_wd_locked.anm");
            }
            playerPlaySound__Fii(who, 7);
            hudSetInfoMessage__FPUsiic(textGetString__FPc("DoorLocked"), 180, -1, 0);
            return;
        }
    }
    *reinterpret_cast<UWord*>(object + 12) &= 0xffefffffu;
    objectAddToRunList__FP10GameObject(self);
    if (*reinterpret_cast<Half*>(object + 76) < 0) return;
    *reinterpret_cast<Single*>(object + 84) = 600.0f;
    char* sound = findTagString__10GameObjectPci(self, "sound", 0);
    if (sound != 0) {
        if (strcmp(sound, "jail") == 0) play_named(self, "door_jail_open.anm");
    } else {
        play_named(self, "door_wd_open.anm");
    }
    worldSetDoorCollision__FP12_worldHeaderii(world_header(), *reinterpret_cast<Half*>(object + 76), 0);

    const int door = *reinterpret_cast<Half*>(object + 76);
    const unsigned char* entry = w8avb::address_of(*reinterpret_cast<const UWord*>(world_bytes() + 84)) + 28 * door;
    const unsigned char* record = w8avb::address_of(*reinterpret_cast<const UWord*>(world_bytes() + 36)) +
                                  60 * *reinterpret_cast<const SWord*>(entry);
    const float k = single_from_bits(0x38000100u);
    const float x = static_cast<float>(*reinterpret_cast<const SWord*>(entry + 8)) * 0.25f;
    const float y = static_cast<float>(*reinterpret_cast<const SWord*>(entry + 12)) * 0.25f;
    const float sine = static_cast<float>(*reinterpret_cast<const Half*>(entry + 22)) * k;
    const float cosine = static_cast<float>(*reinterpret_cast<const Half*>(entry + 20)) * k;
    float px, py;
    if ((*reinterpret_cast<const UWord*>(record + 36) & 0x40) != 0) {
        py = x * sine - y * cosine;
        px = x * cosine + y * sine;
    } else {
        py = x * sine + y * cosine;
        px = x * cosine - y * sine;
    }
    const int slot = who == -10 ? 0 : who;
    const unsigned char* who_object =
        w8avb::address_of(*reinterpret_cast<const UWord*>(player + 23984 * slot + 21872));
    const float dx = *reinterpret_cast<const Single*>(who_object + 16) - px;
    const float dy = *reinterpret_cast<const Single*>(who_object + 20) - py;
    object[88] = dx * cosine + dy * sine < 0.0f ? 1 : 0xff;
    {
        const int door_again = *reinterpret_cast<Half*>(object + 76);
        const unsigned char* entry_again = w8avb::address_of(*reinterpret_cast<const UWord*>(world_bytes() + 84)) + 28 * door_again;
        const unsigned char* record_again = w8avb::address_of(*reinterpret_cast<const UWord*>(world_bytes() + 36)) +
                                            60 * *reinterpret_cast<const SWord*>(entry_again);
        if ((*reinterpret_cast<const UWord*>(record_again + 36) & 0x40) != 0)
            object[88] = static_cast<unsigned char>(-static_cast<int>(object[88]));
    }
    *reinterpret_cast<UWord*>(object + 12) |= 1;
}
