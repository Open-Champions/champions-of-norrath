// Observed storage of the shop screen and the inventory pickup flags, with the
// original symbol, address and size. Every object is zero in the image (the .data
// and .sdata ones have all-zero file bytes; the others are .bss), so all of them are
// plain zeroed objects here and the .data ones land in .bss in this build.
//   shop              0x007d01f8  464 bytes, .bss (the gap to sellMode)
//   sellMode          0x007d03c8   16 bytes, .bss (four words; the gap to the next symbol)
//   playerGettinIt    0x007d08f0   16 bytes, .bss (four words, one per player)
//   shopIntro         0x00486aa0    4 bytes, .bss
//   shopTick          0x00486aac    4 bytes, .bss (a pointer-sized word)
//   shopGrid          0x0048491c    4 bytes, .sdata, zero
//   shopDisableMouse  0x00484a10    8 bytes, .data, zero (two words)
//   shopMouseDecay    0x00484a18    8 bytes, .data, zero (two words)
// The element types are not established by this unit: the shop record is a byte
// block whose fields other units read at fixed offsets; the others follow how the
// callers use them (words, or two or four of them).
// Unknown: every field of `shop` beyond the offsets the callers touch.
extern "C" {
unsigned char shop[464];
int sellMode[4];
int playerGettinIt[4];
int shopIntro;
void* shopTick;
int shopGrid;
int shopDisableMouse[2];
int shopMouseDecay[2];
}
