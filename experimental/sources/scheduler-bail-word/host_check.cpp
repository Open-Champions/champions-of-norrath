#include <cstdio>
extern "C" unsigned char norrath_game_bail_from_loop_view[4];
int main() {
    for (int index = 0; index < 4; ++index) {
        if (norrath_game_bail_from_loop_view[index] != 0) {
            std::fprintf(stderr, "fail: bail word byte %d is %u\n", index,
                         norrath_game_bail_from_loop_view[index]);
            return 1;
        }
    }
    std::printf("bail word host check passed\n");
    return 0;
}
