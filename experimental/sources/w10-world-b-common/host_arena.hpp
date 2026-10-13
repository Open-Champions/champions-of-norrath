// Host-only helper (not a build-plan input): a 1 MiB arena addressed as target words.
#pragma once
static unsigned char norrath_arena[1 << 20];
static const unsigned norrath_arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) {
    return norrath_arena + (address - norrath_arena_base);
}
extern "C" unsigned norrath_host_address(const unsigned char* pointer) {
    return norrath_arena_base + static_cast<unsigned>(pointer - norrath_arena);
}
inline unsigned char* arena_at(unsigned offset) { return norrath_arena + offset; }
