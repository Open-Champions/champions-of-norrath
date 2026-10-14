/* Observed DPRINT at 0x002d4818, 40 bytes. It takes a format pointer and
 * further arguments, spills the register arguments the variadic prologue
 * keeps (a1 through t3, at stack offsets 72 through 120 of a 128-byte frame),
 * and returns. It reads no argument and calls nothing, so it prints nothing
 * in this build. Compiled output is not claimed to match the original.
 */
extern "C" void DPRINT(const char* format, ...) {
    (void)format;
}
