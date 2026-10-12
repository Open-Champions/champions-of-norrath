// Observed switchToHalfCounter.797 at 0x0048446c. Four .sdata bytes, file
// value 0. engineFrameEnd reads and writes this word.
extern "C" unsigned switchToHalfCounter_797 asm("switchToHalfCounter.797") = 0u;
static_assert(sizeof(switchToHalfCounter_797) == 4, "one word");
