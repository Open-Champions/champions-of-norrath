#include "formatting.hpp"
extern "C" int norrath_factory_sprintf(char* destination, const char* format, ...) {
    using namespace norrath_format_detail;
    const Word reent = read_word(address(norrath_factory_impure_pointer_word));
    // Only fields actually initialized by the original sprintf are filled.
    // This is its88-byte stream view, not a zero-filled replacement stream.
    alignas(8) unsigned char stream[88];
    const Word state = address(stream);
    write_word(state + 20u, 0x7fffffffu);
    write_word(state + 8u, 0x7fffffffu);
    write_half(state + 12u, 0x0208u);
    write_word(state + 16u, address(destination));
    write_word(state + 84u, reent);
    write_word(state, address(destination));
    __builtin_va_list arguments;
    __builtin_va_start(arguments, format);
    // Under the fixed single-float EABI this builtin type is void*, with
    // contiguous8-byte GPR save slots followed by overflow-stack slots.
    // Complex FPR/GPR va_list representations are intentionally unsupported.
    const auto* cursor = static_cast<const unsigned char*>(arguments);
    const int result = norrath_factory_vfprintf(stream, format, cursor);
    *bytes(read_word(state)) = 0u;
    __builtin_va_end(arguments);
    return result;
}
