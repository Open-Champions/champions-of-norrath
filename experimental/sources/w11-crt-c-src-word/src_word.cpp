/* Observed: the symbol table names a file-local .bss word src at 0x00486d00,
 * directly followed by dst at 0x00486d04. initPosition__Fii (see
 * w10-world-b-init-position-hermite) stores its first index argument in src and the second in dst.
 * The image symbol has no recorded size; four bytes is the distance to dst.
 * Unknown: whether anything reads it back. dst is not defined here.
 */
extern "C" {
int src;
}
