// Observed memCheckHeapReal__FPCci at 0x144b60, extent 8: jr ra and a delay-slot
// nop. It ignores both arguments and returns nothing. The authored heap checker
// norrath_mem_check_heap_real is the same extent. Unknown: whether a larger
// checker was compiled out of this build of the game.
extern "C" void memCheckHeapReal__FPCci(const char* file, int line) {
    (void)file;
    (void)line;
}
