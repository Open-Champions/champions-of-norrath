// Host check for the heap pointer word. Not an EE plan input.
#include "heap_pointer.cpp"
extern "C" { unsigned char _end[1]; }
int main() {
    // The word is initialised from the symbol _end (the host supplies one byte).
    return norrath_player_heap_pointer_word == _end ? 0 : 1;
}
