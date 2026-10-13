// Host contract for mummy_miniboss_name_buffer_7d7e20. Includes the source; not an EE plan input.
#include <cstdio>
#include <cstring>
#include "name_buffer.cpp"

int main() {
    if (sizeof mummy_miniboss_name_buffer_7d7e20 != 16) return 1;
    for (unsigned char b : mummy_miniboss_name_buffer_7d7e20) if (b != 0) return 2;
    // Longest result of the observed format: signed byte -128.
    if (std::snprintf(mummy_miniboss_name_buffer_7d7e20, sizeof mummy_miniboss_name_buffer_7d7e20, "MBMummy%d", -128) != 11) return 3;
    if (std::strcmp(mummy_miniboss_name_buffer_7d7e20, "MBMummy-128") != 0) return 4;
    std::printf("mummy name buffer contract passed\n");
    return 0;
}
