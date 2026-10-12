#include "cur_player.cpp"
int main() {
    if (sizeof(curPlayer) != 4) return 1;
    if (curPlayer != 0) return 2;
    curPlayer = 1;
    if (curPlayer != 1) return 3;
    return 0;
}
