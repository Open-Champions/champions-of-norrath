// Host contract for norrath_object_remove_string (objectRemoveString__FPcP10GameObject). Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" unsigned char objectHash[512 * 4] = {0};
extern "C" unsigned char objectStrings[4] = {0};
extern "C" unsigned char norrath_factory_ctype_minimum_view[384] = {0};
extern "C" void norrath_object_remove_string(char* name, void* object);

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static unsigned word_at(const unsigned char* base, unsigned index) {
    unsigned v;
    std::memcpy(&v, base + index * 4u, 4);
    return v;
}
static void set_word(unsigned char* base, unsigned index, unsigned v) {
    std::memcpy(base + index * 4u, &v, 4);
}
extern "C" unsigned norrath_factory_strlen(const char* text) {
    return static_cast<unsigned>(std::strlen(text));
}
static int signed_byte(unsigned char value) { return value < 128u ? value : int(value) - 256; }
static unsigned sar(unsigned value, unsigned bits) {
    return (value >> bits) | ((value & 0x80000000u) ? (~0u << (32u - bits)) : 0u);
}
static unsigned slot_of(const char* name) {
    unsigned hash = 0;
    for (const char* p = name; *p; ++p) {
        const int c = signed_byte(static_cast<unsigned char>(*p));
        const int f = (norrath_factory_ctype_minimum_view[c + 128] & 1u) ? c + 32 : c;
        hash += static_cast<unsigned>(f);
        hash += hash << 10;
        hash ^= sar(hash, 6u);
    }
    hash += hash << 3;
    hash ^= sar(hash, 11u);
    return hash & 511u;
}

int main() {
    for (int ch = 'A'; ch <= 'Z'; ++ch) norrath_factory_ctype_minimum_view[ch + 128] = 1;
    char name[] = "Gate";
    const unsigned slot = slot_of("gate");
    expect(slot_of("Gate") == slot, "hash folds case");

    set_word(objectStrings, 0, 10u);
    set_word(objectHash, slot, 0x1000u);
    norrath_object_remove_string(name, reinterpret_cast<void*>(0x1000u));
    expect(word_at(objectStrings, 0) == 9u, "count drops by one");
    expect(word_at(objectHash, slot) == 0u, "direct hit is cleared");

    // Object a few slots past its home slot, with other objects in between.
    std::memset(objectHash, 0, sizeof objectHash);
    set_word(objectHash, slot, 0x2000u);
    set_word(objectHash, (slot + 1u) & 511u, 0x3000u);
    set_word(objectHash, (slot + 2u) & 511u, 0x4000u);
    norrath_object_remove_string(name, reinterpret_cast<void*>(0x4000u));
    expect(word_at(objectHash, slot) == 0x2000u && word_at(objectHash, (slot + 1u) & 511u) == 0x3000u,
           "earlier slots are untouched");
    expect(word_at(objectHash, (slot + 2u) & 511u) == 0u, "probed object is cleared");
    expect(word_at(objectStrings, 0) == 8u, "count drops again");

    // The probe wraps and stops at 522 steps; a missing object only drops the count.
    std::memset(objectHash, 0, sizeof objectHash);
    set_word(objectHash, 0, 0x5000u);
    set_word(objectHash, 511, 0x6000u);
    norrath_object_remove_string(name, reinterpret_cast<void*>(0x9999u));
    expect(word_at(objectHash, 0) == 0x5000u && word_at(objectHash, 511) == 0x6000u, "missing object leaves the table");
    expect(word_at(objectStrings, 0) == 7u, "missing object still drops the count");

    // Only the first equal word is cleared (the table can hold the object twice).
    std::memset(objectHash, 0, sizeof objectHash);
    set_word(objectHash, (slot + 3u) & 511u, 0x7000u);
    set_word(objectHash, (slot + 5u) & 511u, 0x7000u);
    norrath_object_remove_string(name, reinterpret_cast<void*>(0x7000u));
    expect(word_at(objectHash, (slot + 3u) & 511u) == 0u && word_at(objectHash, (slot + 5u) & 511u) == 0x7000u,
           "only the first match is cleared");

    // A slot reached only after wrapping past 511 is still found. The 522 probe
    // limit exceeds the 512 slots, so the limit itself is not observable here.
    std::memset(objectHash, 0, sizeof objectHash);
    set_word(objectHash, (slot + 300u) & 511u, 0x8000u);
    norrath_object_remove_string(name, reinterpret_cast<void*>(0x8000u));
    expect(word_at(objectHash, (slot + 300u) & 511u) == 0u, "the wrapped slot is reached within 522 probes");
    if (failures != 0) return 1;
    std::printf("remove-string contract passed\n");
    return 0;
}
