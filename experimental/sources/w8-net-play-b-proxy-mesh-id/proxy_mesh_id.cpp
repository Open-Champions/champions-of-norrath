// Observed getProxyMeshId__4itemPQ24item11description at 0x00108498, 68
// bytes, published under the name norrath_player_proxy_mesh_id that the
// authored item-prop constructor imports. The argument is an item
// description. Its first word is a pointer to a record whose signed halfword
// at offset 52 is the base value. The 64-bit word at offset 8 decides the
// result: when bit 36 is set the result is that halfword plus the five bits
// 11..15 of the word; otherwise it is the halfword alone. The result is an int
// (the halfword is sign extended).
// Inferred: the value indexes the proxy-mesh table; the five bits are a level.
// Unknown: the description layout beyond the three fields read here.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned char* norrath_player_host_base;
#endif

namespace {
unsigned char* pointer_from_word(unsigned word) {
#if defined(NORRATH_HOST_CONTRACT)
    return norrath_player_host_base + word;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
}

extern "C" int norrath_player_proxy_mesh_id(void* description) {
    unsigned char* bytes = static_cast<unsigned char*>(description);
    unsigned long long flags = *reinterpret_cast<unsigned long long*>(bytes + 8);
    int level = static_cast<int>((flags >> 11) & 0x1f);
    unsigned char* record = pointer_from_word(*reinterpret_cast<unsigned*>(bytes));
    int base = *reinterpret_cast<short*>(record + 52);
    if ((flags & (0x8000ull << 21)) != 0) return base + level;
    return base;
}
