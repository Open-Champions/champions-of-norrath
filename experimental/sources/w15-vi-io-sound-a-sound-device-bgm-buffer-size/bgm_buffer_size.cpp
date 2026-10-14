// Observed: BgmBufferSize__13VISoundDevice at 0x011265c8, 36 bytes. Reads the static word _13VISoundDevice$sBgmVersion
// (0x011b0248). The result is 0x20000 when that word is 1, and 0x8000 for every other value (0 and anything else). No other state
// is read or written.
// Inferred: the word is the background-music stream version selected through SetBgmVersion__13VISoundDevicei; the result is a byte
// size (twice 0x10000 or twice 0x4000).
// Unknown: what version 1 selects, and what the doubling represents.
extern int norrath_sound_bgm_version __asm__("_13VISoundDevice$sBgmVersion");
extern "C" {
int BgmBufferSize__13VISoundDevice(unsigned char*) {
    int size = norrath_sound_bgm_version == 1 ? 0x10000 : 0x4000;
    return size * 2;
}
}
