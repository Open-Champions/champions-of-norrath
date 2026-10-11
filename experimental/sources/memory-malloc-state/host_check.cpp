#include <cstdio>
#include <cstring>

extern "C" {
extern unsigned char norrath_player_malloc_trim_threshold_view[8];
extern unsigned char norrath_player_malloc_top_pad_view[8];
extern unsigned char norrath_player_malloc_break_base_view[4];
extern unsigned char norrath_player_malloc_max_break_view[8];
extern unsigned char norrath_player_malloc_max_total_view[8];
extern unsigned char norrath_player_malloc_current_info_view[40];
extern unsigned char norrath_player_malloc_lock_owner_view[4];
extern unsigned char norrath_player_malloc_lock_count_view[4];
extern unsigned char norrath_player_malloc_semaphore_view[4];
}

namespace {
unsigned failures;
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
unsigned long long read_wide(const unsigned char* p) {
    return read_word(p) | (unsigned long long)read_word(p + 4) << 32;
}
void expect(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
}

int main() {
    expect(sizeof(norrath_player_malloc_trim_threshold_view) == 8, "trim size");
    expect(read_wide(norrath_player_malloc_trim_threshold_view) == 0x20000ull, "trim value");
    expect(read_wide(norrath_player_malloc_top_pad_view) == 0ull, "top pad");
    expect(read_word(norrath_player_malloc_break_base_view) == 0xffffffffu, "break base");
    expect(read_wide(norrath_player_malloc_max_break_view) == 0ull, "max break");
    expect(read_wide(norrath_player_malloc_max_total_view) == 0ull, "max total");
    expect(sizeof(norrath_player_malloc_current_info_view) == 40, "mallinfo size");
    unsigned info = 0;
    for (unsigned char byte : norrath_player_malloc_current_info_view) info |= byte;
    expect(info == 0u, "mallinfo zeros");
    expect(read_word(norrath_player_malloc_lock_owner_view) == 0xffffffffu, "lock owner");
    expect(read_word(norrath_player_malloc_lock_count_view) == 0u, "lock count");
    expect(read_word(norrath_player_malloc_semaphore_view) == 0u, "semaphore");
    // A one-byte change to the threshold must fail this wide compare.
    unsigned char mutated[8];
    std::memcpy(mutated, norrath_player_malloc_trim_threshold_view, 8);
    mutated[0] = 1;
    expect(read_wide(mutated) != 0x20000ull, "threshold check can see a low-byte change");
    if (failures != 0) return 1;
    std::printf("malloc scalar checks passed\n");
    return 0;
}
