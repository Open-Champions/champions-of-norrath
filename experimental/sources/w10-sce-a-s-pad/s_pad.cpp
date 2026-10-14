/* Observed: sPad at 0x011b1e30 in the 0x01000000 module image, a 4-byte local
 * object between xmlInitMemoryDone (0x011b1e28) and sKeyboard (0x011b1e34).
 * The window-setup routine stores the pad object pointer there when it
 * initialises the legal-notice window. The image holds zero.
 * Inferred: a pointer to the pad object. Unknown: its readers, and whether
 * anything but the window-setup routine writes it.
 */
extern "C" {
void* sPad = 0;
}
