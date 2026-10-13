/* Observed: sifParamFname at 0x008e6bf0. The symbol size is 0; sifParamNext
 * starts at 0x008e7040, 1104 bytes later. The memory-card calls that pass a
 * name (open, chdir, mkdir, delete, get-dir, format) fill the first 20 bytes
 * with word arguments, copy the name to offset 20, and pass the address as
 * a 1044-byte send area to sceSifCallRpc; byte 1043 is cleared. The image
 * bytes are zero.
 * Inferred: the extent is the 1104-byte span up to the next symbol, of which
 * 1044 bytes are used. Alignment 16 follows the address (0x...bf0).
 * Unknown: what the remaining 60 bytes are, if anything.
 */
extern "C" {
alignas(16) unsigned char sifParamFname[1104];
}
