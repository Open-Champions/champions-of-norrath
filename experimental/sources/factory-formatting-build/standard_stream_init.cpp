#include "formatting.hpp"
extern "C" void norrath_factory_standard_stream_init(unsigned char* stream, int flags, int descriptor, unsigned char* reent) {
    using namespace norrath_format_detail;
    const Word state = address(stream);
    write_word(state + 84u, address(reent));
    write_half(state + 12u, unsigned(flags));
    write_half(state + 14u, unsigned(descriptor));
    write_word(state + 32u, function_word(&norrath_factory_stream_read));
    write_word(state + 36u, function_word(&norrath_factory_stream_write));
    write_word(state + 40u, function_word(&norrath_factory_stream_seek));
    write_word(state + 44u, function_word(&norrath_factory_stream_close));
    write_word(state, 0u);
    write_word(state + 4u, 0u);
    write_word(state + 8u, 0u);
    write_word(state + 16u, 0u);
    write_word(state + 20u, 0u);
    write_word(state + 24u, 0u);
    write_word(state + 28u, state);
}
