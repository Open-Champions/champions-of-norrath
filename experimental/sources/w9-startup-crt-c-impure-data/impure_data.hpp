#pragma once
// Layout of the 752-byte object that _impure_ptr addresses, as laid out by byte offset in the
// image. Field names state only the offset role; see impure_data.cpp for what was observed.
struct ImpureRecord {
    unsigned char bytes[88];
};

struct alignas(8) ImpureData {
    int word0;                           // +0, zero in the image
    ImpureRecord* record_pointers[3];    // +4, +8, +12
    unsigned char gap_a[36];             // +16..+51, zero
    const char* text_pointer;            // +52
    unsigned char gap_b[112];            // +56..+167, zero
    unsigned long long value_168;        // +168
    unsigned char gap_c[308];            // +176..+483, zero
    ImpureRecord records[3];             // +484, +572, +660
    unsigned tail;                       // +748, zero
};

#if defined(__mips__)
static_assert(sizeof(ImpureData) == 752, "impure object size");
static_assert(__builtin_offsetof(ImpureData, record_pointers) == 4, "record pointers at +4");
static_assert(__builtin_offsetof(ImpureData, text_pointer) == 52, "text pointer at +52");
static_assert(__builtin_offsetof(ImpureData, value_168) == 168, "value at +168");
static_assert(__builtin_offsetof(ImpureData, records) == 484, "records at +484");
static_assert(sizeof(ImpureRecord) == 88, "record size");
static_assert(__builtin_offsetof(ImpureData, tail) == 748, "tail at +748");
#endif
