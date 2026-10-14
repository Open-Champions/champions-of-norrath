/* soundGetFreeRam__Fv at 0x0024f588 is 44 bytes. The result is 0x200000
 * minus effectsBufferSize (0x0048599c). When nmSounds (0x0048594c) is not
 * zero, the word at offset 8 of the last record of soundList (16-byte
 * records) is also subtracted. Only a count of exactly zero skips it, so
 * a negative count indexes before the table.
 */
extern "C" {
extern int effectsBufferSize;
extern int nmSounds;
extern unsigned char soundList[];

int soundGetFreeRam__Fv() {
    int free_bytes = 0x200000 - effectsBufferSize;
    if (nmSounds != 0) {
        int last;
        __builtin_memcpy(&last, soundList + nmSounds * 16 - 8, sizeof last);
        free_bytes -= last;
    }
    return free_bytes;
}
}
