#include "wake_semaphore.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof norrath_sound_wake_semaphore_word_view == 4, "size");
    for (unsigned index = 0; index < sizeof norrath_sound_wake_semaphore_word_view; ++index)
        if (norrath_sound_wake_semaphore_word_view[index] != 0) return 1;
    std::puts("wake_semaphore passed");
    return 0;
}
