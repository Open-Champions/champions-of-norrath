// Observed: Init__13VIActorFilter at 0x0105ec08 is 16 bytes in the pinned SLUS-20565 image. It stores -1 in the first word of the
// object and returns zero in the result register. It reads nothing.
// Inferred: the word is a "match anything" filter value; the 0 result is an int status (the constructor of this class tail-calls
// this routine and returns its result).
// Unknown: the meaning of the word, the object size, and what callers do with the status. Nothing here ran on the EE.
extern "C" {
int Init__13VIActorFilter(int* self) {
    self[0] = -1;
    return 0;
}
}
