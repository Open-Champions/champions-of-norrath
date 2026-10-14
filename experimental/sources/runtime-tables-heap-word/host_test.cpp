// Host comparison only. Not an EE build input.
#include "heap_word.cpp"

#include <cstdio>

int main() {
    if (sizeof norrath_player_heap_pointer_word != 4u) return 1;
    // Zero-initialized: the first break call establishes the value.
    for (int i = 0; i < 4; ++i) {
        if (norrath_player_heap_pointer_word[i] != 0) return 2;
    }
    // Writable, as the break helper stores through it.
    norrath_player_heap_pointer_word[0] = 1;
    if (norrath_player_heap_pointer_word[0] != 1) return 3;
    norrath_player_heap_pointer_word[0] = 0;
    return 0;
}
