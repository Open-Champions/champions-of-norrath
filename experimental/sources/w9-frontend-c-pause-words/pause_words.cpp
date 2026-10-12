// Observed pause storage in the pinned SLUS_205.65 image. Both objects are zero
// in the file.
//   pauseStack  0x00486cc0  bss. The next symbol, pauseStackPos, starts 5 bytes
//               later, so the stack has at most 5 entries. pushPause__Fv stores
//               one byte per push at a signed byte index; popPause__Fi reads
//               and optionally clears them.
//   paused      0x00485964  initialized-data local symbol, 1 byte of value 0.
//               The pause processing routine tests and writes it as a signed
//               byte; the sound-clear routine writes it as an unsigned byte.
// Inferred: the 5-entry size, from the distance to the next symbol; nothing in
// the image bounds the index. Unknown: the section placement of the original
// split (initialized `paused` versus bss `pauseStack`); this build profile
// places both in the zero-initialized area.
extern "C" {
unsigned char pauseStack[5];
signed char paused;
}
