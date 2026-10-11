#include "key_words.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof norrath_sound_key_on_words_view == 8, "two key-on words");
    static_assert(sizeof norrath_sound_key_off_words_view == 8, "two key-off words");
    for (unsigned char byte : norrath_sound_key_on_words_view)
        if (byte != 0) return 1;
    for (unsigned char byte : norrath_sound_key_off_words_view)
        if (byte != 0) return 2;
    std::puts("key words passed");
    return 0;
}
