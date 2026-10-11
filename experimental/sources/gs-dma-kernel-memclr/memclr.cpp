/* Observed memclr at 0x002d2c70, 52 bytes. A zero count returns. Any other
 * count stores that many zero bytes, counting down from count-1 through -1.
 */
extern "C" void memclr(unsigned char* dest, int count) {
    if (count == 0)
        return;
    int remaining = count - 1;
    do {
        *dest = 0;
        dest += 1;
        remaining -= 1;
    } while (remaining != -1);
}
