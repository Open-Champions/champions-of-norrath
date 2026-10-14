// Observed in the pinned SLUS_205.65 image:
//   playerStartYaw     0x00484f38  8 bytes  zero-initialised; callers store halfwords at index 0..3,
//                                  one caller also stores single bytes at 2n and 2n+1
//   playerSpecStorage  0x00333668  164 bytes  zero-initialised; the launch code addresses it as
//                                  records of 41 bytes at 41 * slot (copy of 41 bytes, packed-player
//                                  builder) and 164 = 4 * 41
// Inferred: four per-player slots in each, from the extents.
// Unknown: the layout of a 41-byte spec record.
extern "C" {
unsigned short playerStartYaw[4];
unsigned char playerSpecStorage[164];
}
