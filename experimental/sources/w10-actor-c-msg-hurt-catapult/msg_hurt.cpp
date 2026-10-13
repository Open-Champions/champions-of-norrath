// Observed msg_hurt__8CatapultfP10GameObjecti at 0x0028c1d0, 656 bytes. The float and the int
// arguments are not read; the second pointer is the other object (A). Offsets are into the object.
//  1. When the signed byte at +87 is nonzero, return. Otherwise set it to 1.
//  2. When the signed byte at +76 is zero, objectAddToRunList(object).
//  3. Flag F is true when the tag "reduceCarnage" is present (findTagString, index 0) and its
//     integer value (findTagInt, index 0) is nonzero.
//  4. The lump resource "catapult_broken.vif" in "catapult.lmp" gives a record R; for each index
//     i from 0 while i < the signed byte at R+18 (re-read each pass): when F is set, one
//     eRand(object) roll whose low bit is zero skips the pass; otherwise a 584-byte block is
//     allocated and, when that succeeds, the PhProp constructor runs on it with the object's
//     +16 point (by value), "catapult.lmp", "catapult_broken.vif", "catapult_broken.tex", the
//     long 1 << i, the int 534, a null description and 1.0f; then one more eRand(object) roll,
//     and when its low three bits are zero setOnFire(new object, 1).
//  5. When the signed byte at +78 is nonzero, the word at +88 is nonzero and the signed byte
//     cutMode is zero: setHold(held, 0); detach(held); the word at +88 is re-read and cleared;
//     the vector at held+76 becomes unit(object+16 - A+16) * 4.0 (components subtracted x, y, z;
//     the length from x*x + y*y + z*z with the architectural reciprocal square root); the byte
//     at +78 is cleared.
//  6. When the tag "deathScript" is present, scriptStart on its text.
//  7. objectAddToDeleteList(object).
// Returns nothing. Unknown: what the held FireBomblet, the lump record byte at +18 and the
// +76/+87 bytes mean. The host check defines NORRATH_W10AC_HOST so address words map to host
// memory.
extern "C" {
void objectAddToRunList__FP10GameObject(void* object);
char* findTagString__10GameObjectPci(void* object, const char* tag, int index);
int findTagInt__10GameObjectPci(void* object, const char* tag, int index);
void* lumpFindResource__FPcT0(const char* lump, const char* name);
int eRand__10GameObject(void* object);
unsigned char* norrath_block_alloc(int requested_size);
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, const void* position, const char* lump,
                                                      const char* model, const char* texture, long mask, int kind,
                                                      void* description, float scale);
void setOnFire__6PhPropi(void* prop, int on);
void setHold__11FireBombleti(void* bomblet, int hold);
void detach__11FireBomblet(void* bomblet);
int scriptStart__FPc(const char* name);
void objectAddToDeleteList__FP10GameObject(void* object);
extern signed char cutMode;
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
// Host check only: pointer words are offsets from a base chosen by the check.
void* pointer_from_word(unsigned value) { return norrath_w10ac_host_base + value; }
#else
void* pointer_from_word(unsigned value) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(value));
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
__attribute__((always_inline)) inline float reciprocal_sqrt(float value) {
#if defined(__mips__)
    float result;
    float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(value));
    return result;
#else
    return 1.0f / __builtin_sqrtf(value);
#endif
}
}

extern "C" void msg_hurt__8CatapultfP10GameObjecti(void* object_storage, float, void* other_storage, int) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    unsigned char* other = static_cast<unsigned char*>(other_storage);
    if (static_cast<signed char>(object[87]) != 0) return;
    object[87] = 1;
    if (static_cast<signed char>(object[76]) == 0) objectAddToRunList__FP10GameObject(object);
    bool reduce_carnage = false;
    if (findTagString__10GameObjectPci(object, "reduceCarnage", 0) != nullptr)
        reduce_carnage = findTagInt__10GameObjectPci(object, "reduceCarnage", 0) != 0;
    const unsigned char* record = static_cast<const unsigned char*>(
        lumpFindResource__FPcT0("catapult.lmp", "catapult_broken.vif"));
    for (int index = 0; index < static_cast<signed char>(record[18]); ++index) {
        if (reduce_carnage && (eRand__10GameObject(object) & 1) == 0) continue;
        unsigned char* prop = norrath_block_alloc(584);
        if (prop != nullptr) {
            unsigned char position[12];
            copy_bytes(position, object + 16u, 12u);
            __6PhPropG6Point3PcN22liPQ24item11descriptionf(prop, position, "catapult.lmp", "catapult_broken.vif",
                                                           "catapult_broken.tex", 1L << index, 534, nullptr, 1.0f);
            if ((eRand__10GameObject(object) & 7) == 0) setOnFire__6PhPropi(prop, 1);
        }
    }
    unsigned held = read_word(object + 88u);
    if (static_cast<signed char>(object[78]) != 0 && held != 0u && cutMode == 0) {
        setHold__11FireBombleti(pointer_from_word(held), 0);
        detach__11FireBomblet(pointer_from_word(read_word(object + 88u)));
        unsigned char* velocity = static_cast<unsigned char*>(pointer_from_word(read_word(object + 88u))) + 76;
        write_word(object + 88u, 0u);
        float dx = read_float(object + 16u) - read_float(other + 16u);
        write_float(velocity, dx);
        float dy = read_float(object + 20u) - read_float(other + 20u);
        write_float(velocity + 4, dy);
        float dz = read_float(object + 24u) - read_float(other + 24u);
        float length_squared = dx * dx + dy * dy + dz * dz;
        float scale = reciprocal_sqrt(length_squared);
        write_float(velocity + 8, (dz * scale) * 4.0f);
        write_float(velocity, (dx * scale) * 4.0f);
        write_float(velocity + 4, (dy * scale) * 4.0f);
        object[78] = 0;
    }
    char* script = findTagString__10GameObjectPci(object, "deathScript", 0);
    if (script != nullptr) scriptStart__FPc(script);
    objectAddToDeleteList__FP10GameObject(object);
}
