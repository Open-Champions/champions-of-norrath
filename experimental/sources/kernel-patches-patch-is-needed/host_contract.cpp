/* Host contract for PatchIsNeeded. Not an EE plan input. */
#include "patch_is_needed.cpp"

#include <cstdio>
#include <cstring>

static unsigned stored_word;      // the word the fake OSD configuration holds
static unsigned accepted_mask;    // bits of a Set that the fake keeps
static char log_text[8];
static int log_used;
static unsigned set_seen[2];
static int set_count;
static const unsigned* get_destination[2];
static int get_count;

static void mark(char step) {
    if (log_used < 7) log_text[log_used++] = step;
    log_text[log_used] = 0;
}

extern "C" void GetOsdConfigParam(unsigned* destination) {
    mark('G');
    if (get_count < 2) get_destination[get_count] = destination;
    ++get_count;
    *destination = stored_word;
}
extern "C" void SetOsdConfigParam(const unsigned* source) {
    mark('S');
    if (set_count < 2) set_seen[set_count] = *source;
    ++set_count;
    stored_word = (stored_word & ~accepted_mask) | (*source & accepted_mask);
}

static int run(unsigned initial, unsigned mask) {
    stored_word = initial;
    accepted_mask = mask;
    log_used = 0;
    log_text[0] = 0;
    set_count = 0;
    get_count = 0;
    return PatchIsNeeded();
}

int main() {
    // A word that accepts bits 13..15: the probe reads back as 1, no patch.
    if (run(0x12345678u, 0xffffffffu) != 0 || std::strcmp(log_text, "GSGS") != 0) return 1;
    if (set_seen[0] != ((0x12345678u & 0xffff1fffu) | 0x2000u)) return 2;
    if (set_seen[1] != 0x12345678u) return 3;
    if (stored_word != 0x12345678u) return 4;
    // The second read targets the probe word, not the saved one.
    if (get_destination[0] == get_destination[1]) return 5;
    // A word that ignores bits 13..15: the field reads back as zero, patch needed.
    if (run(0x00001fffu, 0xffff1fffu) != 1 || std::strcmp(log_text, "GSGS") != 0) return 6;
    if (stored_word != 0x00001fffu) return 7;
    // Other bits of the incoming word survive into the probe.
    run(0xffffe000u, 0xffff1fffu);
    if (set_seen[0] != 0xffff2000u) return 8;
    run(0x0000a5a5u, 0xffffffffu);
    if (set_seen[0] != ((0x0000a5a5u & 0xffff1fffu) | 0x2000u)) return 9;
    // A field that keeps only part of the probe (bit 14) reads back nonzero.
    if (run(0, 0x4000u) != 1) return 10;
    if (run(0, 0x2000u) != 0) return 11;
    std::puts("patch is needed contract passed");
    return 0;
}
