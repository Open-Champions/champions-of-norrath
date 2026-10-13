/* Observed _sceSifMInitCheck at 0x00447c48, a 4-byte initialised-data word in the symbol
 * table, zero in the image. sceSifMExitRpc (w9-startup-crt-f) stores zero in it.
 * Unknown: what stores a nonzero value and what reads it (no authored routine does).
 * Placement differs from the original: a zero-initialised definition here lands in
 * uninitialised data.
 */
extern "C" {
int _sceSifMInitCheck = 0;
}
