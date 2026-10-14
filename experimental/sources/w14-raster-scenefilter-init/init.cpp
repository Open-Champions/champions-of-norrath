// Observed: Init__13VISceneFilter at 0x01138310 is 28 bytes in the pinned SLUS-20565 image. It stores -1 in the four words at
// offsets 0, 4, 8 and 12 (the original stores +12 first, then +0, +4, +8) and returns zero. It reads nothing.
// Inferred: the words are "match anything" filter values; the zero is an int status.
// Unknown: the meaning of each word and the size of the object (offset 16 holds the class table, set by the constructor). Nothing
// here ran on the EE.
extern "C" {
int Init__13VISceneFilter(int* self) {
    self[3] = -1;
    self[0] = -1;
    self[1] = -1;
    self[2] = -1;
    return 0;
}
}
