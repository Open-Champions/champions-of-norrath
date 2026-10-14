// Observed gameInitSkillRamps__Fv at 0x001d0d20, 96 bytes.
// skills is 252 bytes at 0x0034b440. The walk starts at skills+4. The word at
// skills+0 is not loaded. Each following word is a 32-bit address until a
// zero word. For a nonzero word, the 32-bit value at offset 96 is another
// address. The signed halfword at offset 80 of that second object is added
// to the first address, and the 32-bit value at offset 84 is called with the
// sum. The image's first skills word is 0; the skip does not depend on that.
// Nothing here was executed.
extern "C" unsigned skills[];

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned char* norrath_host_word_pointer(unsigned word);
extern "C" void norrath_host_call_word(unsigned word, unsigned char* argument);
static unsigned char* object_at(unsigned word) { return norrath_host_word_pointer(word); }
static void call_word(unsigned word, unsigned char* argument) { norrath_host_call_word(word, argument); }
#else
static unsigned char* object_at(unsigned word) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
}
static void call_word(unsigned word, unsigned char* argument) {
    using Function = void (*)(unsigned char*);
    Function function = reinterpret_cast<Function>(static_cast<__UINTPTR_TYPE__>(word));
    function(argument);
}
#endif

static unsigned read_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}

static int read_i16(const unsigned char* address) {
    const unsigned value = static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8);
    return value < 0x8000u ? static_cast<int>(value) : static_cast<int>(value) - 65536;
}

extern "C" void gameInitSkillRamps__Fv() {
    const unsigned* cursor = skills + 1;
    while (*cursor != 0u) {
        unsigned char* item = object_at(*cursor);
        unsigned char* table = object_at(read_u32(item + 96));
        const int adjust = read_i16(table + 80);
        call_word(read_u32(table + 84), item + adjust);
        ++cursor;
    }
}
