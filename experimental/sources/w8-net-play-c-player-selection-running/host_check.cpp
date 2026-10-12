#include "player_selection_running.cpp"
int playerSelRunning;
int main() {
    int values[] = {0, 1, -1, 0x7fffffff, 0x12345678};
    for (int v : values) {
        playerSelRunning = v;
        if (playerSelectionRunning__Fv() != v) return 1;
    }
    return 0;
}
