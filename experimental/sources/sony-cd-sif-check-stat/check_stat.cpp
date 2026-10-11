// Observed sceSifCheckStatRpc at 0x002d8468, 60 bytes.
// The word at client+0 is a pointer. Null returns 0. The word at client+4
// must equal the word at that pointer + 24. Bit 0 of the word at pointer + 16
// returns 1 when set and 0 otherwise. What those words mean to the IOP is unknown.
extern "C" {
static unsigned load_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) |
           (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) |
           (static_cast<unsigned>(slot[3]) << 24);
}
#if defined(NORRATH_HOST_CONTRACT)
// The client stores a 32-bit address. The host test names that object with a
// small token because this process does not have a 32-bit address space.
const unsigned char* norrath_sony_cd_resolve(unsigned bits);
#endif
int sceSifCheckStatRpc(void* client) {
    const auto* bytes = static_cast<const unsigned char*>(client);
    const unsigned other_bits = load_word(bytes);
    if (other_bits == 0u) return 0;
#if defined(NORRATH_HOST_CONTRACT)
    const auto* other = norrath_sony_cd_resolve(other_bits);
#else
    const auto* other = reinterpret_cast<const unsigned char*>(static_cast<__UINTPTR_TYPE__>(other_bits));
#endif
    if (load_word(bytes + 4) != load_word(other + 24)) return 0;
    if ((load_word(other + 16) & 1u) != 0u) return 1;
    return 0;
}
}
