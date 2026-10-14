// Observed __9LooseFireG6Point3P10GameObject at 0x001e6490, 212 bytes in .text. Arguments: the object, a point, and an
// owner object pointer (may be null). Returns nothing observed (the caller's object pointer is unused afterwards).
// Steps, in order:
//  1. The GameObject base constructor is called as (object, a copy of the point, 0, 0).
//  2. Word +72 = _vt$9LooseFire. Word +76 (a list head) = 0. When the owner is non-null, listOnewayLink(object+76, owner,
//     owner) is called.
//  3. Word +80 = frameNm + 120. addEffect(5, a copy of the object's 12 bytes at +16).
//  4. objectAddToRunList(object). Float +28 = 10.0f. Word +12 gets the bits 0x420c set (read after the run-list call).
// Inferred: a loose fire effect object owned by another game object. Unknown: the meanings of the fields and the constant
// 0x420c's individual bits. Nothing here ran on the EE; checks are the host contract in this directory and a private
// paired run.
#if defined(NORRATH_HOST_ARENA)
// Host checks only: stored 32-bit addresses are arena offsets; the check supplies both conversions.
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const void* pointer);
#endif
struct Point3 { float x, y, z; };
struct ListHead { void* first; unsigned unknown[3]; };

extern "C" {
extern unsigned char vt_LooseFire[] asm("_vt$9LooseFire");
extern unsigned frameNm;
void* __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* object, Point3 point, void* map, char** arguments);
void listOnewayLink__FP8ListHeadT0Pv(ListHead* list, void* first, void* second);
void addEffect__FiG6Point3(int kind, Point3 point);
void objectAddToRunList__FP10GameObject(void* object);
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
}

extern "C" void __9LooseFireG6Point3P10GameObject(void* self, Point3 point, void* owner) {
    unsigned char* o = static_cast<unsigned char*>(self);
    __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(self, point, nullptr, nullptr);
    store_word(o, 72, address_word(vt_LooseFire));
    store_word(o, 76, 0);
    if (owner != nullptr) listOnewayLink__FP8ListHeadT0Pv(reinterpret_cast<ListHead*>(o + 76), owner, owner);
    store_word(o, 80, frameNm + 120u);
    addEffect__FiG6Point3(5, *reinterpret_cast<const Point3*>(o + 16));
    objectAddToRunList__FP10GameObject(self);
    store_float(o, 28, 10.0f);
    store_word(o, 12, load_word(o, 12) | 0x420cu);
}
