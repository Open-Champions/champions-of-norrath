#include "../w8-actor-virtuals-c-bytes/bytes.hpp"
// Observed msg_trigger__8AutoPropi at 0x224ec8, 256 bytes.
// Returns unless bit 0x40 of the word at object+284 is set and the word at
// object+288 is nonzero. Then the virtual slot at byte offset 136 of the table at
// object+72 is called with the adjusted object. (The image leaves the incoming
// integer argument in the second argument register, so the slot is also given it
// by accident; the declared slot function takes only the object.)
// Then, with the name string at object+76:
//  - when the state word at object+376 is 1 and the name equals "gothtrap"
//    (case-insensitive), the resource "cs_wallcrumble.anm" in the lump
//    "gothtrap.lmp" is played as a sequence at gain 1.0 with this object, and
//    that is all;
//  - otherwise, when the name equals "fayactuator", the resource
//    "fayacthack.anm" in the lump "fayswitc.lmp" is played the same way.
// Inferred: norrath_lump_find_resource is lumpFindResource and
// norrath_sound_play_sequence is the soundPlaySequence body that takes an object
// (0x24d308), as in the Gold collision source. strcasecmp is the libc routine.
// Unknown: what slot 136 does (the image's table names it advanceState for
// AutoProp) and the meaning of the two flag fields.
extern "C" int strcasecmp(const char* left, const char* right);
extern "C" void* norrath_lump_find_resource(char* lump_name, char* resource_name);
extern "C" int norrath_sound_play_sequence(void* animation_header, void* object, float gain);

namespace {
typedef void (*SlotFunction)(void*);
}

extern "C" void msg_trigger__8AutoPropi(void* self, int value) {
    (void)value;
    using namespace w8c;
    const Address base = address(self);
    if ((read_word(base + 284u) & 0x40u) == 0u) return;
    if (read_word(base + 288u) == 0u) return;
    void* adjusted;
    SlotFunction function = slot_function<SlotFunction>(self, 136u, &adjusted);
    function(adjusted);
    const char* name = reinterpret_cast<const char*>(static_cast<unsigned char*>(self) + 76);
    if (read_word(base + 376u) == 1u) {
        static const char gothtrap[] = {'g', 'o', 't', 'h', 't', 'r', 'a', 'p', '\0'};
        if (strcasecmp(name, gothtrap) == 0) {
            static char lump[] = {'g', 'o', 't', 'h', 't', 'r', 'a', 'p', '.', 'l', 'm', 'p', '\0'};
            static char resource[] = {'c', 's', '_', 'w', 'a', 'l', 'l', 'c', 'r', 'u', 'm', 'b', 'l', 'e', '.',
                                      'a', 'n', 'm', '\0'};
            norrath_sound_play_sequence(norrath_lump_find_resource(lump, resource), self, 1.0f);
            return;
        }
    }
    static const char actuator[] = {'f', 'a', 'y', 'a', 'c', 't', 'u', 'a', 't', 'o', 'r', '\0'};
    if (strcasecmp(name, actuator) == 0) {
        static char lump[] = {'f', 'a', 'y', 's', 'w', 'i', 't', 'c', '.', 'l', 'm', 'p', '\0'};
        static char resource[] = {'f', 'a', 'y', 'a', 'c', 't', 'h', 'a', 'c', 'k', '.', 'a', 'n', 'm', '\0'};
        norrath_sound_play_sequence(norrath_lump_find_resource(lump, resource), self, 1.0f);
    }
}
