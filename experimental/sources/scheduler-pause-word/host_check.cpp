#include <cstdio>
extern "C" unsigned char norrath_game_pause_word_view[4];
int main() {
    for (int index = 0; index < 4; ++index) {
        if (norrath_game_pause_word_view[index] != 0) {
            std::fprintf(stderr, "fail: pause word byte %d is %u\n", index,
                         norrath_game_pause_word_view[index]);
            return 1;
        }
    }
    std::printf("pause word host check passed\n");
    return 0;
}
