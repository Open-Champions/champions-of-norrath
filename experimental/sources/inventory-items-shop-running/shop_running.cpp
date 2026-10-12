// Observed shopRunning at 0x483f2c, 4 bytes in .sdata, initial value 0. The
// neighbouring symbols are 4 bytes apart on both sides. shopStart__Fiii stores
// 1 here when it opens the shop screen (gp-relative offset -15044).
// Unknown: every reader and every other writer.
extern "C" int shopRunning = 0;
