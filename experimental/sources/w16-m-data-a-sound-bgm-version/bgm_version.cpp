// Storage for the background-music version word read by BgmBufferSize__13VISoundDevice
// (see w15-vi-io-sound-a-sound-device-bgm-buffer-size).
// Observed: the reference image holds a 4-byte word at 0x011b0248 whose value is zero. It sits in the
// SRS-loaded region, and its symbol is named _13VISoundDevice$sBgmVersion.
// Inferred: the word is initialised to zero and written later by the sound setter; its meaning is not
// established here.
// Unknown: the setter that changes it, and any runtime value other than the initial zero.
extern "C" {
int norrath_sound_bgm_version __asm__("_13VISoundDevice$sBgmVersion") = 0;
}
