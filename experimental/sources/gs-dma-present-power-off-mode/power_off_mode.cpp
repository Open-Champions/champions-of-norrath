// Observed powerOffMode at 0x004865f8. One .sdata byte, file value 0.
// engineFrameEnd reads it as a signed byte.
extern "C" signed char powerOffMode = 0;
static_assert(sizeof(powerOffMode) == 1, "one byte");
