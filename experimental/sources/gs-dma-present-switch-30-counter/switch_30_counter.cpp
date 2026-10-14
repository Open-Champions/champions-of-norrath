// Observed switchTo30Counter.798 at 0x00484470. Four .sdata bytes, file
// value 0. engineFrameEnd reads and writes this word.
extern "C" unsigned switchTo30Counter_798 asm("switchTo30Counter.798") = 0u;
static_assert(sizeof(switchTo30Counter_798) == 4, "one word");
