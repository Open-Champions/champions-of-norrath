#include "stream_heap.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof norrath_sound_stream_heap_words_view == 16, "size");
    for (unsigned index = 0; index < sizeof norrath_sound_stream_heap_words_view; ++index)
        if (norrath_sound_stream_heap_words_view[index] != 0) return 1;
    std::puts("stream_heap passed");
    return 0;
}
