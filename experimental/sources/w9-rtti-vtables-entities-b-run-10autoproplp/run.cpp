// Observed msg_run__10AutoPropLP at 0x2246d0, 636 bytes. First msg_run__8AutoProp(object)
// runs (defined elsewhere). Then, if the word at object+280 is nonzero, for each of the
// four pointer words at object+424 (index i = 0..3), skipping null ones and ones whose
// words at +80, +84 and +88 are all zero:
//  - the shared per-entry update in ../w9-rtti-vtables-entities-b-autoproplp-common;
//  - if bit 0 of the i-th word of the array at object+440 is set, the entry's table slot at
//    byte offset 144 is called with the entry's adjusted address and a pointer to a copy of
//    the three words at object+16;
//  - otherwise modelGetCharAttachmentPos(model = the word at +144, a copy of the three
//    words at +16, the halfword at +56 minus 16385, the animation state at object+152,
//    i, out1, out2, 1.0) fills two local points, and the same slot at offset 144 is
//    called with the entry's adjusted address and the first point.
// Unknown: what the entries are, what the slot does, what the two output points are.

#include "../w9-rtti-vtables-entities-b-autoproplp-common/entries.hpp"

extern "C" {
struct Point3 { float x, y, z; };
void msg_run__8AutoProp(void* object);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, Point3 position, short angle, void* state, int index, Point3* out, Point3* other, float scale);
}

#pragma pack(push, 4)
struct Triple { unsigned a, b, c; };
#pragma pack(pop)

using w8avb::UWord;
using w8avb::Half;

extern "C" void msg_run__10AutoPropLP(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    msg_run__8AutoProp(self);
    if (*reinterpret_cast<UWord*>(object + 280) == 0) return;
    for (int i = 0; i < 4; ++i) {
        unsigned char* entry = autoproplp::entry_at(object, i);
        if (entry == 0 || autoproplp::entry_idle(entry)) continue;
        autoproplp::update_entry(object, entry, i);
        if ((*reinterpret_cast<UWord*>(object + 440 + 4 * i) & 1) != 0) {
            const unsigned char* slot = w8avb::address_of(*reinterpret_cast<UWord*>(entry + 72)) + 144;
            typedef void (*Function)(void*, Triple);
            Function function = reinterpret_cast<Function>(w8avb::function_of(*reinterpret_cast<const UWord*>(slot + 4)));
            Triple copy = *reinterpret_cast<const Triple*>(object + 16);
            function(entry + *reinterpret_cast<const Half*>(slot), copy);
        } else {
            Point3 first, second;
            Point3 position = *reinterpret_cast<const Point3*>(object + 16);
            modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
                reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<UWord*>(object + 144))), position,
                static_cast<short>(*reinterpret_cast<unsigned short*>(object + 56) - 16385), object + 152, i, &first,
                &second, 1.0f);
            const unsigned char* slot = w8avb::address_of(*reinterpret_cast<UWord*>(entry + 72)) + 144;
            typedef void (*Function)(void*, Point3);
            Function function = reinterpret_cast<Function>(w8avb::function_of(*reinterpret_cast<const UWord*>(slot + 4)));
            function(entry + *reinterpret_cast<const Half*>(slot), first);
        }
    }
}
