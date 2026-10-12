// Observed findTagString__10GameObjectPci at 0x143a80, extent 304.
// The object's tag list is the word at object+44: a zero-terminated array of
// string pointers. A zero word there returns null. The query is lowercased in
// place with ctype bit 0 (adding 32), then each entry whose first strlen
// bytes match (strncmp) and whose next byte is '=' counts as one hit. After
// index + 1 hits the address just past that '=' is returned. Without enough
// hits the result is null. The list word is reread after every compare.
extern "C" {
unsigned norrath_factory_strlen(const char* text);
int norrath_factory_strncmp(const char* left, const char* right, unsigned count);
extern const unsigned char norrath_factory_ctype_minimum_view[];
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void* norrath_host_pointer(unsigned address);
#endif

namespace {
using Address = __UINTPTR_TYPE__;
unsigned char* bytes(Address a) {
#if defined(NORRATH_HOST_CONTRACT)
    return static_cast<unsigned char*>(norrath_host_pointer(static_cast<unsigned>(a)));
#else
    return reinterpret_cast<unsigned char*>(a);
#endif
}
unsigned read_word(Address a) {
    const unsigned char* p = bytes(a);
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
int signed_byte(unsigned char value) {
    return value < 128u ? static_cast<int>(value) : static_cast<int>(value) - 256;
}
}

extern "C" char* findTagString__10GameObjectPci(void* object, char* name, int index) {
    const Address self = reinterpret_cast<__UINTPTR_TYPE__>(object);
    if (read_word(self + 44u) == 0u) return nullptr;
    unsigned char* cursor = reinterpret_cast<unsigned char*>(name);
    while (*cursor != 0u) {
        const int character = signed_byte(*cursor);
        const int folded = (norrath_factory_ctype_minimum_view[static_cast<unsigned>(character + 128)] & 1u) != 0u
                               ? character + 32
                               : character;
        *cursor = static_cast<unsigned char>(folded);
        ++cursor;
    }
    const unsigned length = norrath_factory_strlen(name);
    unsigned tags = read_word(self + 44u);
    if (read_word(tags) == 0u) return nullptr;
    unsigned offset = 0u;
    for (;;) {
        const unsigned entry = read_word(tags + offset);
        const int compare = norrath_factory_strncmp(reinterpret_cast<const char*>(bytes(entry)), name, length);
        tags = read_word(self + 44u);
        offset += 4u;
        if (compare == 0 && signed_byte(bytes(entry)[length]) == 61) {
            index -= 1;
            if (index == -1) return reinterpret_cast<char*>(bytes(entry + length + 1u));
        }
        if (read_word(tags + offset) == 0u) return nullptr;
    }
}
