#include "remote_bind_failure.cpp"

#include <cstdio>
#include <cstring>

int main() {
    static_assert(sizeof norrath_sound_remote_bind_failure_format_view == 35, "size");
    static const unsigned char expected[] = {'s', 'c', 'e', 'S', 'd', 'R', 'e', 'm', 'o', 't', 'e', 'I', 'n', 'i', 't', '(', ')', ' ', 'R', 'P', 'C', ' ', 'b', 'i', 'n', 'd', ' ', 'e', 'r', 'r', 'o', 'r', '!', '\n', 0};
    static_assert(sizeof expected == 35, "expected size");
    if (std::memcmp(norrath_sound_remote_bind_failure_format_view, expected, 35) != 0) return 1;
    std::puts("remote_bind_failure passed");
    return 0;
}
