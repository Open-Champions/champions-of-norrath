// Observed button words. controlStyle is the one byte 0x01 at 0x00483f44.
// padAcceptButton is the word 0x00000040 at 0x0048404c. padBackButton is the
// word 0x00000120 at 0x00484048. padVibrationEnable is the word 1 at
// 0x00484040. The other words below are zero. Sizes are the ELF sizes.
extern "C" unsigned char controlStyle = 0x01;
extern "C" unsigned dropButton = 0u;
extern "C" unsigned dropButtonFontChar = 0u;
extern "C" unsigned equipButton = 0u;
extern "C" unsigned equipButtonFontChar = 0u;
extern "C" unsigned infoButton = 0u;
extern "C" unsigned infoButtonFontChar = 0u;
extern "C" unsigned padAcceptButton = 0x40u;
extern "C" unsigned padAcceptButtonFontChar = 0u;
extern "C" unsigned padBackButton = 0x120u;
extern "C" unsigned padBackButtonFontChar = 0u;
extern "C" unsigned padVibrationEnable = 1u;
