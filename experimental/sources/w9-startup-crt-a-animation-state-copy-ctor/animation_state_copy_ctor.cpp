// Observed __14AnimationStateRC14AnimationState at 0x00162bf0, 460 bytes: the AnimationState copy constructor
// (destination first, source second). Observed layout: a 40-byte header, then up to the number in the signed byte at
// +5 of slots of 32 bytes starting at +8; within slot i the word at +4 (object +12 + 32*i) is a pointer to a frame
// state and the halfword at +28 (object +36 + 32*i) holds flag bits.
//  1. The 40 header bytes are copied from the source, then the word at +0 of the destination is cleared.
//  2. For slot 0 up to the count read from the destination (re-read from the destination's byte +5 each step), the
//     flags halfword is tested against 0x4800. With neither bit set the slot's frame-state pointer is set to zero.
//     Otherwise a frame state is obtained from getFreeFrameState__Fv (provider name
//     norrath_animation_frame_state_acquire) and stored in the slot; its word +20 receives the address of the slot
//     (object + 8 + 32*i); its words +8, +0 and +4 are copied, in that order, from the source's frame state; and its
//     bytes +28 up to +3868 (3840 bytes, 120 blocks of 32) are copied from the same offsets of the source's frame
//     state. The frame state's words +12 and +16 and its bytes +24..+27 are not written.
// Nothing is returned. The original has separate copy loops for 8-byte-aligned and unaligned pointers; both copy the
// same bytes. What the fields mean is not established here. Fresh compilation is not an original-byte match.
extern "C" void* norrath_animation_frame_state_acquire();

// Stored pointers are 32-bit words on the target. The host contract runs with 64-bit pointers and substitutes a
// handle table for the two conversions.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned norrath_host_word_for_pointer(const void* pointer);
extern "C" unsigned char* norrath_host_pointer_for_word(unsigned word);
#define NORRATH_POINTER_WORD(pointer) norrath_host_word_for_pointer(pointer)
#define NORRATH_WORD_POINTER(word) norrath_host_pointer_for_word(word)
#else
#define NORRATH_POINTER_WORD(pointer) static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer))
#define NORRATH_WORD_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

namespace {
void copy_block(unsigned char* destination, const unsigned char* source) {
    for (unsigned n = 0; n < 32; ++n) destination[n] = source[n];
}
}

extern "C" void __14AnimationStateRC14AnimationState(void* destination_storage, const void* source_storage) {
    unsigned char* destination = static_cast<unsigned char*>(destination_storage);
    const unsigned char* source = static_cast<const unsigned char*>(source_storage);
    for (unsigned n = 0; n < 40; ++n) destination[n] = source[n];
    *reinterpret_cast<unsigned*>(destination) = 0u;
    for (int index = 0; index < static_cast<signed char>(destination[5]); ++index) {
        unsigned char* slot = destination + 8 + 32 * index;
        unsigned* slot_state = reinterpret_cast<unsigned*>(slot + 4);
        const unsigned short flags = *reinterpret_cast<const unsigned short*>(slot + 28);
        if ((flags & 0x4800u) == 0u) {
            *slot_state = 0u;
            continue;
        }
        unsigned char* fresh = static_cast<unsigned char*>(norrath_animation_frame_state_acquire());
        *slot_state = NORRATH_POINTER_WORD(fresh);
        *reinterpret_cast<unsigned*>(fresh + 20) = NORRATH_POINTER_WORD(slot);
        const unsigned char* original = NORRATH_WORD_POINTER(*reinterpret_cast<const unsigned*>(source + 12 + 32 * index));
        *reinterpret_cast<unsigned*>(fresh + 8) = *reinterpret_cast<const unsigned*>(original + 8);
        *reinterpret_cast<unsigned*>(fresh) = *reinterpret_cast<const unsigned*>(original);
        *reinterpret_cast<unsigned*>(fresh + 4) = *reinterpret_cast<const unsigned*>(original + 4);
        for (unsigned offset = 28; offset < 3868; offset += 32) copy_block(fresh + offset, original + offset);
    }
}
