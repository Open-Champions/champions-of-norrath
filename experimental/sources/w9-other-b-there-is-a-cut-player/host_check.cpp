// Host check for there_is_a_cut_player.cpp; not an EE plan input.
#include "there_is_a_cut_player.cpp"

int main() {
    if (thereIsACutPlayer != 0) return 1;
    thereIsACutPlayer = 1;
    if (thereIsACutPlayer != 1) return 2;
    return 0;
}
