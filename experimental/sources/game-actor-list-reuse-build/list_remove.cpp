#include "list_remove.hpp"
namespace {
using Word = unsigned;
Word address(const unsigned char* p) {
    return static_cast<Word>(reinterpret_cast<__UINTPTR_TYPE__>(p));
}
unsigned char* bytes(Word value) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(value));
}
Word read_word(Word location) {
    const unsigned char* p = bytes(location);
    return Word(p[0]) | (Word(p[1]) << 8) |
           (Word(p[2]) << 16) | (Word(p[3]) << 24);
}
void write_word(Word location, Word value) {
    unsigned char* p = bytes(location);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
}
extern "C" unsigned norrath_list_remove(unsigned char* node) {
    const Word current = address(node);
    const Word head = address(norrath_constructor_list_pool_view);
    Word previous = read_word(current + 4u);
    const Word result = read_word(current + 12u);
    if (previous != 0u) write_word(previous, read_word(current));
    Word next = read_word(current);
    if (next != 0u) write_word(next + 4u, read_word(current + 4u));

    const Word associated = read_word(current + 8u);
    if (associated != 0u) {
        previous = read_word(associated + 4u);
        if (previous != 0u) write_word(previous, read_word(associated));
        next = read_word(associated);
        if (next != 0u) write_word(next + 4u, read_word(associated + 4u));
        write_word(associated, read_word(head));
        write_word(head, associated);
    }
    write_word(current, read_word(head));
    write_word(head, current);
    return result;
}
