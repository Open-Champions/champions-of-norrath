/* Observed data words, pinned SLUS_205.65. Each object's initial bytes are read from the image.
 *
 * gConfig (0x00444850, 8 bytes, .data): initial bytes 1c 02 00 00 00 00 00 00. The time-zone
 * reader takes the signed halfword at offset 0 (0x021c = 540 minutes), and the summer-time
 * reader takes the byte at offset 6 (0). Inferred: the object is a copy of the OSD configuration
 * that the T10K path reads directly. Unknown: which code writes it and what the other bytes mean.
 *
 * iRandState (0x00486ab4, 4 bytes, .sbss): zero at load. The shop grid sets it to the player
 * number times 0x3184b3 before its cell loop. Unknown: its role outside that loop.
 *
 * localRandState (0x00484918, 4 bytes, .sdata, file-local in the image): initial bytes cd 18 0e 00,
 * the word 0x000e18cd. The shop grid sets it to the player number times 0xe171f. Unknown: the other
 * readers, if any.
 */
extern "C" {
unsigned char gConfig[8] = {0x1c, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
int iRandState = 0;
int localRandState = 0x000e18cd;
}
