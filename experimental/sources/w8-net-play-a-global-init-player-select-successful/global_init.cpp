// Observed _GLOBAL_$I$playerSelectSuccessful at 0x00280a68, 32 bytes. The
// compiler's static-initialization entry: it calls the file-local initializer
// at 0x00280480 with (1, 0xffff). That initializer only returns, so this unit
// has no observable effect.
extern "C" void _GLOBAL_$I$playerSelectSuccessful() {
}
