// Observed storage in the pinned SLUS_205.65 image, all zero in the file.
//   newSel            0x00484c74  4 bytes, word (initialized data in the image)
//   nmActions         0x00486a1c  4 bytes, word (bss)
//   nmDialogNodes     0x00484364  4 bytes, word (initialized-data local)
//   playerSelectInfo  0x00438910  164 bytes (0xa4) = four 41-byte records, one
//                                 per player, as the select-screen code indexes
//                                 them. Bytes 32..36 of a record are used by
//                                 the select, enforce and change-players code;
//                                 bytes 0..31 are passed as a name.
// Unknown: the field layout beyond the used bytes and the value ranges.
// This build profile places the zero-initialized objects in the bss area.
extern "C" {
int newSel;
int nmActions;
int nmDialogNodes;
unsigned char playerSelectInfo[164];
}
