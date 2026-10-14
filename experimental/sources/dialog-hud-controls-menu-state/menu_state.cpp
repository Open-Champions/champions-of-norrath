// Observed menu state used by the pause check.
// menu at 0x004842d4 and menuNm at 0x004842d0 are four zero bytes. A second
// local symbol of each name exists at 0x00484ba4 and 0x00484ba0; the pause
// and debug routines use the first pair. startMenuState at 0x004841c8 and
// useRaceMenu at 0x004841c9 are zero bytes. A second startMenuState byte is
// at 0x00484bdc and is not this object. menuPlayerNm at 0x004869e0 is a word
// in .sbss. kickMenuList begins eight bytes later; loads and stores at this
// address are words and the low byte. The four bytes in between are not
// part of this object.
extern "C" unsigned menu = 0u;
extern "C" unsigned menuNm = 0u;
extern "C" unsigned char startMenuState = 0;
extern "C" unsigned char useRaceMenu = 0;
extern "C" unsigned menuPlayerNm = 0u;
