// Observed storage of sceCbPoff_Gp at 0x008e1c80: a zero-size LOCAL label in .bss,
// with the next label, cd, 64 bytes later (0x008e1cc0). The span is 64 bytes and is
// aligned to 64 because the original address is.
// Observed use by a different unit: the power-off path loads the first word of this
// span into the global-pointer register before calling the registered callback.
// Inferred: the word holds the global pointer for that callback.
// Unknown: who stores it, and what the other 60 bytes of the span hold; they are
// zero in the image and no authored code reads them. The image bytes are zero.
// Nothing here ran on the EE.
extern "C" {
alignas(64) unsigned int sceCbPoff_Gp[16];
}
