// Observed lumpLock__Fi at 0x001662f0, 88 bytes. For each of the nmLumps
// records, a signed lock word at offset 32 that is below the argument is
// raised to the argument. soundLock__Fi then receives the same argument.
// A nonpositive count skips the records and still calls soundLock.
extern "C" {
extern unsigned char norrath_constructor_lump_cache_view[];
extern unsigned char norrath_constructor_lump_count_view[4];
void soundLock__Fi(int value);
static unsigned load_word(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}
static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static int signed_word(unsigned value) {
    return static_cast<int>(value);
}
void lumpLock__Fi(int value) {
    int remaining = signed_word(load_word(norrath_constructor_lump_count_view));
    if (remaining > 0) {
        unsigned char* lock = norrath_constructor_lump_cache_view + 32;
        do {
            const int current = signed_word(load_word(lock));
            if (current < value) store_word(lock, static_cast<unsigned>(value));
            remaining -= 1;
            lock += 40;
        } while (remaining != 0);
    }
    soundLock__Fi(value);
}
}
