#pragma once
// Field helpers for the w11-creature-a bodies. Object fields stay at their observed byte offsets
// inside the object and are read through may-alias scalar types. Pointer words are 32 bits wide
// on the target, so every body here is target-only (comparisons run through the private paired
// harness, not a host build).
static_assert(sizeof(void*) == 4 && sizeof(int) == 4 && sizeof(short) == 2 && sizeof(float) == 4, "target32 representation");

namespace w11_creature_a {
typedef unsigned __attribute__((may_alias)) U32;
typedef int __attribute__((may_alias)) I32;
typedef short __attribute__((may_alias)) I16;
typedef unsigned short __attribute__((may_alias)) U16;
typedef signed char __attribute__((may_alias)) I8;
typedef unsigned char __attribute__((may_alias)) U8;
typedef float __attribute__((may_alias)) F32;
typedef unsigned long long __attribute__((may_alias)) U64;
typedef void* __attribute__((may_alias)) Ptr;

struct __attribute__((may_alias)) Point3 {
    float x, y, z;
};

template <typename T>
struct Alias {
    typedef T __attribute__((may_alias)) Type;
};

template <>
struct Alias<Point3> {
    typedef Point3 Type;
};

template <typename T>
inline typename Alias<T>::Type& at(void* object, unsigned offset) {
    return *reinterpret_cast<typename Alias<T>::Type*>(static_cast<unsigned char*>(object) + offset);
}

// A virtual slot of the table pointed to by object+72 is an 8-byte entry: a signed halfword
// adjustment at +0 and the function word at +4. Callers pass the object plus the adjustment.
struct Slot {
    void* self;
    void* function;
};
inline Slot slot(void* object, unsigned slot_byte_offset) {
    unsigned char* entry = at<unsigned char*>(object, 72) + slot_byte_offset;
    Slot s;
    s.self = static_cast<unsigned char*>(object) + *reinterpret_cast<I16*>(entry);
    s.function = *reinterpret_cast<Ptr*>(entry + 4);
    return s;
}
}  // namespace w11_creature_a
