/* Observed sceResetttyinit at 0x002d5560, 12 bytes. Stores zero in the
 * local word ttyinit (0x00444950, file value to be read from the image). The
 * word is private to this unit here; its other users are not part of this
 * function.
 */
int ttyinit;

extern "C" void sceResetttyinit() {
    ttyinit = 0;
}
