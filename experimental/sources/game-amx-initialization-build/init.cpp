#include "init.hpp"
namespace {
using namespace norrath_constructor_detail;
unsigned half(Address p) {
    return unsigned(*bytes(p)) | (unsigned(*bytes(p + 1u)) << 8);
}
void expand_compact(Address program) {
    const unsigned code_offset = read_word(program + 12u);
    const unsigned image_size = read_word(program);
    const unsigned heap_offset = read_word(program + 20u);
    const Address code = program + code_offset;
    unsigned remaining = image_size - code_offset;
    unsigned output = heap_offset - code_offset;
    while (signed_word(remaining) > 0) {
        unsigned value = 0u;
        unsigned bits = 0u;
        unsigned char last;
        do {
            remaining -= 1u;
            last = *bytes(code + remaining);
            value |= unsigned(last & 0x7fu) << (bits & 31u);
            bits += 7u;
            if (signed_word(remaining) <= 0) break;
        } while ((*bytes(code + remaining - 1u) & 0x80u) != 0u);
        if ((last & 0x40u) != 0u) {
            while (signed_word(bits) < 32) {
                value |= 0xffu << (bits & 31u);
                bits += 8u;
            }
        }
        output -= 4u;
        write_word(code + output, value);
    }
}
}
extern "C" unsigned long long norrath_amx_initialize(void* amx, void* program) {
    using namespace norrath_constructor_detail;
    const Address state = address(amx);
    const Address base = address(program);
    if (half(base + 4u) != 0xf1e0u || half(base + 10u) != 24u)
        return 17ull;
    if (signed_byte(*bytes(base + 7u)) >= 4) return 18ull;
    const unsigned stack_top = read_word(base + 24u);
    if (stack_top == 0u || stack_top > 0x7ffffffeu) return 17ull;
    const unsigned flags = half(base + 8u);
    if ((flags & 1u) != 0u) return 17ull;
    if ((flags & 4u) != 0u) expand_compact(base);
    const unsigned state_flags = read_word(state + 32u);
    if ((state_flags & 0x2000u) != 0u) return 0ull;
    const unsigned callback = read_word(state + 4u);
    write_word(state + 32u, state_flags | 0x2000u);
    write_word(state, base);
    const unsigned top = read_word(base + 24u) - 4u;
    write_word(base + 24u, top);
    write_word(base + top, 0u);
    const unsigned first_data = read_word(base + 16u);
    const unsigned first_heap = read_word(base + 20u);
    write_word(state + 20u, first_heap - first_data);
    const unsigned second_data = read_word(base + 16u);
    const unsigned second_top = read_word(base + 24u);
    const unsigned stack_offset = second_top - second_data;
    write_word(state + 24u, stack_offset);
    write_word(state + 28u, stack_offset);
    if (callback == 0u)
        write_word(state + 4u, address(reinterpret_cast<const void*>(norrath_amx_callback)));
    if (read_word(state + 8u) == 0u)
        write_word(state + 8u, address(reinterpret_cast<const void*>(norrath_amx_debug)));
    write_word(state + 36u, 0u);
    write_word(state + 40u, 0u);
    const unsigned short endian_probe = 255u;
    const unsigned char first_probe_byte = *reinterpret_cast<const unsigned char*>(&endian_probe);
    write_word(address(norrath_amx_little_endian_word_view), first_probe_byte == 255u ? 1u : 0u);
    // The original ignores the genuine relocation routine's status.
    (void)norrath_amx_browse_relocate(amx);
    return 0ull;
}
