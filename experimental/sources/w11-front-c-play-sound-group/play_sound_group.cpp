// Observed playSoundGroup__FP15AnimationHeaderiiP6Point3 at 0x00160660, 464 bytes, one
// caller (animAdvanceAnimationTime). Arguments: an animation header, a first and an end
// index, and a position. The header holds a table of 28-byte event records at +56, a
// table of 8-byte sound entries at +52 (the word at +4 of an entry is what the sound
// call takes) and a byte at +60. For each index in first .. end-1:
//   * An event whose byte at +8 is 1 to 6 names a group (that signed byte). A local
//     mask of groups already played during this call is kept in a signed byte. If the
//     group's bit is set the event is skipped. Otherwise the events at later indexes
//     below end with the same signed group byte are counted (n, starting at 1), one step
//     of the shared generator is taken (state * 0x19660d + 0x3c6ef35f), and the event is
//     skipped when 1.0f / n < (state >> 16) * 2^-16. If it is not skipped the group's
//     bit is set in the mask.
//   * For an event that was not skipped, the signed halfword at +4 selects the action.
//     Zero or more: animPlaySound(entry word, position, the event record, header byte
//     +60). Below -1: if specialHandler is set, it is called with that code and a copy
//     of the 12-byte position. -1 does nothing.
// Inferred: bytes 1 to 6 are sound groups, one chosen with equal chance per group.
// Unknown: the record and header types, and the real meaning of the event bytes.
struct Point3 {
    float x, y, z;
};
// A host check defines NORRATH_HOST_ARENA, because a 64-bit host cannot keep a target
// pointer in a 4-byte record word; the record words then hold arena offsets.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* pointerAt(const unsigned char* p) {
    unsigned word = p[0] | (p[1] << 8) | (p[2] << 16) | (static_cast<unsigned>(p[3]) << 24);
    return word == 0 ? nullptr : norrath_host_pointer(word);
}
#else
static inline unsigned char* pointerAt(const unsigned char* p) {
    return *reinterpret_cast<unsigned char* const*>(p);
}
#endif
extern "C" {
extern int eRandState;
extern void (*specialHandler)(int code, Point3 position);
void animPlaySound__FPvP6Point3P9animSoundi(void* sound, Point3* position, void* event, int groupByte);
}

extern "C" void playSoundGroup__FP15AnimationHeaderiiP6Point3(unsigned char* header, int first, int end,
                                                             Point3* position) {
    signed char played = 0;
    for (int i = first; i < end; ++i) {
        unsigned char* events = pointerAt(header + 56);
        unsigned char* event = events + i * 28;
        unsigned type = event[8];
        if (type - 1u < 6u) {
            signed char group = static_cast<signed char>(type);
            if ((played >> group) & 1) continue;
            int count = 1;
            for (int j = i + 1; j < end; ++j)
                if (static_cast<signed char>(events[j * 28 + 8]) == group) ++count;
            unsigned state = static_cast<unsigned>(eRandState) * 0x19660du + 0x3c6ef35fu;
            eRandState = static_cast<int>(state);
            float chance = 1.0f / static_cast<float>(count);
            float roll = static_cast<float>(state >> 16) * (1.0f / 65536.0f);
            if (chance < roll) continue;
            played = static_cast<signed char>(played | (1 << event[8]));
        }
        short code = *reinterpret_cast<short*>(event + 4);
        if (code >= 0) {
            animPlaySound__FPvP6Point3P9animSoundi(pointerAt(pointerAt(header + 52) + code * 8 + 4), position, event,
                                                   header[60]);
        } else if (code < -1) {
            if (specialHandler != 0) specialHandler(code, *position);
        }
    }
}
