// Observed sprintf at 0x00303eb8, the callee of MC_LoadLevelEntities.
// Only the fields that routine stores are initialized. The argument cursor
// is the compiler varargs pointer used by the existing authored sprintf.
extern "C" unsigned char norrath_factory_impure_pointer_word[];
extern "C" int norrath_factory_vfprintf(unsigned char* stream, const char* format, const unsigned char* argument_cursor);

namespace {
using Word = unsigned;
#if !defined(__mips__)
struct HostNote { unsigned key; unsigned char* value; };
HostNote host_notes[8];
int host_note_count;
void note(const void* pointer) {
    auto* value = const_cast<unsigned char*>(static_cast<const unsigned char*>(pointer));
    const unsigned key = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(value));
    for (int i = 0; i < host_note_count; ++i) {
        if (host_notes[i].key == key) { host_notes[i].value = value; return; }
    }
    if (host_note_count < 8) host_notes[host_note_count++] = HostNote{key, value};
}
#endif
Word address_of(const void* pointer) {
#if !defined(__mips__)
    note(pointer);
#endif
    return static_cast<Word>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
unsigned char* bytes(Word value) {
#if !defined(__mips__)
    for (int i = 0; i < host_note_count; ++i) {
        const unsigned key = host_notes[i].key;
        if (value >= key && value - key < 256u) return host_notes[i].value + (value - key);
    }
#endif
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(value));
}
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
void write_half(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
}
}

static int format_into(char* destination, const char* format, __builtin_va_list arguments) {
    const Word reent = read_word(norrath_factory_impure_pointer_word);
    alignas(8) unsigned char stream[88];
    unsigned char* state = stream;
    write_word(state + 20, 0x7fffffffu);
    write_word(state + 8, 0x7fffffffu);
    write_half(state + 12, 0x0208u);
    write_word(state + 16, address_of(destination));
    write_word(state + 84, reent);
    write_word(state, address_of(destination));
    const auto* cursor = reinterpret_cast<const unsigned char*>(arguments);
    const int result = norrath_factory_vfprintf(stream, format, cursor);
    *bytes(read_word(state)) = 0u;
    return result;
}

extern "C" int norrath_format_string(char* destination, const char* format, ...) {
    __builtin_va_list arguments;
    __builtin_va_start(arguments, format);
    const int result = format_into(destination, format, arguments);
    __builtin_va_end(arguments);
    return result;
}

// Same body as norrath_format_string. engineInit calls this name.
#if defined(__mips__)
extern "C" int sprintf(char* destination, const char* format, ...) __attribute__((alias("norrath_format_string")));
#else
extern "C" int sprintf(char* destination, const char* format, ...) {
    __builtin_va_list arguments;
    __builtin_va_start(arguments, format);
    const int result = format_into(destination, format, arguments);
    __builtin_va_end(arguments);
    return result;
}
#endif
