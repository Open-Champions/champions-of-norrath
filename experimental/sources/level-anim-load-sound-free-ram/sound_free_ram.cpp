#include "../level-anim-load-common/mem.hpp"
// Observed soundGetFreeRam__Fv at 0x0024f588, 48 bytes; the loader's import name for it is
// norrath_archive_sound_free_ram. Start from 0x200000 minus the word effectsBufferSize
// (0x0048599c). When the word nmSounds (0x0048594c) is nonzero, also subtract the word at
// offset 8 of the last 16-byte entry of soundList (0x007d9280), that is soundList +
// 16 * nmSounds - 8. All arithmetic is unsigned 32-bit.
// Inferred: the 0x200000 total is the sound memory size and the entry word is that
// sound's end offset, so the result is the unused sound memory; the image states only the
// arithmetic. The loader reads it before and after initializing a lump to learn how much
// sound memory the lump took.
// Unknown: the field names of the sound entries, and nmSounds above the table length.
extern "C" {
extern unsigned effectsBufferSize;
extern unsigned nmSounds;
extern unsigned char soundList[];
unsigned norrath_archive_sound_free_ram();
}
extern "C" unsigned norrath_archive_sound_free_ram() {
    using namespace norrath_lal;
    unsigned free_bytes = 0x200000u - effectsBufferSize;
    if (nmSounds != 0) free_bytes -= load_word(soundList + 16u * nmSounds - 8u);
    return free_bytes;
}
