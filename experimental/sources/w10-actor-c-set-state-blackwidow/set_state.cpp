// Observed setState__10BlackWidowi at 0x0020ac50, 1224 bytes. Offsets are into the object; A is
// the AnimationState embedded at +288 and T is the table pointed to by the 32-bit pointer at +36
// (its words are animation headers, from the callees' declared parameter types).
// The new state is held in a local; Creature::setStateProcessing(object, &local) runs first and a
// nonzero result returns at once. Otherwise: unless the local is 3, the float at +176 becomes
// 140.0; then a switch on the local (values above 16, and 2, 4, 7, 11, 12, 16, do nothing more).
// roll(n) below is (unsigned) eRand(object) % n.
//   0: r = roll(60); word at +256 = word at +808 + r + 120; animAddTransitionTo(A, T[116], 0.3, 3).
//   1: resetRouting; getSmartRandomDestination fills a Point3 (object, 200), copied to +272;
//      animAddTransitionTo(A, T[140], 0.3, 3).
//   3: r = roll(180); float at +176 = 140.0; resetRouting; word at +960 = word at +808 + r + 180;
//      animAddTransitionTo(A, T[140], 0.3, 3).
//   5: unless the halfword at +572 is 16: animAddTransitionTo(A, T[124], 0.1, 3).
//   6: animAddTransitionTo(A, T[124], 0.1, 3); animAddOneShot(A, T[52], 0.01, 0.01, 8); then X.
//  15: word at +964 = 1; animAddTransitionTo(A, T[124], 0.1, 3); animAddOneShot(A, T[36], 0.01,
//      0.1, 8); then X.   X: word at +256 = roll(15) + 15.
//  13: Web::reset(object + 976); animAddTransitionTo(A, T[124], 0.1, 3);
//      animAddOneShot(A, T[132], 0.1, 0.1, 8); word at +256 = 0.
//  14: animAddTransitionTo(A, T[60], 0.1, 3); word at +960 = word at +808 + 300.
//   9: unless the halfword at +572 is 16: animAddTransitionTo(A, T[116], 0.3, 3); else the
//      local becomes 16.
//   8: with cur the halfword at +572: when cur is 9 or 16, or cur is 6 with bit 3 of the
//      halfword at +294, the local becomes cur; else animAddOneShot(A, T[108], 0.1, 0.1, 16).
//  10: word at +964 = 0; loseBodyPart(object, roll & 7) with an unreduced eRand roll; a second
//      roll, and when (unsigned) roll % 3 is zero loseBodyPart(object, 8); dropTreasure(object,
//      copy of the +16 point, signed halfword +56, 36.0, 2.0, 1.6, 0.6, 1.4, 0.25, the text
//      "venomsac"); a third roll picks the header T[68 + 8 * (roll & 3)] for
//      animAddTransitionTo(A, header, 0.3, 6); when bit 1 of the word at +12 is set,
//      animSetToEnd(A); then the object's virtual function at vtable byte offset 192 is called.
// Finally the low 16 bits of the local are stored in the halfword at +572 and, unless the
// (signed) value is 16, the 64-bit value at +160 loses bit 11 and, unless the value is also 10,
// the word at +12 loses bit 0.
// Returns nothing. Unknown: what the state numbers, +176, +256, +960 and +964 mean.
// The host check defines NORRATH_W10AC_HOST so that 32-bit address words map to host memory.
extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int distance);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float time, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animSetToEnd__FP14AnimationState(void* state);
void reset__3Web(void* web);
void loseBodyPart__10BlackWidowi(void* self, int part);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const void* position, int yaw, float a, float b, float c,
                                               float d, float e, float f, const char* text);
int eRand__10GameObject(void* object);
#if defined(NORRATH_W10AC_HOST)
extern unsigned char* norrath_w10ac_host_base;
void* norrath_w10ac_host_function(unsigned word);
#endif
}

namespace {
typedef unsigned long long DWord __attribute__((may_alias));
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
short read_half(const unsigned char* p) { return static_cast<short>(p[0] | (p[1] << 8)); }
void write_float(unsigned char* p, float value) {
    union { unsigned u; float f; } v;
    v.f = value;
    write_word(p, v.u);
}
#if defined(NORRATH_W10AC_HOST)
// Host check only: pointer words are offsets from a base chosen by the check.
unsigned char* address_of(unsigned word) { return norrath_w10ac_host_base + word; }
void* function_of(unsigned word) { return norrath_w10ac_host_function(word); }
#else
unsigned char* address_of(unsigned word) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
}
void* function_of(unsigned word) { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word)); }
#endif
void copy_bytes(unsigned char* dest, const unsigned char* src, unsigned count) {
    for (unsigned i = 0; i < count; ++i) dest[i] = src[i];
}
void call_slot(unsigned char* target, unsigned byte_offset) {
    const unsigned char* entry = address_of(read_word(target + 72u)) + byte_offset;
    typedef void (*Function)(void*);
    Function function = reinterpret_cast<Function>(function_of(read_word(entry + 4)));
    function(target + read_half(entry));
}
unsigned roll(unsigned char* object, unsigned modulus) {
    return static_cast<unsigned>(eRand__10GameObject(object)) % modulus;
}
unsigned char* header_at(unsigned char* object, unsigned byte_offset) {
    return address_of(read_word(address_of(read_word(object + 36u)) + byte_offset));
}
}

extern "C" void setState__10BlackWidowi(void* object_storage, int new_state) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    int state = new_state;
    if (setStateProcessing__8CreaturePi(object, &state) != 0) return;
    if (state != 3) write_float(object + 176u, 140.0f);
    unsigned char* animation = object + 288;
    if (static_cast<unsigned>(state) < 17u) {
        switch (state) {
        case 0: {
            unsigned r = roll(object, 60);
            write_word(object + 256u, read_word(object + 808u) + r + 120u);
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header_at(object, 116), 0.3f, 3);
            break;
        }
        case 1: {
            resetRouting__8Creature(object);
            unsigned char destination[12];
            getSmartRandomDestination__8Creaturei(destination, object, 200);
            unsigned char copy[12];
            copy_bytes(copy, destination, 12u);
            copy_bytes(object + 272u, copy, 12u);
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header_at(object, 140), 0.3f, 3);
            break;
        }
        case 3: {
            unsigned r = roll(object, 180);
            write_float(object + 176u, 140.0f);
            write_word(object + 960u, read_word(object + 808u) + r + 180u);
            resetRouting__8Creature(object);
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header_at(object, 140), 0.3f, 3);
            break;
        }
        case 5:
            if (read_half(object + 572u) != 16)
                animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header_at(object, 124), 0.1f,
                                                                            3);
            break;
        case 6:
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header_at(object, 124), 0.1f, 3);
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, header_at(object, 52), 0.01f, 0.01f, 8);
            write_word(object + 256u, roll(object, 15) + 15u);
            break;
        case 15:
            write_word(object + 964u, 1u);
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header_at(object, 124), 0.1f, 3);
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, header_at(object, 36), 0.01f, 0.1f, 8);
            write_word(object + 256u, roll(object, 15) + 15u);
            break;
        case 13:
            reset__3Web(object + 976);
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header_at(object, 124), 0.1f, 3);
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, header_at(object, 132), 0.1f, 0.1f, 8);
            write_word(object + 256u, 0u);
            break;
        case 14:
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header_at(object, 60), 0.1f, 3);
            write_word(object + 960u, read_word(object + 808u) + 300u);
            break;
        case 9:
            if (read_half(object + 572u) != 16)
                animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header_at(object, 116), 0.3f,
                                                                            3);
            else
                state = 16;
            break;
        case 8: {
            short current = read_half(object + 572u);
            if (current == 9 || current == 16 || (current == 6 && (read_half(object + 294u) & 8) != 0))
                state = current;
            else
                animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, header_at(object, 108), 0.1f, 0.1f,
                                                                        16);
            break;
        }
        case 10: {
            write_word(object + 964u, 0u);
            loseBodyPart__10BlackWidowi(object, eRand__10GameObject(object) & 7);
            if (roll(object, 3) == 0u) loseBodyPart__10BlackWidowi(object, 8);
            unsigned char position[12];
            copy_bytes(position, object + 16u, 12u);
            dropTreasure__8CreatureG6Point3iffffffPc(object, position, read_half(object + 56u), 36.0f, 2.0f, 1.6f, 0.6f,
                                                      1.4f, 0.25f, "venomsac");
            unsigned pick = static_cast<unsigned>(eRand__10GameObject(object)) & 3u;
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header_at(object, 68 + pick * 8u),
                                                                        0.3f, 6);
            if ((read_word(object + 12u) & 2u) != 0u) animSetToEnd__FP14AnimationState(animation);
            call_slot(object, 192);
            break;
        }
        default:
            break;
        }
    }
    object[572] = static_cast<unsigned char>(state);
    object[573] = static_cast<unsigned char>(static_cast<unsigned>(state) >> 8);
    short low = static_cast<short>(state);
    if (low != 16) {
        *reinterpret_cast<DWord*>(object + 160) &= 0xfffffffffffff7ffull;
        if (low != 10) write_word(object + 12u, read_word(object + 12u) & 0xfffffffeu);
    }
}
