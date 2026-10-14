#include "pause_words.cpp"
int main() {
    static_assert(sizeof(pauseStack) == 5, "distance to pauseStackPos");
    static_assert(sizeof(paused) == 1, "one byte");
    for (unsigned char b : pauseStack) if (b != 0) return 1;
    return paused == 0 ? 0 : 2;
}
