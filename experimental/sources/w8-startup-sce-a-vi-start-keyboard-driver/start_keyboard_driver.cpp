/* Observed VIStartKeyboardDriver__Fv at 0x002cbe98, 52 bytes. It calls
 * lgKbMInit (0x002e6ed0) with the four integers 1, 0, 15 and 0, and returns 0
 * when that call returns 0 and -1 for any other result. What the four
 * arguments mean is unknown. textSystemInit__Fv ignores this result.
 * Compiled output is not claimed to match the original.
 */
extern "C" int lgKbMInit(int first, int second, int third, int fourth);

extern "C" int VIStartKeyboardDriver__Fv() {
    return lgKbMInit(1, 0, 15, 0) == 0 ? 0 : -1;
}
