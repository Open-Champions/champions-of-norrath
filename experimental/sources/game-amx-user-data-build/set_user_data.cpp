#include "user_data.hpp"
extern "C" unsigned long long norrath_amx_set_user_data(
    void* amx, long long tag, unsigned long long user_data) {
    using namespace norrath_constructor_detail;
    const Address state = address(amx);
    unsigned slot = 0;
    while (slot != 4u) {
        const long long stored_tag = signed_word(read_word(state + 60u + slot * 4u));
        if (stored_tag == tag) break;
        ++slot;
    }
    if (slot == 4u) {
        // The original restarts at tag zero only after the whole tag search.
        slot = 0;
        while (slot != 4u && read_word(state + 60u + slot * 4u) != 0u)
            ++slot;
        if (slot == 4u) return 20ull;
    }
    // Preserve source publication order: payload first, then low32 tag.
    write_word(state + 76u + slot * 4u, static_cast<unsigned>(user_data));
    write_word(state + 60u + slot * 4u, static_cast<unsigned>(tag));
    return 0ull;
}
