// Observed _GLOBAL_$I$wrathHammerCount at 0x0015fbe8, 32 bytes. The compiler's
// static-initialization entry: it calls the file-local initializer with
// (1, 0xffff). That initializer at 0x0015fbe0 only returns, so this unit has no
// observable effect. The original local initializer's result is unused.
extern "C" void _GLOBAL_$I$wrathHammerCount() {
}
