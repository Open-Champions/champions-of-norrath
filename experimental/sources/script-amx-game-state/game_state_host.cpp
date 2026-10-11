#include <cstdio>
extern "C" unsigned char norrath_script_game_state_data_view[8192];
int main() {
    if (sizeof norrath_script_game_state_data_view != 8192) return 1;
    if (norrath_script_game_state_data_view[0] != 0) return 2;
    if (norrath_script_game_state_data_view[8191] != 0) return 3;
    const char* name = "open";
    unsigned i = 0;
    for (; name[i] != 0; ++i) norrath_script_game_state_data_view[i] = static_cast<unsigned char>(name[i]);
    norrath_script_game_state_data_view[i] = 0;
    norrath_script_game_state_data_view[i + 1] = 7;
    norrath_script_game_state_data_view[i + 2] = 0;
    if (norrath_script_game_state_data_view[4] != 0 || norrath_script_game_state_data_view[5] != 7) return 4;
    std::puts("game state passed");
}
