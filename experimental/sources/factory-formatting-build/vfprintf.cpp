#include "formatting.hpp"
extern "C" int norrath_factory_vfprintf(unsigned char* stream, const char* format, const unsigned char* argument_cursor) {
    using namespace norrath_format_detail;
    const Word state = address(stream);
    Word reent = read_word(state + 84u);
    if (reent == 0u) {
        reent = read_word(address(norrath_factory_impure_pointer_word));
        write_word(state + 84u, reent);
    }
    if (read_word(reent + 56u) == 0u) {
        norrath_factory_streams_init(bytes(reent));
        reent = read_word(state + 84u);  // Actual fresh post-initializer read.
    }
    return norrath_factory_vfprintf_reentrant(bytes(reent), stream, format, argument_cursor);
}
