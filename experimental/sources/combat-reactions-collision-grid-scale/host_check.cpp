#include "grid_scale.cpp"
#include <cstdio>
#include <cstring>
int main() {
    if (sizeof norrath_collision_grid_scale_bytes != 4u) { std::fprintf(stderr, "fail: size\n"); return 1; }
    unsigned bits = 0;
    for (unsigned i = 0; i < 4u; ++i) bits |= static_cast<unsigned>(norrath_collision_grid_scale_bytes[i]) << (8u * i);
    if (bits != 0x3bccccccu) { std::fprintf(stderr, "fail: bits %08x\n", bits); return 1; }
    float value;
    std::memcpy(&value, norrath_collision_grid_scale_bytes, 4);
    if (!(value > 0.00624f && value < 0.00626f)) { std::fprintf(stderr, "fail: value\n"); return 1; }
    return 0;
}
