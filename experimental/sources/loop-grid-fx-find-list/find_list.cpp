// Observed objectFindObject__FPcPP10GameObjecti at 0x140678, extent 352.
// The name is folded with ctype bit 0, mixed, and masked to 512 slots.
// stringHash is 0x5182c0 and objectHash is 0x518ac0, 512 words each.
// A zero name slot ends the probe. A matching name with a zero object is
// skipped. At most capacity-1 objects are stored, then a zero word.
// strlen and strcasecmp stay the existing factory routines.
extern "C" {
unsigned norrath_factory_strlen(const char* text);
int norrath_factory_strcasecmp(const char* left, const char* right);
extern const unsigned char norrath_factory_ctype_minimum_view[];
extern unsigned char stringHash[];
extern unsigned char objectHash[];
}

namespace {
using Address = __UINTPTR_TYPE__;
Address address(const void* p) { return reinterpret_cast<Address>(p); }
unsigned char* bytes(Address a) { return reinterpret_cast<unsigned char*>(a); }
char* text(Address a) { return reinterpret_cast<char*>(a); }
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

extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** output, int capacity) {
    const unsigned length = norrath_factory_strlen(name);
    const Address query = address(name);
    unsigned hash = 0u;
    unsigned cursor = 0u;
    if (static_cast<int>(length) > 0) {
        do {
            const int character = signed_byte(*bytes(query + cursor));
            cursor += 1u;
            const unsigned flags = norrath_factory_ctype_minimum_view[static_cast<unsigned>(character + 128)];
            const int folded = (flags & 1u) != 0u ? character + 32 : character;
            hash += static_cast<unsigned>(folded);
            hash += hash << 10;
            hash ^= arithmetic_shift(hash, 6u);
        } while (static_cast<int>(cursor) < static_cast<int>(length));
    }
    hash += hash << 3;
    hash ^= arithmetic_shift(hash, 11u);
    unsigned slot = hash & 511u;
    unsigned offset = slot << 2;
    const Address names = address(stringHash);
    const Address objects = address(objectHash);
    const Address destination = address(output);
    unsigned found = 0u;
    Address write_cursor = destination;
    if (read_word(names + offset) != 0u) {
        const unsigned stop_count = static_cast<unsigned>(capacity) - 1u;
        for (;;) {
            const int comparison = norrath_factory_strcasecmp(name, text(read_word(names + offset)));
            slot = (slot + 1u) & 511u;
            const Address current_object_slot = objects + offset;
            offset = slot << 2;
            if (comparison == 0) {
                const unsigned object = read_word(current_object_slot);
                if (object != 0u) {
                    found += 1u;
                    write_word(write_cursor, object);
                    write_cursor += 4u;
                    if (found == stop_count) break;
                }
            }
            if (read_word(names + offset) == 0u) break;
        }
    }
    write_word(destination + (found << 2), 0u);
}
