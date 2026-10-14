#include "restore_stream_count.cpp"

#include <cstdio>
#include <cstring>

int main() {
    static_assert(sizeof norrath_sound_restore_stream_count_condition_view == 30, "size");
    static const unsigned char expected[] = {'n', 'm', 'S', 't', 'r', 'e', 'a', 'm', 's', 'A', 'l', 'l', 'o', 'c', 'a', 't', 'e', 'd', '<', '=', 'N', 'M', 'S', 'T', 'R', 'E', 'A', 'M', 'S', 0};
    static_assert(sizeof expected == 30, "expected size");
    if (std::memcmp(norrath_sound_restore_stream_count_condition_view, expected, 30) != 0) return 1;
    std::puts("restore_stream_count passed");
    return 0;
}
