// Observed state words in the pinned SLUS-20565 image, all zero in the file
// image (the image has them in .data or .sbss; here they sit in whatever
// section the -G0 build profile gives zero-initialised objects):
//   curSel 0x00484c78, configMode 0x00484c7c, inGameConfigShuttingDown
//   0x00484c98 and emergencyMusicWaitAbort 0x00484c00 and dialogPacket
//   0x00484360 and needEnforce 0x00485ec0 are 4-byte words;
//   gotPlayerStartThisLevel 0x00484ca8, hudDisplayNetCamToggle 0x00484b01 and
//   netForceLocalUpdate 0x0048382f are single bytes in .data;
//   deathState 0x00486afc and deathNetHack 0x00486afd are single bytes in
//   .sbss (deathState is read as signed and unsigned bytes by gameDeath__Fv);
//   needActAlign 0x00486928 is an object of at most 8 bytes in .sbss.
// Inferred: needActAlign is indexed by pad number 0 to 3 (a byte per pad),
// from the existing pad-processing source that indexes it that way; it is
// given four bytes here although the gap to the next symbol is eight.
// Unknown: the meaning of curSel, configMode, dialogPacket and needEnforce, and
// the type and extent of dialogPacket and needEnforce beyond the symbol size.
extern "C" {
int curSel;
int configMode;
int inGameConfigShuttingDown;
int emergencyMusicWaitAbort;
int dialogPacket;
int needEnforce;
signed char gotPlayerStartThisLevel;
signed char hudDisplayNetCamToggle;
signed char netForceLocalUpdate;
signed char deathState;
signed char deathNetHack;
signed char needActAlign[4];
}
