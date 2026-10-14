#include "address_string.hpp"
extern "C" unsigned long long norrath_amx_get_address(
    void* amx, long long script_address, void** physical_address) {
    using namespace norrath_constructor_detail;
    const Address state = address(amx);
    const long long heap = signed_word(read_word(state + 20u));
    const bool below_heap = script_address < heap;
    // Original branch delay reads the header on every path.
    const Address header = read_word(state);
    if (!below_heap) {
        const long long stack = signed_word(read_word(state + 24u));
        if (script_address < stack) return 5ull;
    }
    const long long stack_top = signed_word(read_word(state + 28u));
    if (script_address >= stack_top) return 5ull;
    const unsigned data_offset = read_word(header + 16u);
    const Address result = header + data_offset + static_cast<unsigned>(script_address);
    write_word(address(physical_address), result);
    return 0ull;
}
