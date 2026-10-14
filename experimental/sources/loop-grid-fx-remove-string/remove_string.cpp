// Observed objectRemoveString__FPcP10GameObject at 0x140580, extent 244. Its
// importers (gameobject-base) call it norrath_object_remove_string, and the plan
// rejects aliases, so it is published under that name.
// objectStrings (0x486898) drops by one before anything else. The name hash is
// the one objectFindObject uses (ctype bit 0 folding, 512 slots). The object
// word table objectHash (0x518ac0) is probed from that slot, then at most 522
// following slots (wrapping at 512), for a word equal to the object. The first
// equal word is set to zero. The name pointer table stringHash is not touched.
// A missing object changes only the count.
extern "C" {
unsigned norrath_factory_strlen(const char* text);
extern const unsigned char norrath_factory_ctype_minimum_view[];
extern unsigned char objectHash[];
extern unsigned char objectStrings[];
}

namespace {
using Address = __UINTPTR_TYPE__;
Address address(const void* p) { return reinterpret_cast<Address>(p); }
unsigned char* bytes(Address a) { return reinterpret_cast<unsigned char*>(a); }
unsigned read_word(Address a) {
    const unsigned char* p = bytes(a);
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(Address a, unsigned value) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
int signed_byte(unsigned char value) {
    return value < 128u ? static_cast<int>(value) : static_cast<int>(value) - 256;
}
unsigned arithmetic_shift(unsigned value, unsigned bits) {
    return (value >> bits) | ((value & 0x80000000u) != 0u ? (~0u << (32u - bits)) : 0u);
}
}

extern "C" void norrath_object_remove_string(char* name, void* object) {
    write_word(address(objectStrings), read_word(address(objectStrings)) - 1u);
    const unsigned length = norrath_factory_strlen(name);
    const Address query = address(name);
    unsigned hash = 0u;
    for (unsigned cursor = 0u; static_cast<int>(cursor) < static_cast<int>(length); ++cursor) {
        const int character = signed_byte(*bytes(query + cursor));
        const unsigned flags = norrath_factory_ctype_minimum_view[static_cast<unsigned>(character + 128)];
        const int folded = (flags & 1u) != 0u ? character + 32 : character;
        hash += static_cast<unsigned>(folded);
        hash += hash << 10;
        hash ^= arithmetic_shift(hash, 6u);
    }
    hash += hash << 3;
    hash ^= arithmetic_shift(hash, 11u);
    unsigned slot = hash & 511u;
    const Address table = address(objectHash);
    const unsigned target = static_cast<unsigned>(address(object));
    if (read_word(table + (slot << 2)) != target) {
        for (int probes = 1; probes < 523; ++probes) {
            slot = (slot + 1u) & 511u;
            if (read_word(table + (slot << 2)) == target) goto found;
        }
        return;
    }
found:
    write_word(table + (slot << 2), 0u);
}
