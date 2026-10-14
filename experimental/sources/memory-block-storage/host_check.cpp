#include <cstdio>
#include <cstring>

extern "C" unsigned char norrath_block_data_pool_view[0x000e7700];
extern "C" unsigned char norrath_block_region_view[64];

namespace {
unsigned failures;
void expect(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
}

int main() {
    expect(sizeof norrath_block_data_pool_view == 0x000e7700u, "pool extent");
    expect(norrath_block_data_pool_view[0] == 0 &&
           norrath_block_data_pool_view[0x000e76ff] == 0 &&
           norrath_block_data_pool_view[384u * 512u] == 0, "pool starts clear");
    const unsigned char expected[64] = {
        0x80, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x05, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xb8, 0x0b, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    expect(std::memcmp(norrath_block_region_view, expected, 64) == 0, "region bytes");
    expect(read_word(norrath_block_region_view) == 384u, "first capacity");
    expect(read_word(norrath_block_region_view + 4) == 512u, "first count");
    expect(read_word(norrath_block_region_view + 16) == 1280u, "second capacity");
    expect(read_word(norrath_block_region_view + 32) == 3000u, "third capacity");
    expect(read_word(norrath_block_region_view + 36) == 32u, "third count");
    expect(read_word(norrath_block_region_view + 48) == 0x7fffffffu, "sentinel capacity");
    expect(read_word(norrath_block_region_view + 60) == 0u, "final head starts empty");
    if (failures != 0) return 1;
    std::printf("block storage checks passed\n");
    return 0;
}
