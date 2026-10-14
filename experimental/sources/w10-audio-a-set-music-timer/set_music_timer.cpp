// Observed: gameSetMusicTimer__Fi at 0x001d1100 is 8 bytes. It stores its integer argument
// in the 32-bit word musicTimer (0x00484f0c) and returns nothing.
// Inferred: nothing.
// Unknown: the unit of the timer and who counts it down.

extern "C" {
extern int musicTimer;
}

extern "C" void gameSetMusicTimer__Fi(int value) {
    musicTimer = value;
}
