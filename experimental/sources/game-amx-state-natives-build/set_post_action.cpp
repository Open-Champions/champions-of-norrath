#include "native_state.hpp"
extern "C" long long norrath_amx_native_set_post_action(
    void* amx, const void* parameters) {
    using namespace norrath_constructor_detail;
    char action[160] = {};
    void* cells;
    const long long script_address = static_cast<long long>(
        signed_word(read_word(address(parameters) + 4u)));
    (void)norrath_amx_get_address(amx, script_address, &cells);
    (void)norrath_amx_get_string(action, cells);
    (void)norrath_script_set_post_action(action);
    return 0ll;
}
