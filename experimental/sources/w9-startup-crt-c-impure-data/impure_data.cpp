// Observed data: _impure_ptr at 0x00448480 (global, 4 bytes) holding 0x00448190, the address of
// the local 752-byte object impure_data at 0x00448190. Callers in the libc routines load the
// pointer and pass or dereference it. Nonzero words of the object in the file image, by byte
// offset: +4, +8 and +12 hold the addresses of three embedded 88-byte records at +484, +572 and
// +660 (the records themselves are all zero, and +748..+751 are zero); +52 holds the address of a
// one-character text "C" (0x0047e2f8); +168 holds the 64-bit value 1. Every other byte is zero.
// Reading these as stream-record pointers, a locale name and a seed is inferred from how the
// callers use the object, not observed in a symbol name. Nothing here was executed on the EE.
#include "impure_data.hpp"

namespace {
const char c_locale_text[] = "C";

ImpureData impure_data = {
    0,
    {&impure_data.records[0], &impure_data.records[1], &impure_data.records[2]},
    {},
    c_locale_text,
    {},
    1ull,
    {},
    {},
    0,
};
}  // namespace

extern "C" {
ImpureData* _impure_ptr = &impure_data;
}
