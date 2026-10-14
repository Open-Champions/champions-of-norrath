// Observed `player` at 0x00408600, 95936 bytes (0x176c0) in the pinned
// SLUS_205.65 image. 95936 is exactly four 23984-byte records, the record size
// used by playerInit. Every file byte is zero. No field layout is claimed
// here. Other authored code indexes the record by 23984 * index.
// Inferred: four player records. Unknown: how the image treats a fifth index.
extern "C" unsigned char player[95936];
unsigned char player[95936];
