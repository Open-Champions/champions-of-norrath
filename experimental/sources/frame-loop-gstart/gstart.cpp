// Observed gstart at 0x004844c4, 4 bytes in .sdata. The file word is 0.
// engineFrameStart stores the same display-list pointer here as in glistp.
extern "C" {
unsigned gstart = 0u;
}
static_assert(sizeof(gstart) == 4, "gstart is one word");
