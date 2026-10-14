// Observed GLISTSIZE at 0x00484468, 4 bytes in .sdata.
// The file bytes are b8 88 00 00, the word 35000.
// The pause-menu check subtracts (glistp - gstart) >> 3 from this word.
// It is not the 0x445c0 immediate in engineFrameStart.
extern "C" {
unsigned GLISTSIZE = 35000u;
}
static_assert(sizeof(GLISTSIZE) == 4, "GLISTSIZE is one word");
