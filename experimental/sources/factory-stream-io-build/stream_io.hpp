#pragma once
#include "formatter_helpers.hpp"
extern "C" {
// Complete genuine reentrant syscall wrappers remain source closure work.
int norrath_factory_read_reentrant(unsigned char* reent, int descriptor, char* destination, int count);
int norrath_factory_write_reentrant(unsigned char* reent, int descriptor, const char* source, int count);
NorrathFormatSigned64 norrath_factory_seek_reentrant(unsigned char* reent, int descriptor, NorrathFormatSigned64 offset, int whence);
int norrath_factory_close_reentrant(unsigned char* reent, int descriptor);
int norrath_factory_walk_streams(unsigned char* reent, int (*callback)(unsigned char*));
// Canonical genuine memmove identity already declared by the archive source.
void* norrath_archive_move_bytes(void* destination, const void* source, unsigned count);
}
namespace norrath_stream_io_detail {
using namespace norrath_format_detail;
using norrath_core_format_detail::signed_word;
inline Word half(Word a) { return Word(bytes(a)[0]) | Word(bytes(a)[1]) << 8; }
inline int descriptor(Word stream) {
    Word h = half(stream + 14u);
    return h < 0x8000u ? int(h) : int(h) - 0x10000;
}
inline char* text(Word a) { return reinterpret_cast<char*>(static_cast<__UINTPTR_TYPE__>(a)); }
inline int write_callback(Word stream, Word source, Word count) {
    using Driver = int (*)(unsigned char*, const char*, int);
    Driver driver = reinterpret_cast<Driver>(static_cast<__UINTPTR_TYPE__>(read_word(stream + 36u)));
    return driver(bytes(read_word(stream + 28u)), text(source), signed_word(count));
}
inline void move(Word destination, Word source, Word count) {
    (void)norrath_archive_move_bytes(bytes(destination), bytes(source), count);
}
inline int fail(Word stream) {
    write_half(stream + 12u, half(stream + 12u) | 0x40u);
    return -1;
}
}
