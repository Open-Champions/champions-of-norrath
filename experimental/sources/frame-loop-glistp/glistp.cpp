// Observed glistp at 0x004844c8, 4 bytes in .sdata. The file word is 0.
// engineFrameStart stores the display-list pointer here. The pause-menu
// check reads glistp - gstart.
extern "C" {
unsigned glistp = 0u;
}
static_assert(sizeof(glistp) == 4, "glistp is one word");
