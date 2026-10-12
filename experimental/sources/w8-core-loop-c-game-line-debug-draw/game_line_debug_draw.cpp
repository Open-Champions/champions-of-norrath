// gameLineDebugDraw at 0x00279b50, 8 bytes, in SLUS_205.65.
// Observed: the body is a return and its delay-slot no-op. It reads and writes nothing and
// takes no arguments. Inferred: a debug draw hook whose body was compiled out of this build.
extern "C" void gameLineDebugDraw__Fv() {}
