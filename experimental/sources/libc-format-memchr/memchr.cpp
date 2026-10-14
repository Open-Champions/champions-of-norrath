// Observed memchr at 0x0030b314. The byte scan masks the search value to
// eight bits, treats the count as unsigned, and returns the first match or
// a null pointer. The original also has a 16-byte-aligned quadword scan
// before that loop; this is the same ordinary-memory result, not those
// access widths.
extern "C" const unsigned char* norrath_factory_memchr(const unsigned char* input, int byte, unsigned count) {
    const unsigned char needle = static_cast<unsigned char>(byte);
    for (unsigned index = 0; index < count; ++index) {
        if (input[index] == needle) return input + index;
    }
    return nullptr;
}
