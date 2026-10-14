// Observed strtod at 0x003062c8, 36 bytes. It loads the impure pointer and
// tail-calls _strtod_r. That callee is the authored
// norrath_player_strtod_reentrant. The result is the double bit pattern
// that callee returns.
extern "C" unsigned char norrath_factory_impure_pointer_word[];
extern "C" unsigned long long norrath_player_strtod_reentrant(unsigned char* reent, const char* text, char** end);

static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}

extern "C" unsigned long long strtod(const char* text, char** end) {
    const unsigned impure = read_word(norrath_factory_impure_pointer_word);
    auto* reent = reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(impure));
    return norrath_player_strtod_reentrant(reent, text, end);
}
