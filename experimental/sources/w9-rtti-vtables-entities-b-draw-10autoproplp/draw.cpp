// Observed msg_draw__10AutoPropLP at 0x224300, 976 bytes. For each of the four pointer
// words at object+424 (index i = 0..3), skipping null ones and ones whose words at +80, +84
// and +88 are all zero (unlike msg_run__10AutoPropLP there is no guard on the word at
// +280):
//  - the shared per-entry update in ../w9-rtti-vtables-entities-b-autoproplp-common;
//  - if bit 0 of the i-th word of the array at object+440 is set: n = modelGetNmShadowPoints(
//    the model word at +144); for j = 0..n-1 (nothing when n <= 0),
//    modelGetShadowPos(model, animation state at object+152, a copy of the three words
//    at +16, the halfword at +56 minus 16384, j, point, scratch, null matrix, 1.0) fills a
//    local point, and the entry's table slot at byte offset 136 is called with the
//    entry's adjusted address, that point, 0 and a final argument that is 1 for j = 0,
//    else 3 for j = n-1, else 2;
//  - otherwise modelGetCharAttachmentPos(model, a copy of the three words at +16, the
//    halfword at +56 minus 16385, the animation state at +152, i, out1, out2, 1.0) fills two
//    local points and the slot at offset 136 is called with the adjusted address, the
//    first point, then the second point's address if bit 1 of the flag word is set (else 0),
//    and 0.
// Last, msg_draw__10AutoObject(object) is called (defined elsewhere).
// Unknown: what the entries are, what the slot does, what the shadow points are.

#include "../w9-rtti-vtables-entities-b-autoproplp-common/entries.hpp"

extern "C" {
struct Point3 { float x, y, z; };
void msg_draw__10AutoObject(void* object);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
int modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* state, Point3 position, short angle, int index, Point3* out, float* scratch, void* matrix, float scale);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, Point3 position, short angle, void* state, int index, Point3* out, Point3* other, float scale);
}

using w8avb::UWord;
using w8avb::Half;

static inline void* word_pointer(unsigned word) { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word)); }

extern "C" void msg_draw__10AutoPropLP(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    for (int i = 0; i < 4; ++i) {
        unsigned char* entry = autoproplp::entry_at(object, i);
        if (entry == 0 || autoproplp::entry_idle(entry)) continue;
        autoproplp::update_entry(object, entry, i);
        if ((*reinterpret_cast<UWord*>(object + 440 + 4 * i) & 1) != 0) {
            const int count = modelGetNmShadowPoints__FP12_modelHeader(word_pointer(*reinterpret_cast<UWord*>(object + 144)));
            for (int j = 0; j < count; ++j) {
                Point3 point;
                float scratch[4];
                Point3 position = *reinterpret_cast<const Point3*>(object + 16);
                modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
                    word_pointer(*reinterpret_cast<UWord*>(object + 144)), object + 152, position,
                    static_cast<short>(*reinterpret_cast<unsigned short*>(object + 56) - 16384), j, &point, scratch, 0, 1.0f);
                const int kind = j == 0 ? 1 : (j == count - 1 ? 3 : 2);
                const unsigned char* slot = w8avb::address_of(*reinterpret_cast<UWord*>(entry + 72)) + 136;
                typedef void (*Function)(void*, Point3, void*, int);
                Function function = reinterpret_cast<Function>(w8avb::function_of(*reinterpret_cast<const UWord*>(slot + 4)));
                function(entry + *reinterpret_cast<const Half*>(slot), point, 0, kind);
            }
        } else {
            Point3 first, second;
            Point3 position = *reinterpret_cast<const Point3*>(object + 16);
            modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
                word_pointer(*reinterpret_cast<UWord*>(object + 144)), position,
                static_cast<short>(*reinterpret_cast<unsigned short*>(object + 56) - 16385), object + 152, i, &first, &second, 1.0f);
            const unsigned char* slot = w8avb::address_of(*reinterpret_cast<UWord*>(entry + 72)) + 136;
            typedef void (*Function)(void*, Point3, Point3*, int);
            Function function = reinterpret_cast<Function>(w8avb::function_of(*reinterpret_cast<const UWord*>(slot + 4)));
            function(entry + *reinterpret_cast<const Half*>(slot), first,
                     (*reinterpret_cast<UWord*>(object + 440 + 4 * i) & 2) != 0 ? &second : 0, 0);
        }
    }
    msg_draw__10AutoObject(self);
}
