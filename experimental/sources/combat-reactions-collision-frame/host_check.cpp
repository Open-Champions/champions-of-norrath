#include "frame.cpp"
#include <cstdio>
int main() {
    if (sizeof norrath_collision_frame_bytes != 4u) { std::fprintf(stderr, "fail: size\n"); return 1; }
    for (unsigned i = 0; i < 4u; ++i) if (norrath_collision_frame_bytes[i] != 0u) { std::fprintf(stderr, "fail: nonzero\n"); return 1; }
    return 0;
}
