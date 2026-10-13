// Observed memory-card storage in the pinned SLUS_205.65 image, zero bss.
//   quickLoadStamp  0x007f7fc0  64 bytes (distance to sortedTimes); copied
//                               whole by prepareBrowserUndo__Fv and cleared
//                               with a 64-byte memset.
//   ramSlot         0x0081e240  731592 bytes (distance to slotInfoHeaders).
//                               That is exactly the length the load routine
//                               copies inline into it.
// Both addresses are multiples of 64; alignas(64) records that observation and
// is not evidence of a required alignment.
// Unknown: the internal layout of either buffer beyond what the authored
// memory-card routines already read.
extern "C" {
alignas(64) unsigned char quickLoadStamp[64];
alignas(64) unsigned char ramSlot[731592];
}
