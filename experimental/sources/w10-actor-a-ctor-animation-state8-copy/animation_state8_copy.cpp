// Observed __15AnimationState8RC15AnimationState8 at 0x00163010, 588 bytes: the copy constructor of
// AnimationState8. Arguments: the new object and the source object.
//  - Byte +5 of the new object is set to 1, then the first 264 bytes (an 8-byte header and eight
//    32-byte slots) are copied from the source, replacing that byte. Word +0 is then cleared.
//  - The signed byte at +5 (taken from the copy) is a slot count. For each slot index i below it:
//    the source's word at +12+32*i is a pointer to a 3868-byte record, or zero. If it is zero,
//    or the halfword at +36+32*i of the new object has none of the bits 0x4800 set, the new
//    object's word at +12+32*i becomes zero. Otherwise a record is taken from getFreeFrameState,
//    stored at that word, and filled: its word +20 gets the address of the new object's slot
//    (object+8+32*i); words +8, +0, +4 are copied from the source record in that order; then bytes
//    +28 through +3867 are copied. The count is read again after each slot.
// The copy order is observed from the image; the source and the new object are assumed not to
// overlap. Inferred: the 264-byte block is a plain struct copy, the 3868-byte records are
// per-slot state. Unknown: what the slots, flags and records mean. Nothing here ran on the EE.
extern "C" {
void* getFreeFrameState__Fv();
}

namespace {
const unsigned kRecordBytes = 3868;
const unsigned kRecordBodyOffset = 28;

inline __attribute__((always_inline))
void copy_words(unsigned* to, const unsigned* from, unsigned bytes) {
    for (unsigned i = 0; i < bytes / 4; ++i) to[i] = from[i];
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void __15AnimationState8RC15AnimationState8(void* self, const void* source) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const unsigned char* from = static_cast<const unsigned char*>(source);
    object[5] = 1;
    copy_words(reinterpret_cast<unsigned*>(object), reinterpret_cast<const unsigned*>(from), 264);
    *reinterpret_cast<unsigned*>(object) = 0;
    for (int i = 0; i < static_cast<signed char>(object[5]); ++i) {
        unsigned* slotRecord = reinterpret_cast<unsigned*>(object + 12 + 32 * i);
        const unsigned sourceRecord = *reinterpret_cast<const unsigned*>(from + 12 + 32 * i);
        if (sourceRecord == 0 || (*reinterpret_cast<unsigned short*>(object + 36 + 32 * i) & 0x4800) == 0) {
            *slotRecord = 0;
        } else {
            unsigned char* record = static_cast<unsigned char*>(getFreeFrameState__Fv());
            const unsigned char* original = reinterpret_cast<const unsigned char*>(static_cast<__UINTPTR_TYPE__>(sourceRecord));
            *slotRecord = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(record));
            *reinterpret_cast<unsigned*>(record + 20) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(object + 8 + 32 * i));
            *reinterpret_cast<unsigned*>(record + 8) = *reinterpret_cast<const unsigned*>(original + 8);
            *reinterpret_cast<unsigned*>(record + 0) = *reinterpret_cast<const unsigned*>(original + 0);
            *reinterpret_cast<unsigned*>(record + 4) = *reinterpret_cast<const unsigned*>(original + 4);
            copy_words(reinterpret_cast<unsigned*>(record + kRecordBodyOffset),
                       reinterpret_cast<const unsigned*>(original + kRecordBodyOffset), kRecordBytes - kRecordBodyOffset);
        }
    }
}
