// Observed: four zero-initialized objects in the pinned SLUS-20565 image.
//   menuModeRebootQuestion 0x00484c04: one byte in .sdata, image value 0.
//   title 0x00486ad4: four bytes in .sbss, a pointer to 16-bit text in the
//     authored help-message source (stored there, read by its frame code).
//   ranBrowser 0x00486c6c and ranConfig 0x00486c78: four-byte words in
//     .sbss between browserState/configState neighbours; MC_SaveGameBrowser
//     clears both before starting the card browser.
// Inferred: menuModeRebootQuestion is a flag runFrontEnd sets to 1 and clears
// when the answer is 1. Unknown: what ranBrowser and ranConfig record once
// set, since no authored function stores a nonzero value to them yet.
extern "C" {
unsigned char menuModeRebootQuestion = 0;
unsigned short* title;
int ranBrowser;
int ranConfig;
}
static_assert(sizeof(int) == 4, "words are four bytes");
static_assert(__SIZEOF_POINTER__ != 4 || sizeof(unsigned short*) == 4, "title is a 32-bit pointer");
