#include "script_start.hpp"
#include "dependencies.hpp"
namespace {
using namespace norrath_constructor_detail;
int read_signed_half(Address location) {
    const unsigned value = norrath_constructor_dependency_detail::read_half(location);
    return value < 0x8000u ? static_cast<int>(value)
                         : static_cast<int>(value) - 65536;
}
}
extern "C" unsigned long long norrath_amx_find_public(
    void* amx, const char* name, int* public_index) {
    using namespace norrath_constructor_detail;
    int lower = 0;
    const Address first_header = read_word(address(amx));
    int upper = read_signed_half(first_header + 32u) - 1;
    // Observed original scratch starts at SP16, below the first save at SP48.
    // Keep it unwritten until the genuine copy; no synthetic name/result seed.
    alignas(16) char public_name[32];
    while (lower <= upper) {
        // The original rounds signed addition/division toward zero. The initial
        // signed16 count bounds the active search indices and their sum.
        const int middle = (lower + upper) / 2;
        const Address header = read_word(address(amx));
        const int current_count = read_signed_half(header + 32u);
        if (middle < current_count) {
            const unsigned table_offset = read_word(header + 36u);
            const unsigned record_offset = static_cast<unsigned>(middle) * 24u;
            norrath_factory_strcpy(public_name,
                text(header + table_offset + record_offset + 4u));
        }
        const int comparison = norrath_string_compare(public_name, name);
        if (comparison > 0) {
            upper = middle - 1;
        } else {
            // Original ADDIU in the BLTZ delay executes on equality too.
            lower = middle + 1;
            if (comparison == 0) {
                write_word(address(public_index), static_cast<unsigned>(middle));
                return 0ull;
            }
        }
    }
    write_word(address(public_index), 0x7fffffffu);
    return 19ull;
}
