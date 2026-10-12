/* Observed serialPutchar at 0x002d6868, 52 bytes. Passes the character to the
 * local kputchar (0x002d6778). A character equal to 10 is preceded by a call
 * with 13. The result of the last kputchar call is returned.
 */
extern "C" int kputchar(int c);

extern "C" int serialPutchar(int c) {
    if (c == 10) kputchar(13);
    return kputchar(c);
}
