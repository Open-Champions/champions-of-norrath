/* Observed data object, pinned SLUS_205.65: m_buf at 0x011e8540 in the overlay's zero-initialised storage. Its symbol is
 * local to the image and has no recorded size; the next label (sCombinationList) is 320 bytes later, so the extent is taken
 * as 320 bytes. It is the shared request/reply block of the eznetctl client: ezNetCtlDownInterface, ezNetCtlUpInterface
 * and ezNetCtlSetCombination write their request into it and pass it as both the send and receive area.
 * Published as one global so the units of that library file can share it. ezNetCtlDownInterface
 * (vi-launcher-eznet-down) still declares its own file-local copy and so addresses a different object at link time until it
 * is switched to this definition.
 * Unknown: the layout beyond the words and strings the three callers use.
 */
extern "C" {
alignas(8) unsigned char m_buf[320];
}
