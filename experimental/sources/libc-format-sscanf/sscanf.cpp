// Observed sscanf at 0x00303f50, 168 bytes. It builds a read-only string
// stream, stores the zero-returning eofread at file+32, and calls the
// authored scan of __svfscanf. The same setup already exists as
// norrath_player_sscanf; this is the original symbol startup calls.
extern "C" unsigned norrath_factory_strlen(const char* text);
extern "C" unsigned char norrath_factory_impure_pointer_word[];
extern "C" int norrath_player_string_eof_read(unsigned char*, char*, int);
extern "C" int norrath_player_stream_scan(unsigned char* stream, const char* format, const unsigned char* argument_cursor);

static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
static void write_half(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
}
static unsigned word_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}

extern "C" int sscanf(const char* input, const char* format, ...) {
    alignas(8) unsigned char stream[96];
    write_half(stream + 12, 4u);
    write_word(stream + 16, word_of(input));
    write_word(stream, word_of(input));
    const unsigned size = norrath_factory_strlen(input);
    const unsigned impure = read_word(norrath_factory_impure_pointer_word);
    write_word(stream + 20, size);
    write_word(stream + 32, word_of(reinterpret_cast<const void*>(norrath_player_string_eof_read)));
    write_word(stream + 84, impure);
    write_word(stream + 4, size);
    write_word(stream + 48, 0u);
    write_word(stream + 68, 0u);
    __builtin_va_list arguments;
    __builtin_va_start(arguments, format);
    const int result = norrath_player_stream_scan(stream, format, reinterpret_cast<const unsigned char*>(arguments));
    __builtin_va_end(arguments);
    return result;
}
