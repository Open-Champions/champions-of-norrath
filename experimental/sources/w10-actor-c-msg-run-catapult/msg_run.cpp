// Observed msg_run__8Catapult at 0x0028bd08, 620 bytes. Offsets are into the object.
//  1. When the signed byte at +87 is nonzero, return.
//  2. The byte at +76 becomes objectActiveProcessing(object, signed +76, 1600); when its low
//     byte is zero, return.
//  3. When the word at +244 is nonzero, decrement it and return.
//  4. When both the signed bytes at +84 and +78 are zero and the float at +124 is below the pool
//     constant about 2.1002: set byte +84 to 1. If the word at +80 is nonzero, scriptStart on
//     it as a string; a zero result sets byte +85 to 0 and skips the launch below. Otherwise
//     launch: V = (icos(h) * s, isin(h) * s, s * 0.5) with h the signed halfword at +56 and s
//     the float at +240; allocate 184 bytes and, when that succeeds, run the FireBomblet
//     constructor on it with the object's +16 point (by value), the halfword at +56 minus 32768
//     as a short, a null string, the point (x + 0.0, y, z + 100.0) (by value), &V and the
//     integer 1. The result (null or the new object) is stored at +88; when nonzero,
//     setHold(it, 1). Bytes +78 and +85 become 1.
//  5. Otherwise, when either of the bytes at +84 and +78 is nonzero and the pool constant about
//     158.58 is below the float at +124: if the byte at +78 is zero only +84 is cleared; else
//     the held object at +88 (when nonzero) gets setHold(it, 0), the word at +88 is cleared
//     and bytes +78 and +84 are cleared.
//  6. When the signed byte at +96 is nonzero: animAdvanceAnimation(object+92, object+16, 0, 0).
//  7. When the word at +88 is nonzero: modelGetCharAttachmentPos(model, copy of the +16 point,
//     the signed halfword at +56, object+92, 0, held object + 16, object + 228, 1.0f), where the
//     model is the word at +12 of the structure pointed to by the 32-bit pointer at +36.
// Returns nothing. Unknown: what the held FireBomblet, the floats and the byte flags mean; the
// roles of the two out-parameters of modelGetCharAttachmentPos are not established.
// The host check defines NORRATH_W10AC_HOST so that 32-bit address words map to host memory.
extern "C" {
int objectActiveProcessing__FP10GameObjectii(void* object, int a, int b);
int scriptStart__FPc(const char* name);
float icos__Fi(int angle);
float isin__Fi(int angle);
unsigned char* norrath_block_alloc(int requested_size);
void __11FireBombletG6Point3sPcT1P6Point3i(void* self, const void* position, short yaw, char* text,
                                           const void* second_position, void* velocity, int flag);
void setHold__11FireBombleti(void* bomblet, int hold);
void animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(void* state, void* position, void* a, short* b);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, const void* position, short yaw, void* state, int a, void* out_a, void* out_b, float scale);
#if defined(NORRATH_W10AC_HOST)
extern unsigned char* norrath_w10ac_host_base;
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
#if defined(NORRATH_W10AC_HOST)
// Host check only: pointer words are offsets from a base chosen by the check, and an
// allocation result is stored as such an offset.
void* pointer_from_word(unsigned value) { return norrath_w10ac_host_base + value; }
unsigned word_from_pointer(const void* pointer) {
    return static_cast<unsigned>(static_cast<const unsigned char*>(pointer) - norrath_w10ac_host_base);
}
#else
void* pointer_from_word(unsigned value) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(value));
}
unsigned word_from_pointer(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
#endif
void copy_bytes(unsigned char* dest, const unsigned char* src, unsigned count) {
    for (unsigned i = 0; i < count; ++i) dest[i] = src[i];
}
float read_float(const unsigned char* p) {
    union { unsigned u; float f; } v;
    v.u = read_word(p);
    return v.f;
}
void write_float(unsigned char* p, float value) {
    union { unsigned u; float f; } v;
    v.f = value;
    write_word(p, v.u);
}
short read_half(const unsigned char* p) { return static_cast<short>(p[0] | (p[1] << 8)); }
const float kLaunchBelow = 2.10017991065979f;
const float kReleaseAbove = 158.57968139648438f;
}

extern "C" void msg_run__8Catapult(void* object_storage) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    if (static_cast<signed char>(object[87]) != 0) return;
    int active = objectActiveProcessing__FP10GameObjectii(object, static_cast<signed char>(object[76]), 1600);
    object[76] = static_cast<unsigned char>(active);
    if ((active & 0xff) == 0) return;
    unsigned countdown = read_word(object + 244u);
    if (countdown != 0u) {
        write_word(object + 244u, countdown - 1u);
        return;
    }
    float distance = read_float(object + 124u);
    if (static_cast<signed char>(object[84]) == 0 && static_cast<signed char>(object[78]) == 0) {
        if (distance < kLaunchBelow) {
            object[84] = 1;
            unsigned script = read_word(object + 80u);
            bool launch = true;
            if (script != 0u && scriptStart__FPc(static_cast<const char*>(pointer_from_word(script))) == 0) {
                object[85] = 0;
                launch = false;
            }
            if (launch) {
                int angle = read_half(object + 56u);
                float cosine = icos__Fi(angle);
                float sine = isin__Fi(angle);
                float speed = read_float(object + 240u);
                float velocity[3];
                velocity[0] = cosine * speed;
                velocity[1] = sine * speed;
                velocity[2] = speed * 0.5f;
                unsigned char* bomblet = norrath_block_alloc(184);
                if (bomblet != nullptr) {
                    float offset[3] = {0.0f, 0.0f, 100.0f};
                    float raised[3];
                    raised[0] = read_float(object + 16u) + offset[0];
                    raised[1] = read_float(object + 20u);
                    raised[2] = read_float(object + 24u) + offset[2];
                    unsigned char position[12];
                    copy_bytes(position, object + 16u, 12u);
                    unsigned yaw = static_cast<unsigned>(object[56]) | (static_cast<unsigned>(object[57]) << 8);
                    __11FireBombletG6Point3sPcT1P6Point3i(
                        bomblet, position, static_cast<short>(yaw - 32768u), nullptr, raised, velocity, 1);
                }
                write_word(object + 88u, bomblet == nullptr ? 0u : word_from_pointer(bomblet));
                if (bomblet != nullptr) setHold__11FireBombleti(bomblet, 1);
                object[78] = 1;
                object[85] = 1;
            }
        }
    } else if (kReleaseAbove < distance) {
        if (static_cast<signed char>(object[78]) == 0) {
            object[84] = 0;
        } else {
            unsigned held = read_word(object + 88u);
            if (held != 0u) setHold__11FireBombleti(pointer_from_word(held), 0);
            write_word(object + 88u, 0u);
            object[78] = 0;
            object[84] = 0;
        }
    }
    if (static_cast<signed char>(object[96]) != 0)
        animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(object + 92, object + 16, nullptr, nullptr);
    unsigned held = read_word(object + 88u);
    if (held != 0u) {
        unsigned char* shared = static_cast<unsigned char*>(pointer_from_word(read_word(object + 36u)));
        unsigned char position[12];
        copy_bytes(position, object + 16u, 12u);
        modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
            pointer_from_word(read_word(shared + 12u)), position, read_half(object + 56u), object + 92, 0,
            static_cast<unsigned char*>(pointer_from_word(held)) + 16, object + 228, 1.0f);
    }
}
