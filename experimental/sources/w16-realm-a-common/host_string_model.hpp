// Host checks only; not a build input. A small model of the old reference-counted string library that the realm
// interface routines call, with 32-bit "words" translated to host pointers through an arena and a table of the
// few static objects (the shared empty representations and the free list).
#pragma once
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

// The including source defines struct StringRep { unsigned length, capacity; int references, shared; } first.

namespace hostmodel {
static unsigned char arena[1 << 20];
static unsigned arena_used;
struct Mapped { void* pointer; unsigned size; unsigned word; };
static std::vector<Mapped> mapped;
static const unsigned ARENA_BASE = 0x00100000;
inline void map_static(void* pointer, unsigned size, unsigned word) { mapped.push_back({pointer, size, word}); }
inline unsigned word_of(const void* pointer) {
    const unsigned char* p = static_cast<const unsigned char*>(pointer);
    if (p >= arena && p < arena + sizeof arena) return ARENA_BASE + static_cast<unsigned>(p - arena);
    for (auto& m : mapped)
        if (p >= m.pointer && p < static_cast<unsigned char*>(m.pointer) + m.size) return m.word + static_cast<unsigned>(p - static_cast<unsigned char*>(m.pointer));
    return 0xdeadbeef;
}
inline void* pointer_of(unsigned word) {
    if (word >= ARENA_BASE && word < ARENA_BASE + sizeof arena) return arena + (word - ARENA_BASE);
    for (auto& m : mapped)
        if (word >= m.word && word < m.word + m.size) return static_cast<unsigned char*>(m.pointer) + (word - m.word);
    return nullptr;
}
inline void reset_arena() { std::memset(arena, 0, sizeof arena); arena_used = 0; mapped.clear(); }
// Allocates a representation with `bytes` of data (plus one terminator character) and returns the data word.
inline unsigned new_rep(const void* data, unsigned length, unsigned char_size, unsigned capacity, int references, int shared) {
    unsigned size = (16 + (capacity > length ? capacity : length) * char_size + char_size + 16 + 7) / 8 * 8;
    unsigned char* rep = arena + arena_used;
    arena_used += size;
    StringRep* r = reinterpret_cast<StringRep*>(rep);
    r->length = length; r->capacity = capacity; r->references = references; r->shared = shared;
    if (data) std::memcpy(rep + 16, data, length * char_size);
    return word_of(rep + 16);
}
inline StringRep* rep_of_word(unsigned data) { return static_cast<StringRep*>(pointer_of(data - 16)); }
inline unsigned word_at(void* slot) { return *static_cast<unsigned*>(slot); }
inline void set_word(void* slot, unsigned w) { *static_cast<unsigned*>(slot) = w; }
}
