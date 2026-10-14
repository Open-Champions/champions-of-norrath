// Observed setState__6Badgeri at 0x001efaa8, 920 bytes. Offsets are into the object; A is the
// AnimationState embedded at +288 and T is the table pointed to by the 32-bit pointer at +36
// (its words are animation headers, from the callees' declared parameter types).
// The new state is held in a local; Creature::setStateProcessing(object, &local) runs first and a
// nonzero result returns without touching the object. Otherwise a switch on the local (values
// above 16 and 2, 4, 11, 12 do nothing more) runs, and at the end the low 16 bits of the local
// are stored in the halfword at +572. 0.1 is one float constant used by almost every case.
//   0: animAddTransitionTo(A, T[132], 0.1, 3); word at +256 = word at +808 + 180.
//   1: resetRouting(object); getSmartRandomDestination fills a Point3 (object, 200), copied to
//      +272; animAddTransitionTo(A, T[148], 0.1, 2051); animGlobalSync(A).
//   3: resetRouting; when the halfword at +572 is not one of 3, 6, 13, 14, 15, 16:
//      animAddTransitionTo(A, T[76], 0.2, 2051); then animGlobalSync(A).
//   5 and 9: animAddTransitionTo(A, T[132], 0.1, 3).
//   6, 14, 15, 16, 13: animAddOneShot(A, header, 0.1, 0.1, flags) with T[36]/2056,
//      T[44]/18440, T[60]/18440, T[52]/2056, T[84]/2056 respectively; the word at +960 = 0.
//   7: resetRouting; animAddTransitionTo(A, T[68], 0.1, 2051); word at +256 = word at +808 + 60;
//      animGlobalSync(A).
//   8: with cur the halfword at +572: when (cur == 6 and bit 3 of the halfword at +294) or
//      cur == 9, the local becomes cur (sign-extended); else animAddOneShot(A, T[108], 0.1, 0.1, 16).
//  10: bit 0 of the word at +12 is cleared unless bit 1 is set; dropTreasure(object, copy of the
//      +16 point, signed halfword +56, 36.0, 2.0, 2.8, 0.0, 3.0, 0.25, null); then one eRand roll
//      selects the header T[92 + 8 * (roll & 1)] for animAddTransitionTo(A, header, 0.1, 6); then
//      the object's virtual function at vtable byte offset 192 is called with no arguments.
// Returns nothing. Unknown: what the state numbers mean, and the roles of +256, +272, +808, +960.
// The host check defines NORRATH_W10AC_HOST so that 32-bit address words map to host memory.
extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int distance);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float time, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animGlobalSync__FP14AnimationState(void* state);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const void* position, int yaw, float a, float b, float c,
                                               float d, float e, float f, char* text);
int eRand__10GameObject(void* object);
#if defined(NORRATH_W10AC_HOST)
extern unsigned char* norrath_w10ac_host_base;
void* norrath_w10ac_host_function(unsigned word);
#endif
}

namespace {
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
}

extern "C" void setState__6Badgeri(void* object_storage, int new_state) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    int state = new_state;
    if (setStateProcessing__8CreaturePi(object, &state) != 0) return;
    unsigned char* animation = object + 288;
    unsigned char* table = address_of(read_word(object + 36u));
    switch (static_cast<unsigned>(state)) {
    case 0:
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(
            animation, address_of(read_word(table + 132u)), 0.1f, 3);
        write_word(object + 256u, read_word(object + 808u) + 180u);
        break;
    case 1: {
        resetRouting__8Creature(object);
        unsigned char destination[12];
        getSmartRandomDestination__8Creaturei(destination, object, 200);
        unsigned char copy[12];
        copy_bytes(copy, destination, 12u);
        copy_bytes(object + 272u, copy, 12u);
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(
            animation, address_of(read_word(table + 148u)), 0.1f, 2051);
        animGlobalSync__FP14AnimationState(animation);
        break;
    }
    case 3: {
        resetRouting__8Creature(object);
        short current = read_half(object + 572u);
        if (current != 3 && current != 6 && current != 14 && current != 15 && current != 16 && current != 13)
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(
                animation, address_of(read_word(table + 76u)), 0.2f, 2051);
        animGlobalSync__FP14AnimationState(animation);
        break;
    }
    case 5:
    case 9:
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(
            animation, address_of(read_word(table + 132u)), 0.1f, 3);
        break;
    case 6:
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, address_of(read_word(table + 36u)), 0.1f,
                                                                0.1f, 2056);
        write_word(object + 960u, 0u);
        break;
    case 14:
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, address_of(read_word(table + 44u)), 0.1f,
                                                                0.1f, 18440);
        write_word(object + 960u, 0u);
        break;
    case 15:
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, address_of(read_word(table + 60u)), 0.1f,
                                                                0.1f, 18440);
        write_word(object + 960u, 0u);
        break;
    case 16:
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, address_of(read_word(table + 52u)), 0.1f,
                                                                0.1f, 2056);
        write_word(object + 960u, 0u);
        break;
    case 13:
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, address_of(read_word(table + 84u)), 0.1f,
                                                                0.1f, 2056);
        write_word(object + 960u, 0u);
        break;
    case 7:
        resetRouting__8Creature(object);
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(
            animation, address_of(read_word(table + 68u)), 0.1f, 2051);
        write_word(object + 256u, read_word(object + 808u) + 60u);
        animGlobalSync__FP14AnimationState(animation);
        break;
    case 8: {
        short current = read_half(object + 572u);
        if ((current == 6 && (read_half(object + 294u) & 8) != 0) || current == 9) {
            state = current;
        } else {
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, address_of(read_word(table + 108u)),
                                                                    0.1f, 0.1f, 16);
        }
        break;
    }
    case 10: {
        unsigned flags = read_word(object + 12u);
        if ((flags & 2u) == 0u) write_word(object + 12u, flags & 0xfffffffeu);
        unsigned char position[12];
        copy_bytes(position, object + 16u, 12u);
        dropTreasure__8CreatureG6Point3iffffffPc(object, position, read_half(object + 56u), 36.0f, 2.0f, 2.8f, 0.0f,
                                                  3.0f, 0.25f, nullptr);
        unsigned roll = static_cast<unsigned>(eRand__10GameObject(object)) & 1u;
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(
            animation, address_of(read_word(address_of(read_word(object + 36u)) + 92u + roll * 8u)), 0.1f, 6);
        call_slot(object, 192);
        break;
    }
    default:
        break;
    }
    object[572] = static_cast<unsigned char>(state);
    object[573] = static_cast<unsigned char>(static_cast<unsigned>(state) >> 8);
}
