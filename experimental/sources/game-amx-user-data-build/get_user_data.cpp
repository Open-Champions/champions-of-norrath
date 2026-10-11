#include "user_data.hpp"
extern "C" unsigned long long norrath_amx_get_user_data(
    void* amx, long long tag, void* output_word) {
    using namespace norrath_constructor_detail;
    const Address state = address(amx);
    for (unsigned slot = 0; slot != 4u; ++slot) {
        const long long stored_tag = signed_word(read_word(state + 60u + slot * 4u));
        // Original LW is sign-extended before the full native64 comparison.
        if (stored_tag == tag) {
            const unsigned value = read_word(state + 76u + slot * 4u);
            write_word(address(output_word), value);
            return 0ull;
        }
    }
    return 23ull;
}
