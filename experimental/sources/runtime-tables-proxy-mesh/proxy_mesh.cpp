// Observed getProxyMeshId__4itemPQ24item11description at 0x108498, 68
// bytes. The item pointer the method is called on is never read; only the
// description pointer matters. The 64-bit word at description offset 8 is
// tested for bit 0x100000. Word 0 is a pointer whose signed halfword at
// offset 52 is the base mesh id. When the bit is set, the low five bits of
// the 64-bit word are added to that halfword, otherwise the halfword alone
// is returned; the addition wraps mod 2^32.
// Inferred: this reconstruction name is that computation as a single
// argument function, matching how the item constructor calls it.
// Unknown: why the item pointer is ignored, and what the flag bit and the
// pointed-to record at offset 52 mean beyond this use.
#if defined(NORRATH_PROXY_MESH_HOST)
// A host pointer does not fit in a 32-bit object word. Tests register each
// record with norrath_proxy_mesh_host_note so word loads resolve back to
// the full pointer; unregistered words fall back to a raw reinterpret.
struct ProxyMeshHostRow {
    unsigned bits;
    const unsigned char* full;
};
inline ProxyMeshHostRow proxy_mesh_host_rows[8] = {};
inline void norrath_proxy_mesh_host_note(const unsigned char* full) {
    const unsigned bits = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(full));
    for (unsigned index = 0u; index < 8u; ++index) {
        if (proxy_mesh_host_rows[index].full == nullptr || proxy_mesh_host_rows[index].bits == bits) {
            proxy_mesh_host_rows[index].bits = bits;
            proxy_mesh_host_rows[index].full = full;
            return;
        }
    }
}
inline const unsigned char* proxy_mesh_word_pointer(unsigned bits) {
    for (unsigned index = 0u; index < 8u; ++index) {
        if (proxy_mesh_host_rows[index].full != nullptr && proxy_mesh_host_rows[index].bits == bits)
            return proxy_mesh_host_rows[index].full;
    }
    return reinterpret_cast<const unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
}
#else
inline const unsigned char* proxy_mesh_word_pointer(unsigned bits) {
    return reinterpret_cast<const unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
}
#endif

namespace {
unsigned read_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}
int read_half(const unsigned char* slot) {
    const int low = slot[0] | (slot[1] << 8);
    return (low & 0x8000) != 0 ? low - 0x10000 : low;
}
} // namespace

extern "C" int norrath_player_proxy_mesh_id(void* description) {
    const auto* bytes = static_cast<const unsigned char*>(description);
    const unsigned low = read_word(bytes + 8);
    const unsigned high = read_word(bytes + 12);
    const unsigned long long whole = (static_cast<unsigned long long>(high) << 32) | low;
    const unsigned char* base = proxy_mesh_word_pointer(read_word(bytes));
    const int mesh = read_half(base + 52);
    if ((whole & 0x100000ull) != 0ull) {
        const unsigned sum = static_cast<unsigned>(mesh) + static_cast<unsigned>(whole & 0x1full);
        return static_cast<int>(sum);
    }
    return mesh;
}
