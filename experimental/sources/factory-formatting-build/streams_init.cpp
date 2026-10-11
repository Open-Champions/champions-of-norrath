#include "formatting.hpp"
extern "C" void norrath_factory_streams_init(unsigned char* reent) {
    using namespace norrath_format_detail;
    const Word context = address(reent);
    const Word first_stream = context + 484u;
    write_word(context + 60u, function_word(&norrath_factory_cleanup_reentrant));
    write_word(context + 56u, 1u);
    norrath_factory_standard_stream_init(bytes(first_stream), 4, 0, reent);
    norrath_factory_standard_stream_init(bytes(context + 572u), 9, 1, reent);
    norrath_factory_standard_stream_init(bytes(context + 660u), 10, 2, reent);
    write_word(context + 472u, 0u);
    write_word(context + 480u, first_stream);
    write_word(context + 476u, 3u);
}
