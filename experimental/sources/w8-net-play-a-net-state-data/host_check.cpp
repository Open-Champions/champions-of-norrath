#include "net_state_data.cpp"
#include <cstring>
int main() {
    if (validLocalPlayer || nmPlayersAlive || localNmPlayersAlive || lastNmLocalPlayersAlive) return 1;
    if (sizeof(playersAlive) != 16 || sizeof(localPlayersAlive) != 16) return 2;
    for (int i = 0; i < 4; ++i) if (playersAlive[i] || localPlayersAlive[i]) return 3;
    if (comm_handle != -1 || commRVInternal != 25 || commRVThresh != 10000 || commPV != 100) return 4;
    if (sizeof(norrath_player_select_level_bytes_484c40) != 8) return 5;
    if (std::memcmp(norrath_player_select_level_bytes_484c40, "kelsel2", 8) != 0) return 6;
    return 0;
}
