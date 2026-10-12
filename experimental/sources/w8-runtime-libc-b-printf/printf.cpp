// Observed printf at 0x00302560, 112 bytes. It loads the object _impure_ptr
// aims at, takes its standard-out stream (offset 8), stores the impure
// pointer into that stream's owner word (offset 84), and calls the stream
// formatter (vfprintf at 0x00306a60, authored as norrath_factory_vfprintf)
// with that stream, the format, and the argument cursor, returning its
// result. The original spills a1..a3, t0..t3 and f12..f19 below the cursor;
// this uses the compiler varargs cursor, like the authored sprintf.
#include "../libc-format-impure/impure_state.hpp"

extern "C" NorrathImpureState* norrath_factory_impure_pointer_word;
extern "C" int norrath_factory_vfprintf(unsigned char* stream, const char* format,
                                        const unsigned char* argument_cursor);

namespace {
struct OwnedStream {
    unsigned char head[84];
    NorrathImpureState* owner;
};
}

extern "C" int printf(const char* format, ...) {
    NorrathImpureState* const state = norrath_factory_impure_pointer_word;
    OwnedStream* const stream = reinterpret_cast<OwnedStream*>(state->standard_out);
    stream->owner = state;
    __builtin_va_list arguments;
    __builtin_va_start(arguments, format);
    const int result = norrath_factory_vfprintf(reinterpret_cast<unsigned char*>(stream), format,
                                                reinterpret_cast<const unsigned char*>(arguments));
    __builtin_va_end(arguments);
    return result;
}
