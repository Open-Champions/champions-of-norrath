#include <cstdio>
extern "C" unsigned char norrath_game_frame_number_word_view[4];
int main() {
    for (int index = 0; index < 4; ++index) {
        if (norrath_game_frame_number_word_view[index] != 0) {
            std::fprintf(stderr, "fail: frame word byte %d is %u\n", index,
                         norrath_game_frame_number_word_view[index]);
            return 1;
        }
    }
    std::printf("frame number host check passed\n");
    return 0;
}
