#include "formatter_helpers.hpp"
extern "C" int norrath_factory_sprint(unsigned char* stream, unsigned char* iovec_header) {
    using namespace norrath_format_detail;
    const Word header = address(iovec_header);
    int answer = 0;
    if (read_word(header + 8u) != 0u) {
        answer = norrath_factory_scatter_write(stream, iovec_header);
        write_word(header + 4u, 0u);
        write_word(header + 8u, 0u);  // Clears after the genuine call, even on failure.
    } else {
        write_word(header + 4u, 0u);  // Original zero-residual path does not rewrite +8.
    }
    return answer;
}
