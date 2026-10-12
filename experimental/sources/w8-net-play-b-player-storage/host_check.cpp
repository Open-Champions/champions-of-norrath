// Host check for the player storage. Not an EE plan input.
#include "player_storage.cpp"
int main() {
    static_assert(sizeof(player) == 4 * 23984, "four records");
    for (unsigned char b : player) if (b) return 1;
    return 0;
}
