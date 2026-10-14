// Observed arrowLeft at 0x004848fc, 4 bytes, section .sdata/.data (D).
// One initialised word of 4 bytes whose file bytes are zero. The next word is arrowRight. invInitSharedInterfaceTextures__Fv is the only code found referencing it, with a word store.
// Inferred: A 32-bit word holding an address (another unit declares it void*), zero at start. It is declared here as unsigned so that the host check sees the 4 bytes of the target.
// Unknown: What it holds and where it is read.
extern "C" {
unsigned arrowLeft = 0;
}
