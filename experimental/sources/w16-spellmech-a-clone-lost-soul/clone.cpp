// Observed clone__8LostSoul at 0x001255a0, 512 bytes in .text. Argument: the source object. Returns the new object or null
// when the allocation fails.
// Steps, in order:
//  1. blockAlloc(176). When the result is null, null is returned and nothing else happens.
//  2. Word +72 = _vt$10GameObject. Three 4-byte list heads at +0, +4 and +8 are copy-constructed from the source's.
//  3. Plain fields are copied from the source as in the glowing-shaft clone: word +12; the 12 bytes at +16; float +28; word
//     +36; float +32; bytes +40 and +41; float +48; words +44 and +52; float +60; word +68; halfword +56; float +64. Word
//     +72 = _vt$10Projectile is stored between them.
//  4. A list head at +76 is copy-constructed from the source's. Byte +80 is copied. Word +72 = _vt$8LostSoul. Then word +84
//     and the 12 bytes at +88 are copied.
//  5. The 60 bytes at +100 are copied from the source (a light record). When either of its first two words (+100, +104) is
//     non-zero in the copy, both are set to zero and lightAdd is called with the address of the copy's +100.
//  6. Words +160, +164, +168 and +172 are copied. The new object is returned.
// Inferred: the virtual clone of a lost-soul projectile, re-registering the copied light. Unknown: the meanings of the
// copied fields and the light's first two words. Nothing here ran on the EE; checks are the host contract in this
// directory and a private paired run.
#if defined(NORRATH_HOST_ARENA)
// Host checks only: stored 32-bit addresses are arena offsets; the check supplies both conversions.
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const void* pointer);
#endif
struct Point3 { float x, y, z; };
struct ListHead { void* first; unsigned unknown[3]; };

extern "C" {
extern unsigned char vt_GameObject[] asm("_vt$10GameObject");
extern unsigned char vt_Projectile[] asm("_vt$10Projectile");
extern unsigned char vt_LostSoul[] asm("_vt$8LostSoul");
void* blockAlloc__Fi(int size);
void* __8ListHeadRC8ListHead(void* destination, const void* source);
void lightAdd__FP5Light(void* light);
}

namespace {
struct WordTriple { unsigned words[3]; };

#if defined(NORRATH_HOST_ARENA)
inline unsigned address_word(const void* p) { return norrath_host_address(p); }
inline unsigned char* word_pointer(unsigned a) { return a == 0 ? nullptr : norrath_host_pointer(a); }
#else
inline unsigned address_word(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
inline unsigned char* word_pointer(unsigned a) { return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(a)); }
#endif
inline unsigned load_word(const unsigned char* base, unsigned offset) {
    return *reinterpret_cast<const unsigned*>(base + offset);
}
inline unsigned char* pointer_word(const unsigned char* base, unsigned offset) {
    return word_pointer(load_word(base, offset));
}
inline void store_word(unsigned char* base, unsigned offset, unsigned value) {
    *reinterpret_cast<unsigned*>(base + offset) = value;
}
inline void store_half(unsigned char* base, unsigned offset, unsigned short value) {
    *reinterpret_cast<unsigned short*>(base + offset) = value;
}
inline void store_float(unsigned char* base, unsigned offset, float value) {
    *reinterpret_cast<float*>(base + offset) = value;
}
inline float load_float(const unsigned char* base, unsigned offset) {
    return *reinterpret_cast<const float*>(base + offset);
}
inline float float_bits(unsigned bits) {
    union { unsigned u; float f; } value;
    value.u = bits;
    return value.f;
}
struct LightRecord { unsigned words[15]; };
}

extern "C" void* clone__8LostSoul(void* self) {
    const unsigned char* s = static_cast<const unsigned char*>(self);
    unsigned char* d = static_cast<unsigned char*>(blockAlloc__Fi(176));
    if (d == nullptr) return nullptr;
    store_word(d, 72, address_word(vt_GameObject));
    __8ListHeadRC8ListHead(d, s);
    __8ListHeadRC8ListHead(d + 4, s + 4);
    __8ListHeadRC8ListHead(d + 8, s + 8);
    store_word(d, 12, load_word(s, 12));
    *reinterpret_cast<WordTriple*>(d + 16) = *reinterpret_cast<const WordTriple*>(s + 16);
    store_float(d, 28, load_float(s, 28));
    store_word(d, 36, load_word(s, 36));
    store_float(d, 32, load_float(s, 32));
    d[40] = s[40];
    d[41] = s[41];
    store_word(d, 72, address_word(vt_Projectile));
    store_float(d, 48, load_float(s, 48));
    store_word(d, 44, load_word(s, 44));
    store_word(d, 52, load_word(s, 52));
    store_float(d, 60, load_float(s, 60));
    store_word(d, 68, load_word(s, 68));
    store_half(d, 56, *reinterpret_cast<const unsigned short*>(s + 56));
    store_float(d, 64, load_float(s, 64));
    __8ListHeadRC8ListHead(d + 76, s + 76);
    d[80] = s[80];
    store_word(d, 72, address_word(vt_LostSoul));
    store_word(d, 84, load_word(s, 84));
    *reinterpret_cast<WordTriple*>(d + 88) = *reinterpret_cast<const WordTriple*>(s + 88);
    unsigned char* light = d + 100;
    *reinterpret_cast<LightRecord*>(light) = *reinterpret_cast<const LightRecord*>(s + 100);
    if (load_word(light, 0) != 0 || load_word(light, 4) != 0) {
        store_word(light, 0, 0);
        store_word(light, 4, 0);
        lightAdd__FP5Light(light);
    }
    store_word(d, 160, load_word(s, 160));
    store_word(d, 164, load_word(s, 164));
    store_word(d, 168, load_word(s, 168));
    store_word(d, 172, load_word(s, 172));
    return d;
}
