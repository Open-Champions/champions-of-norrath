#include "formatter_helpers.hpp"
extern "C" int norrath_factory_buffered_printf(unsigned char* stream, const char* format, const unsigned char* argument_cursor) {
    using namespace norrath_core_format_detail;
    const Word original = address(stream);
    // Actual FILE minimum88; original buffer is1024 bytes at frame+96.
    // Remaining FILE fields are deliberately uninitialized, as in the original.
    alignas(16) unsigned char local_stream[88];
    alignas(16) unsigned char buffer[1024];
    const Word local = address(local_stream);
    const Word storage = address(buffer);
    write_word(local + 16u, storage);
    const Word flags = read_half(original + 12u);
    const Word descriptor = read_half(original + 14u);
    const Word reent = read_word(original + 84u);
    const Word cookie = read_word(original + 28u);
    const Word write_callback = read_word(original + 36u);
    write_word(local + 84u, reent);
    write_half(local + 12u, flags & 0xfffdu);
    write_half(local + 14u, descriptor);
    write_word(local + 28u, cookie);
    write_word(local + 36u, write_callback);
    write_word(local + 20u, 1024u);
    write_word(local, storage);
    write_word(local + 8u, 1024u);
    write_word(local + 24u, 0u);
    int answer = norrath_factory_vfprintf(local_stream, format, argument_cursor);
    if (answer >= 0) {
        if (norrath_factory_stream_flush(local_stream) != 0) answer = -1;
    }
    if (read_half(local + 12u) & 0x40u) {
        Word fresh_flags = read_half(original + 12u);
        write_half(original + 12u, fresh_flags | 0x40u);
    }
    return answer;
}
