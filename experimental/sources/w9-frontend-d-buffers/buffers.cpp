// Observed in the pinned SLUS-20565 image. Sizes are the span to the next
// symbol; every object starts as zero and lives in bss. Alignment is the
// alignment of the original address, not a measured requirement.
//
//   0x006e1c60     96  rumphole          the next symbol is 0x006e1cc0; the pad code
//                                         reads it as 24 words, 6 words per record
//   0x004f4098     56  savedScene        starts 56 bytes after scene (0x004f4060)
//   0x008d0c08   1120  slotInfoHeaders   ten records of 112 bytes; a halfword at
//                                         +106 of a record is read by the player
//                                         change code
//   0x007e05e0  95936  tempPlayerStruct  four 23984-byte player records; record
//                                         size taken from the net-save routine
//
// Inferred: the record counts above, from the stride used by callers.
// Unknown: the field layouts and the writers.
extern "C" {
alignas(16) unsigned rumphole[24];
alignas(8) unsigned char savedScene[56];
alignas(8) unsigned char slotInfoHeaders[1120];
alignas(16) unsigned char tempPlayerStruct[95936];
}
