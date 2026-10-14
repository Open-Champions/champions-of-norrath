// Observed storage in the pinned SLUS_205.65 image, both zero in the file.
//   playersAlive   0x0042a3e8  16 bytes (ELF size 16)
//   syncRandState  0x00483e5c  4 bytes  (ELF size 4; read and written as a word)
// These two are also defined in session-roster-player-state, a unit that is not
// part of the partial build because its other words collide with definitions
// from other packages. This unit defines only these two names. No field layout
// or meaning is claimed for playersAlive.
extern "C" {
unsigned char playersAlive[16];
unsigned syncRandState;
}
