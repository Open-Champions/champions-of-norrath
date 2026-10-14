// Observed __swsetup at 0x003093c0. A missing stream owner is replaced with
// the impure pointer. A zero value at reentrancy offset 56 calls stream
// initialization. Writing is enabled when the stream is already writable or
// read/write capable; a read-to-write switch drops the ungetc buffer and
// clears the read and end-of-file bits. An absent buffer is created, then
// the write count and line-buffer size follow the flag bits.
extern "C" unsigned char norrath_factory_impure_pointer_word[];
extern "C" void norrath_factory_streams_init(unsigned char* reent);
extern "C" void norrath_player_free_reentrant(unsigned char* reent, unsigned char* allocation);
extern "C" void norrath_player_stream_make_buffer(unsigned char* stream);

#if !defined(__mips__)
struct NorrathHostNote { unsigned key; unsigned char* value; };
static NorrathHostNote norrath_host_notes[32];
static int norrath_host_note_count;
extern "C" void norrath_host_note(const void* pointer) {
    auto* value = const_cast<unsigned char*>(static_cast<const unsigned char*>(pointer));
    const unsigned key = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(value));
    for (int i = 0; i < norrath_host_note_count; ++i) {
        if (norrath_host_notes[i].key == key) { norrath_host_notes[i].value = value; return; }
    }
    if (norrath_host_note_count < 32) norrath_host_notes[norrath_host_note_count++] = NorrathHostNote{key, value};
}
static unsigned char* norrath_host_lookup(unsigned key) {
    for (int i = 0; i < norrath_host_note_count; ++i)
        if (norrath_host_notes[i].key == key) return norrath_host_notes[i].value;
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(key));
}
#endif

static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned read_half(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8);
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
#if !defined(__mips__)
    norrath_host_note(pointer);
#endif
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
static unsigned char* from_word(unsigned value) {
#if !defined(__mips__)
    return norrath_host_lookup(value);
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(value));
#endif
}

extern "C" int norrath_factory_stream_write_setup(unsigned char* stream) {
    unsigned char* file = stream;
    unsigned reent = read_word(file + 84);
    if (reent == 0u) {
        reent = read_word(norrath_factory_impure_pointer_word);
        write_word(file + 84, reent);
    }
    if (read_word(from_word(reent) + 56) == 0u) norrath_factory_streams_init(from_word(reent));
    unsigned flags = read_half(file + 12);
    unsigned base;
    if ((flags & 8u) == 0u) {
        if ((flags & 0x10u) == 0u) return -1;
        if ((flags & 4u) != 0u) {
            const unsigned ub = read_word(file + 48);
            const unsigned tiny = word_of(file + 64);
            if (ub != 0u) {
                if (ub != tiny) {
                    norrath_player_free_reentrant(from_word(read_word(file + 84)), from_word(ub));
                    flags = read_half(file + 12);
                }
                write_word(file + 48, 0u);
            }
            flags &= ~0x24u;
            write_word(file + 4, 0u);
            write_half(file + 12, flags);
            base = read_word(file + 16);
            write_word(file, base);
        } else {
            base = read_word(file + 16);
        }
        flags |= 8u;
        write_half(file + 12, flags);
    } else {
        base = read_word(file + 16);
    }
    if (base == 0u) norrath_player_stream_make_buffer(stream);
    flags = read_half(file + 12);
    if ((flags & 1u) != 0u) {
        const unsigned size = read_word(file + 20);
        write_word(file + 8, 0u);
        write_word(file + 24, 0u - size);
    } else if ((flags & 2u) != 0u) {
        write_word(file + 8, 0u);
    } else {
        write_word(file + 8, read_word(file + 20));
    }
    return 0;
}
