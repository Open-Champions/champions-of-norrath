#pragma once
#include "formatting.hpp"
static_assert('0' == 48 && 'A' == 65 && '.' == 46 && '%' == 37, "observed target ASCII byte constants");
using NorrathFormatUnsigned64 = unsigned long long;
static_assert(sizeof(NorrathFormatUnsigned64) == 8, "actual full64 GPR bit transport");
extern "C" {
// Genuine still-unfinished dependencies: no original code address or host call.
unsigned char* norrath_factory_localeconv();
int norrath_factory_stream_write_setup(unsigned char* stream);
int norrath_factory_buffered_printf(unsigned char* stream, const char* format, const unsigned char* argument_cursor);
int norrath_factory_mbtowc_reentrant(unsigned char* reent, unsigned char* wide_character_word, const char* input, unsigned maximum_bytes, unsigned char* state_word);
int norrath_factory_sprint(unsigned char* stream, unsigned char* iovec_header);
int norrath_factory_double_isinf(NorrathFormatUnsigned64 bits);
int norrath_factory_double_isnan(NorrathFormatUnsigned64 bits);
int norrath_factory_double_compare(NorrathFormatUnsigned64 left_bits, NorrathFormatUnsigned64 right_bits);
unsigned char* norrath_factory_float_digits(unsigned char* reent, NorrathFormatUnsigned64 bits, int precision, unsigned flags, unsigned char* sign_byte, unsigned char* decimal_position_word, int format_character, unsigned char* digit_count_word);
int norrath_factory_format_exponent(unsigned char* destination, int exponent, int format_character);
const unsigned char* norrath_factory_memchr(const unsigned char* input, int byte, unsigned count);
unsigned norrath_factory_strlen(const char* text);
NorrathFormatUnsigned64 norrath_factory_umod64(NorrathFormatUnsigned64 numerator, NorrathFormatUnsigned64 denominator);
NorrathFormatUnsigned64 norrath_factory_udiv64(NorrathFormatUnsigned64 numerator, NorrathFormatUnsigned64 denominator);
extern unsigned char norrath_factory_mb_cur_max_word[];
// Actual original readonly minimum views, not a generated payload or owner.
extern const unsigned char norrath_formatter_space16_view[];
extern const unsigned char norrath_formatter_zero16_view[];
extern const unsigned char norrath_formatter_inf_text_view[];
extern const unsigned char norrath_formatter_nan_text_view[];
extern const unsigned char norrath_formatter_hex_lower_view[];
extern const unsigned char norrath_formatter_hex_upper_view[];
extern const unsigned char norrath_formatter_null_text_view[];
extern const unsigned char norrath_formatter_bad_base_view[];
extern const unsigned char norrath_formatter_zero_text_view[];
extern const unsigned char norrath_formatter_dot_text_view[];
}
namespace norrath_core_format_detail {
using namespace norrath_format_detail;
using Wide = NorrathFormatUnsigned64;
inline int signed_word(Word w) { return w <= 0x7fffffffu ? int(w) : -1 - int(0xffffffffu - w); }
inline int signed_byte(unsigned char b) { return b < 128u ? int(b) : int(b) - 256; }
inline Word read_half(Word a) { const unsigned char* p = bytes(a); return Word(p[0]) | Word(p[1]) << 8; }
inline Wide read_wide(Word a) { return Wide(read_word(a)) | Wide(read_word(a + 4u)) << 32; }
inline void write_wide(Word a, Wide w) { write_word(a, Word(w)); write_word(a + 4u, Word(w >> 32)); }
inline Wide extend_signed_word(Word w) { return Wide(w) | (w & 0x80000000u ? 0xffffffff00000000ULL : 0ULL); }
inline Wide read_integer(Word a, Word flags, bool signed_value) {
    if (flags & 0x10u) return read_wide(a);
    if (flags & 0x40u) {
        Word h = read_half(a);
        if (signed_value && (h & 0x8000u)) return Wide(h) | 0xffffffffffff0000ULL;
        return Wide(h);
    }
    Word w = read_word(a);
    return signed_value ? extend_signed_word(w) : Wide(w);
}
inline int add(int a, int b) { return signed_word(Word(a) + Word(b)); }
inline int subtract(int a, int b) { return signed_word(Word(a) - Word(b)); }
inline int negate(int a) { return signed_word(0u - Word(a)); }
inline int maximum(int a, int b) { return a < b ? b : a; }
// Header fields are byte-addressed original target words. Genuine __sprint
// resets count/bytes itself even on failure; no fabricated acknowledgement.
struct OutputQueue {
    alignas(16) unsigned char header[12];
    alignas(16) unsigned char entries[64];
    unsigned char* stream;
    Word cursor;
    explicit OutputQueue(unsigned char* file) : stream(file), cursor(address(entries)) {
        write_word(address(header) + 8u, 0u);
        write_word(address(header), address(entries));
        write_word(address(header) + 4u, 0u);
    }
    bool flush() {
        if (norrath_factory_sprint(stream, header) != 0) return false;
        cursor = address(entries);
        return true;
    }
    bool emit(Word data, int length) {
        write_word(cursor + 4u, Word(length));
        write_word(cursor, data);
        cursor += 8u;
        Word count = read_word(address(header) + 4u) + 1u;
        Word total = read_word(address(header) + 8u) + Word(length);
        write_word(address(header) + 8u, total);
        write_word(address(header) + 4u, count);
        return signed_word(count) < 8 || flush();
    }
    bool pad(const unsigned char* data, int count) {
        if (count <= 0) return true;
        while (count > 16) {
            if (!emit(address(data), 16)) return false;
            count = subtract(count, 16);
        }
        return emit(address(data), count);
    }
    Word pending() const { return read_word(address(header) + 8u); }
    void reset_count() { write_word(address(header) + 4u, 0u); }
    void reset_cursor() { cursor = address(entries); }
};
}
