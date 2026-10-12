/* soundLock__Fi at 0x0024f540 is 68 bytes. For each of the first nmSounds
 * records of soundList (16 bytes each), the signed byte at offset 12 is
 * raised to `level` when it is smaller. A count of zero or less does
 * nothing; a larger byte is left alone.
 */
extern "C" {
extern int nmSounds;
extern unsigned char soundList[];

void soundLock__Fi(int level) {
    if (nmSounds <= 0) return;
    signed char* lock = reinterpret_cast<signed char*>(soundList + 12);
    for (int remaining = nmSounds; remaining != 0; --remaining) {
        if (*lock < level) *lock = static_cast<signed char>(level);
        lock += 16;
    }
}
}
