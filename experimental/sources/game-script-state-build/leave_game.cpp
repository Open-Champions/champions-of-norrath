#include "state.hpp"
extern "C" long long norrath_amx_native_leave_game(void*,const void* parameters) {
    using namespace norrath_constructor_detail;
    const unsigned argument=read_word(address(parameters)+4u);
    write_word(address(norrath_game_bail_from_loop_view),argument==0u?1u:101u);
    return 0;
}
