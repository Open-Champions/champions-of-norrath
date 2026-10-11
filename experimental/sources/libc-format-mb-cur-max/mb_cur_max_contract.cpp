#include <cstdio>
extern "C" unsigned char norrath_factory_mb_cur_max_word[4];
int main() {
    unsigned value = unsigned(norrath_factory_mb_cur_max_word[0])
        | (unsigned(norrath_factory_mb_cur_max_word[1]) << 8)
        | (unsigned(norrath_factory_mb_cur_max_word[2]) << 16)
        | (unsigned(norrath_factory_mb_cur_max_word[3]) << 24);
    if (value != 1) return 1;
    std::printf("mb_cur_max cases passed\n");
    return 0;
}
