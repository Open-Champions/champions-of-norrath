#include "alive_sync_state.cpp"
int main() {
    if (sizeof playersAlive != 16 || sizeof syncRandState != 4) return 1;
    for (int i = 0; i < 16; ++i) if (playersAlive[i]) return 2;
    if (syncRandState) return 3;
    return 0;
}
