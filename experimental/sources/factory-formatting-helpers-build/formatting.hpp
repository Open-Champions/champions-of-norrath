#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4 && sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4, "target32 byte representation");
static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target little endian words");
static_assert(sizeof(double) == 8 && sizeof(__builtin_va_list) == 4, "reviewed single-float EABI flat varargs cursor");
using NorrathFormatSigned64 = long long;
static_assert(sizeof(NorrathFormatSigned64) == 8, "original64-bit stream seek transport");
extern "C" {
extern unsigned char norrath_factory_impure_pointer_word[];
int norrath_factory_sprintf(char* destination, const char* format, ...);
int norrath_factory_vfprintf(unsigned char* stream, const char* format, const unsigned char* argument_cursor);
int norrath_factory_vfprintf_reentrant(unsigned char* reent, unsigned char* stream, const char* format, const unsigned char* argument_cursor);
void norrath_factory_streams_init(unsigned char* reent);
void norrath_factory_standard_stream_init(unsigned char* stream, int flags, int descriptor, unsigned char* reent);
// Genuine currently unfinished handlers. Merely storing their function words
// is not source completion or authorization to link original game code.
void norrath_factory_cleanup_reentrant(unsigned char* reent);
int norrath_factory_stream_read(unsigned char* stream, char* destination, int count);
int norrath_factory_stream_write(unsigned char* stream, const char* source, int count);
NorrathFormatSigned64 norrath_factory_stream_seek(unsigned char* stream, NorrathFormatSigned64 offset, int whence);
int norrath_factory_stream_close(unsigned char* stream);
}
namespace norrath_format_detail {
using Word = unsigned;
inline Word address(const void* p) { return static_cast<Word>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
inline unsigned char* bytes(Word address) { return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(address)); }
inline Word read_word(Word a) {
    const unsigned char* p = bytes(a);
    return Word(p[0]) | Word(p[1]) << 8 | Word(p[2]) << 16 | Word(p[3]) << 24;
}
inline void write_word(Word a, Word word) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(word);
    p[1] = static_cast<unsigned char>(word >> 8);
    p[2] = static_cast<unsigned char>(word >> 16);
    p[3] = static_cast<unsigned char>(word >> 24);
}
inline void write_half(Word a, Word half) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(half);
    p[1] = static_cast<unsigned char>(half >> 8);
}
template<class Function> inline Word function_word(Function function) {
    return static_cast<Word>(reinterpret_cast<__UINTPTR_TYPE__>(function));
}
}
