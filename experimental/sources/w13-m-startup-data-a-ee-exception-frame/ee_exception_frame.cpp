/* Observed exception frame, pinned SLUS_205.65: a 0x230-byte zero-initialised block at 0x008e5780
 * in .bss. The image has several names at its first byte (EER00, EEREG, hstack); this unit does not
 * use them. Inferred: the frame holds the saved state of an exception, laid out as the restore
 * routine reads it: 128-bit registers at 16 * n, the 64-bit HI, HI1, LO, LO1 and SA slots at 0x200
 * through 0x220, and the EPC word at 0x228. The restore routine is authored separately in
 * w12-startup-rt-a-k-exit-tlb-handler. Unknown: which code fills the frame, and whether the other
 * offsets have meaning beyond the restore routine's reads.
 */
extern "C" {
alignas(16) unsigned char norrath_ee_exception_frame[0x230];
}
