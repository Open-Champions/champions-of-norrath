// The megatile entity parser calls the original sscanf at 0x00303f50
// through this reconstruction name. That original is already defined twice
// in the partial link, as sscanf and as norrath_player_sscanf, both with
// the same read-only string-stream setup. This is the same setup under the
// third name: a 96-byte stream with the read flag, base and cursor at the
// input, size and read count from the input length, the zero-returning
// eofread at file+32, the impure pointer as owner, and zeroes at +48/+68,
// then the authored scan of __svfscanf over the varargs cursor.
// Inferred: the three names are one original; the lead decides whether
// they should become one symbol. This unit closes the factory import
// without opening any new one.
// Unknown: float varargs spill below the integer cursor, as with the other
// two definitions.
extern "C" unsigned norrath_factory_strlen(const char* text);
extern "C" unsigned char norrath_factory_impure_pointer_word[];
extern "C" int norrath_player_string_eof_read(unsigned char*, char*, int);
extern "C" int norrath_player_stream_scan(unsigned char* stream, const char* format, const unsigned char* argument_cursor);

namespace {
unsigned read_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}
void write_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
void write_half(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
}
unsigned word_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
} // namespace

extern "C" int norrath_factory_sscanf(const char* input, const char* format, ...) {
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
    const int result = norrath_player_stream_scan(
        stream, format, reinterpret_cast<const unsigned char*>(arguments));
    __builtin_va_end(arguments);
    return result;
}
