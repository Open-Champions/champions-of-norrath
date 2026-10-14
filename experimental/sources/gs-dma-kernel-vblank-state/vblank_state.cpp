/* Observed vblank words. vblankCount at 0x00484438 is 4 zero sdata bytes.
 * vblankField at 0x0048443c is the next sdata byte and is zero. vblankHID at
 * 0x004843ec is 4 zero sdata bytes. vblankSemaphore at 0x00486a5c and
 * vblankSemaphore2 at 0x00486a60 are sbss, size 0 in the symbol table, with
 * the next symbol 4 bytes later. Callers load each as a word. The file image
 * of all five is zero. This profile does not place them at those addresses.
 *
 * cd-archive reads the same count, field, and second semaphore through
 * norrath_archive_vblank_count_view, norrath_archive_vblank_field_view, and
 * norrath_archive_vblank_semaphore_view. Those names are not defined here.
 */
extern "C" int vblankCount = 0;
extern "C" unsigned char vblankField = 0;
extern "C" int vblankHID = 0;
extern "C" {
int vblankSemaphore;
int vblankSemaphore2;
}
