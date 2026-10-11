// Observed fullFrameMode at 0x004843e4, a 4-byte .sdata zero.
// engineLastAITime begins at 0x004843e8. initSupersample passes this word
// to makeSamplePacket__Fb. The reviewed -G0 profile places the zero in BSS.

extern "C" {
int fullFrameMode;
}
