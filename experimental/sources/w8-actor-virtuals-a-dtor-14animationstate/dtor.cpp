// Observed _$_14AnimationState at 0x00164050, 128 bytes.
// The word at +0 is a pointer. When it is not null, the word at offset 1552
// of the pointed-to block is cleared, then the word at +0. Then the signed
// byte at +5 is a count: for each index below it, the word at +12 + 32 * index
// is a pointer; when not null, the word at offset 20 of the pointed-to block
// and the slot itself are cleared. The count byte is read again on every
// pass, since the stores may reach it. Bit 0 of the incoming flag then
// selects __builtin_delete on the object. Other flag bits do nothing.
// The byte at +4, read by animResetFramestates, is not touched here; whether
// +5 is a second count or a different field is unknown, as is what the two
// pointed-to blocks are.

extern "C" {
void __builtin_delete(void* pointer);
}
#if defined(NORRATH_W8AV_HOST)
extern unsigned long long norrath_w8av_host_base;
#endif

namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
unsigned char* from_word(unsigned word) {
#if defined(NORRATH_W8AV_HOST)
    return reinterpret_cast<unsigned char*>(norrath_w8av_host_base + word);
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
int signed_byte(unsigned char value) {
    return value < 128u ? static_cast<int>(value) : static_cast<int>(value) - 256;
}
}

extern "C" void dtor_14AnimationState(void* object_storage, int flags) asm("_$_14AnimationState");
extern "C" void dtor_14AnimationState(void* object_storage, int flags) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    const unsigned owner = read_word(object);
    if (owner != 0u) {
        write_word(from_word(owner) + 1552u, 0u);
        write_word(object, 0u);
    }
    for (int index = 0; index < signed_byte(object[5]); ++index) {
        unsigned char* slot = object + 12u + 32u * static_cast<unsigned>(index);
        const unsigned block = read_word(slot);
        if (block != 0u) {
            write_word(from_word(block) + 20u, 0u);
            write_word(slot, 0u);
        }
    }
    if ((flags & 1) != 0) __builtin_delete(object);
}
