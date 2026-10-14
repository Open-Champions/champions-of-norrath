#include "native_state.hpp"
extern "C" long long norrath_amx_native_get_game_state(
    void* amx, const void* parameters) {
    using namespace norrath_constructor_detail;
    // Private defined padding, within the documented no-stack-alias domain.
    char name[160] = {};
    void* cells;
    const long long script_address = static_cast<long long>(
        signed_word(read_word(address(parameters) + 4u)));
    // The original ignores this status. The admitted domain requires success.
    (void)norrath_amx_get_address(amx, script_address, &cells);
    (void)norrath_amx_get_string(name, cells);
    return norrath_script_get_game_state(name);
}
