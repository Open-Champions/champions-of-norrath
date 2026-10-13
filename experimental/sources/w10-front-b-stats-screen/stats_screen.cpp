// Observed in the pinned SLUS_205.65 image: statsScreen at 0x004281a8 is 232 bytes of zeros in
// initialised data. inventoryExit indexes it with a 116-byte stride (player * 116, or 0 in network
// mode) and resets the record with resetMenu__FP12screenStruct.
// Inferred: two records of 116 bytes (screenStruct), from 232 = 2 * 116 and the stride.
// Unknown: the layout of a record beyond the offsets that resetMenu and inventoryExit touch.
extern "C" {
unsigned char statsScreen[232];
}
