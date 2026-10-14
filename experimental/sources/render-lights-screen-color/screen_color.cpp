// Observed engineSetScreenColor__Fiiii at 0x1910c0, extent 28.
// It adds 128 to the red, green, and blue registers and stores those words.
// The fourth argument is not stored. compositeAlpha at 0x484428 is the
// previous word and is not a destination.
// compositeR, compositeG, and compositeB are the three .sdata words at
// 0x48442c, 0x484430, and 0x484434. Each file word is 0x00000080.
// vblankCount begins at 0x484438. None of these addresses is an object
// already authored on this branch.

extern "C" {
int compositeR = 128;
int compositeG = 128;
int compositeB = 128;
}

extern "C" void engineSetScreenColor__Fiiii(int red, int green, int blue, int) {
    compositeR = static_cast<int>(static_cast<unsigned>(red) + 128u);
    compositeG = static_cast<int>(static_cast<unsigned>(green) + 128u);
    compositeB = static_cast<int>(static_cast<unsigned>(blue) + 128u);
}
