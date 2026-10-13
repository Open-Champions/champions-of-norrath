#pragma once
// Field access for the render, model and animation sources of this package. Records are
// addressed by byte offset because no original class declaration is claimed. The types
// are may_alias so a record can be read as int, float or halfword at different offsets.
// A host test defines NORRATH_HOST_ARENA and supplies the two hooks below, since a
// 64-bit host cannot hold a 32-bit target pointer in a record word.
static_assert(__CHAR_BIT__ == 8 && sizeof(int) == 4 && sizeof(float) == 4 && sizeof(short) == 2,
              "target scalar representation");
static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target byte order");
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const unsigned char* pointer);
#else
static_assert(sizeof(void*) == 4, "target32 pointer words");
#endif
namespace rend {
typedef int __attribute__((may_alias)) i32;
typedef unsigned __attribute__((may_alias)) u32;
typedef short __attribute__((may_alias)) i16;
typedef unsigned short __attribute__((may_alias)) u16;
typedef float __attribute__((may_alias)) f32;
typedef signed char __attribute__((may_alias)) i8;
template <class T> inline T& at(const void* base, int offset) {
    return *reinterpret_cast<T*>(const_cast<unsigned char*>(static_cast<const unsigned char*>(base)) + offset);
}
inline unsigned char* ptr(const void* base, int offset) {
#ifdef NORRATH_HOST_ARENA
    const unsigned word = at<u32>(base, offset);
    return word == 0 ? nullptr : norrath_host_pointer(word);
#else
    return at<unsigned char*>(base, offset);
#endif
}
inline void set_ptr(void* base, int offset, const void* value) {
#ifdef NORRATH_HOST_ARENA
    at<u32>(base, offset) = value == nullptr ? 0u : norrath_host_address(static_cast<const unsigned char*>(value));
#else
    at<const void*>(base, offset) = value;
#endif
}
}
