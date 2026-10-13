// Host check for the playerGoldIconx object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "data_player_gold_icon_x.cpp"

int main() {
    if (sizeof(playerGoldIconx) != 16) return 1;
    const int expected[4] = {16, 593, 16, 593};
    for (int i = 0; i < 4; ++i)
        if (playerGoldIconx[i] != expected[i]) return 2;
    return 0;
}
