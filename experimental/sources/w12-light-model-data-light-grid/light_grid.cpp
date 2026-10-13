// Observed lightGrid at 0x004f40f0, .bss, no file bytes. The next symbol, saveBestList, is at
// 0x004fbf80, so the extent is 0x7e90 = 32400 bytes = 90 x 90 four-byte words. The authored
// light insertion and the two light-search routines index it as word (row * 90 + column) and treat
// each word as the head of a singly linked list of lights (the light's first word is the next link).
// Inferred: a 90 x 90 spatial hash of lights, 256 world units per cell (the search routines add
// 11520 and shift right by 8). Unknown: who clears it besides the light reset routine.
extern "C" {
unsigned char lightGrid[90 * 90 * 4];
}
