// Observed dispEnv at 0x00791030. copyScreen begins at 0x00791060, so
// this object is 48 bytes. The image leaves those bytes zero. initSupersample
// later masks the word at offset 16. The reviewed -G0 profile places this
// zero object in BSS.

extern "C" {
unsigned char dispEnv[48];
}
