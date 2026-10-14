// Observed: Reset__14VICameraEffect at 0x01079ce8 is 24 bytes in the pinned SLUS-20565 image. It stores zero in the five words at
// offsets 0, 4, 8, 12 and 16 (the original stores +16 first) and returns nothing.
// Inferred: the object's first five words are the effect's active state; zero is the idle state.
// Unknown: the meaning of each word and the size of the object. Nothing here ran on the EE.
extern "C" {
void Reset__14VICameraEffect(int* self) {
    self[4] = 0;
    self[0] = 0;
    self[1] = 0;
    self[2] = 0;
    self[3] = 0;
}
}
