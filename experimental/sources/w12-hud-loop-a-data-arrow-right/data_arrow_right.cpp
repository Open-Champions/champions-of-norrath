// Observed arrowRight at 0x00484900, 4 bytes, section .sdata/.data (D).
// One initialised word of 4 bytes whose file bytes are zero, directly after arrowLeft; invInitSharedInterfaceTextures__Fv is the only code found referencing it, with a word store.
// Inferred: The same kind of 32-bit address word as arrowLeft, zero at start.
// Unknown: What it points at and where it is read.
extern "C" {
unsigned arrowRight = 0;
}
