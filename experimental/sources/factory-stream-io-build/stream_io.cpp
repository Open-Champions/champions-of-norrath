#include "stream_io.hpp"
using namespace norrath_stream_io_detail;
extern "C" int norrath_factory_stream_read(unsigned char* stream, char* destination, int count) {
    const Word file = address(stream);
    const int result = norrath_factory_read_reentrant(bytes(read_word(file + 84u)), descriptor(file), destination, count);
    if (result >= 0) write_word(file + 80u, read_word(file + 80u) + Word(result));
    else write_half(file + 12u, half(file + 12u) & 0xefffu);
    return result;
}
extern "C" int norrath_factory_stream_write(unsigned char* stream, const char* source, int count) {
    const Word file = address(stream);
    if (half(file + 12u) & 0x100u)
        (void)norrath_factory_seek_reentrant(bytes(read_word(file + 84u)), descriptor(file), 0, 2);
    const Word flags = half(file + 12u);
    const int fd = descriptor(file);
    unsigned char* reent = bytes(read_word(file + 84u));
    write_half(file + 12u, flags & 0xefffu);
    return norrath_factory_write_reentrant(reent, fd, source, count);
}
extern "C" NorrathFormatSigned64 norrath_factory_stream_seek(unsigned char* stream, NorrathFormatSigned64 offset, int whence) {
    const Word file = address(stream);
    const NorrathFormatSigned64 result = norrath_factory_seek_reentrant(bytes(read_word(file + 84u)), descriptor(file), offset, whence);
    Word flags = half(file + 12u);
    if (result == -1) flags &= 0xefffu;
    else { write_word(file + 80u, Word(result)); flags |= 0x1000u; }
    write_half(file + 12u, flags);
    return result;
}
extern "C" int norrath_factory_stream_close(unsigned char* stream) {
    const Word file = address(stream);
    const int fd = descriptor(file);
    return norrath_factory_close_reentrant(bytes(read_word(file + 84u)), fd);
}
extern "C" void norrath_factory_cleanup_reentrant(unsigned char* reent) {
    (void)norrath_factory_walk_streams(reent, &norrath_factory_stream_flush);
}
extern "C" int norrath_factory_stream_flush(unsigned char* stream) {
    if (stream == nullptr)
        return norrath_factory_walk_streams(bytes(read_word(address(norrath_factory_impure_pointer_word))), &norrath_factory_stream_flush);
    const Word file = address(stream);
    Word reent = read_word(file + 84u);
    if (reent == 0u) {
        reent = read_word(address(norrath_factory_impure_pointer_word));
        write_word(file + 84u, reent);
    }
    if (read_word(reent + 56u) == 0u) norrath_factory_streams_init(bytes(reent));
    const Word flags = half(file + 12u);
    if (!(flags & 8u)) return 0;
    Word source = read_word(file + 16u);
    if (source == 0u) return 0;
    Word remaining = read_word(file) - source;
    write_word(file, source);
    const Word available = flags & 3u ? 0u : read_word(file + 20u);
    write_word(file + 8u, available);
    if (signed_word(remaining) <= 0) return 0;
    do {
        const int result = write_callback(file, source, remaining);
        remaining -= Word(result);
        if (result <= 0) return fail(file);
        source += Word(result);
    } while (signed_word(remaining) > 0);
    return 0;
}
extern "C" int norrath_factory_scatter_write(unsigned char* stream, unsigned char* iovec_header) {
    const Word file = address(stream), header = address(iovec_header);
    if (read_word(header + 8u) == 0u) return 0;
    Word flags = half(file + 12u);
    if (!(flags & 8u) || read_word(file + 16u) == 0u) {
        if (norrath_factory_stream_write_setup(stream) != 0) return -1;
        flags = half(file + 12u);
    }
    Word vector = read_word(header), source = 0u, length = 0u;
    // The original skips empty vectors without consulting header entry count.
    auto next_nonempty = [&]() {
        do { length = read_word(vector + 4u); source = read_word(vector); vector += 8u; } while (length == 0u);
    };
    auto advance = [&](Word consumed) {
        const Word remaining = read_word(header + 8u) - consumed;
        source += consumed; length -= consumed;
        write_word(header + 8u, remaining);
        return remaining;
    };
    if (flags & 2u) {
        for (;;) {
            if (length == 0u) next_nonempty();
            const Word requested = length < 1025u ? length : 1024u;
            const int result = write_callback(file, source, requested);
            if (result <= 0) return fail(file);
            if (advance(Word(result)) == 0u) return 0;
        }
    }
    if (!(flags & 1u)) {
        for (;;) {
            if (length == 0u) next_nonempty();
            Word consumed;
            Word available = read_word(file + 8u);
            if (flags & 0x200u) {
                const Word copied = length < available ? length : available;
                move(read_word(file), source, copied);
                const Word fresh_available = read_word(file + 8u) - copied;
                const Word fresh_current = read_word(file) + copied;
                // String streams consume the whole vector even when copy truncates.
                consumed = length;
                write_word(file + 8u, fresh_available); write_word(file, fresh_current);
            } else if (read_word(file + 16u) < read_word(file) && available < length) {
                consumed = available;
                move(read_word(file), source, consumed);
                write_word(file, read_word(file) + consumed);
                if (norrath_factory_stream_flush(stream) != 0) return fail(file);
            } else {
                const Word size = read_word(file + 20u);
                if (length >= size) {
                    const int result = write_callback(file, source, size);
                    if (result <= 0) return fail(file);
                    consumed = Word(result);
                } else {
                    consumed = length;
                    move(read_word(file), source, consumed);
                    const Word fresh_available = read_word(file + 8u) - consumed;
                    const Word fresh_current = read_word(file) + consumed;
                    write_word(file + 8u, fresh_available); write_word(file, fresh_current);
                }
            }
            if (advance(consumed) == 0u) return 0;
            flags = half(file + 12u);
        }
    }
    bool newline_known = false;
    Word through_newline = 0u;
    for (;;) {
        if (length == 0u) { newline_known = false; next_nonempty(); }
        if (!newline_known) {
            const unsigned char* newline = norrath_factory_memchr(bytes(source), 10, length);
            through_newline = newline == nullptr ? length + 1u : address(newline) - source + 1u;
            newline_known = true;
        }
        const Word requested = length < through_newline ? length : through_newline;
        const Word current = read_word(file), base = read_word(file + 16u);
        const Word available = read_word(file + 8u), size = read_word(file + 20u);
        Word consumed = available + size;
        if (base < current && signed_word(consumed) < signed_word(requested)) {
            move(current, source, consumed);
            write_word(file, read_word(file) + consumed);
            const int result = norrath_factory_stream_flush(stream);
            through_newline -= consumed;
            if (result != 0) return fail(file);
        } else if (signed_word(requested) >= signed_word(size)) {
            const int result = write_callback(file, source, size);
            consumed = Word(result);
            through_newline -= consumed;
            if (result <= 0) return fail(file);
        } else {
            consumed = requested;
            move(current, source, consumed);
            const Word fresh_available = read_word(file + 8u) - consumed;
            const Word fresh_current = read_word(file) + consumed;
            write_word(file + 8u, fresh_available); write_word(file, fresh_current);
            through_newline -= consumed;
        }
        if (through_newline == 0u) {
            if (norrath_factory_stream_flush(stream) != 0) return fail(file);
            newline_known = false;
        }
        if (advance(consumed) == 0u) return 0;
    }
}
