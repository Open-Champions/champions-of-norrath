// Host contract for objectFindObject__FPcPP10GameObjecti. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" unsigned char stringHash[512 * 4] = {0};
extern "C" unsigned char objectHash[512 * 4] = {0};
extern "C" unsigned char norrath_factory_ctype_minimum_view[384] = {0};
extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** output, int capacity);

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
static int signed_byte(unsigned char value) {
    return value < 128u ? static_cast<int>(value) : static_cast<int>(value) - 256;
}
static unsigned arithmetic_shift(unsigned value, unsigned bits) {
    return (value >> bits) | ((value & 0x80000000u) != 0u ? (~0u << (32u - bits)) : 0u);
}
static unsigned slot_of(const char* name) {
    const unsigned length = static_cast<unsigned>(std::strlen(name));
    unsigned hash = 0u;
    for (unsigned cursor = 0u; cursor < length; ++cursor) {
        const int character = signed_byte(static_cast<unsigned char>(name[cursor]));
        const unsigned flags = norrath_factory_ctype_minimum_view[static_cast<unsigned>(character + 128)];
        const int folded = (flags & 1u) != 0u ? character + 32 : character;
        hash += static_cast<unsigned>(folded);
        hash += hash << 10;
        hash ^= arithmetic_shift(hash, 6u);
    }
    hash += hash << 3;
    hash ^= arithmetic_shift(hash, 11u);
    return hash & 511u;
}
// Stored name words are 32-bit tokens, because the host has wider pointers.
// The strcasecmp stub below resolves a token to its string.
static const char* token_names[64];
static unsigned token_count = 0;
static unsigned token_of(const char* name) {
    for (unsigned i = 0; i < token_count; ++i)
        if (token_names[i] == name) return 0x10000u + i;
    token_names[token_count] = name;
    return 0x10000u + token_count++;
}
static void plant(unsigned slot, const char* name, unsigned object) {
    write_word(stringHash + slot * 4u, token_of(name));
    write_word(objectHash + slot * 4u, object);
}

extern "C" unsigned norrath_factory_strlen(const char* text) {
    return static_cast<unsigned>(std::strlen(text));
}
extern "C" int norrath_factory_strcasecmp(const char* left, const char* right) {
    const auto* a = reinterpret_cast<const unsigned char*>(left);
    const unsigned long token = reinterpret_cast<unsigned long>(right);
    const auto* b = reinterpret_cast<const unsigned char*>(token_names[token - 0x10000u]);
    for (;;) {
        const int ca = signed_byte(*a);
        const int cb = signed_byte(*b);
        const int fa = (norrath_factory_ctype_minimum_view[static_cast<unsigned>(ca + 128)] & 1u) ? ca + 32 : ca;
        const int fb = (norrath_factory_ctype_minimum_view[static_cast<unsigned>(cb + 128)] & 1u) ? cb + 32 : cb;
        if (fa != fb || fa == 0) return fa - fb;
        ++a;
        ++b;
    }
}

int main() {
    std::memset(norrath_factory_ctype_minimum_view, 0, sizeof norrath_factory_ctype_minimum_view);
    for (int ch = 'A'; ch <= 'Z'; ++ch) norrath_factory_ctype_minimum_view[ch + 128] = 1;
    expect(slot_of("a") == 66u, "hash of a is slot 450");
    expect(slot_of("A") == 66u, "uppercase A folds to the same slot");

    std::memset(stringHash, 0, sizeof stringHash);
    std::memset(objectHash, 0, sizeof objectHash);
    unsigned output[8];
    std::memset(output, 0x5a, sizeof output);
    objectFindObject__FPcPP10GameObjecti(const_cast<char*>("a"), reinterpret_cast<void**>(output), 4);
    expect(output[0] == 0u, "empty table writes a null terminator");

    static char name_a[] = "a";
    static char name_A[] = "A";
    static char name_b[] = "b";
    static char other[] = "zz";
    const unsigned slot = slot_of("a");
    plant(slot, other, 0x111u);
    plant((slot + 1u) & 511u, name_A, 0u);
    plant((slot + 2u) & 511u, name_a, 0x222u);
    plant((slot + 3u) & 511u, name_a, 0x333u);
    plant((slot + 4u) & 511u, name_b, 0x444u);
    std::memset(output, 0x5a, sizeof output);
    objectFindObject__FPcPP10GameObjecti(const_cast<char*>("a"), reinterpret_cast<void**>(output), 4);
    expect(output[0] == 0x222u, "skips a different name and a zero object");
    expect(output[1] == 0x333u, "second match");
    expect(output[2] == 0u, "terminator after the matches");
    expect(output[3] == 0x5a5a5a5au, "the fourth word is untouched");

    std::memset(output, 0x5a, sizeof output);
    objectFindObject__FPcPP10GameObjecti(const_cast<char*>("A"), reinterpret_cast<void**>(output), 2);
    expect(output[0] == 0x222u, "capacity 2 keeps one object");
    expect(output[1] == 0u, "capacity 2 still terminates");
    expect(output[2] == 0x5a5a5a5au, "the third word is untouched");

    // Capacity 3 stores two objects and stops before the third match.
    std::memset(stringHash, 0, sizeof stringHash);
    std::memset(objectHash, 0, sizeof objectHash);
    plant(slot, name_a, 0x1u);
    plant((slot + 1u) & 511u, name_a, 0x2u);
    plant((slot + 2u) & 511u, name_a, 0x3u);
    std::memset(output, 0x5a, sizeof output);
    objectFindObject__FPcPP10GameObjecti(const_cast<char*>("a"), reinterpret_cast<void**>(output), 3);
    expect(output[0] == 0x1u && output[1] == 0x2u, "two stored");
    expect(output[2] == 0u, "terminator replaces the third match");

    // Probe wraps from slot 511 to slot 0 when the name hashes to 511.
    // Find a name whose slot is 511 by search over short strings.
    char probe[4] = {0, 0, 0, 0};
    bool found_name = false;
    for (int a = 'a'; a <= 'z' && !found_name; ++a)
        for (int b = 'a'; b <= 'z' && !found_name; ++b) {
            probe[0] = static_cast<char>(a);
            probe[1] = static_cast<char>(b);
            if (slot_of(probe) == 511u) found_name = true;
        }
    expect(found_name, "a two-letter name hashes to slot 511");
    if (found_name) {
        static char wrap_name[4];
        std::memcpy(wrap_name, probe, 4);
        std::memset(stringHash, 0, sizeof stringHash);
        std::memset(objectHash, 0, sizeof objectHash);
        plant(511u, wrap_name, 0x555u);
        plant(0u, wrap_name, 0x666u);
        std::memset(output, 0x5a, sizeof output);
        objectFindObject__FPcPP10GameObjecti(wrap_name, reinterpret_cast<void**>(output), 4);
        expect(output[0] == 0x555u, "starts at slot 511");
        expect(output[1] == 0x666u, "probe wraps to slot 0");
        expect(output[2] == 0u, "terminated after the wrap");
    }

    if (failures != 0) return 1;
    std::printf("find-list contract passed\n");
    return 0;
}
