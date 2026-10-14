#define NORRATH_HOST_ARENA
#include "sound_free_ram.cpp"
#include "../level-anim-load-common/host_check.hpp"
unsigned effectsBufferSize, nmSounds;
unsigned char soundList[16 * 8];
int main() {
    effectsBufferSize = 0x1000; nmSounds = 0;
    sw_(soundList + 8, 0xdead);              // never read with no sounds
    if (norrath_archive_sound_free_ram() != 0x200000u - 0x1000u) return 1;
    nmSounds = 1;
    if (norrath_archive_sound_free_ram() != 0x200000u - 0x1000u - 0xdeadu) return 2;
    for (unsigned i = 0; i != 8; ++i) sw_(soundList + 16 * i + 8, 0x100 * (i + 1));
    sw_(soundList + 16 * 3 + 4, 0x7777);       // neighbouring words are not used
    sw_(soundList + 16 * 4 + 0, 0x8888);
    nmSounds = 4;
    if (norrath_archive_sound_free_ram() != 0x200000u - 0x1000u - 0x400u) return 3;
    effectsBufferSize = 0x300000;              // wraps like the unsigned subtraction
    nmSounds = 0;
    return norrath_archive_sound_free_ram() == 0x200000u - 0x300000u ? 0 : 4;
}
