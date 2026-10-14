// Observed data symbol __ti10GameObject at 0x00486dc8, 8 bytes, zero-initialized storage (.bss, reached through gp).
// It is the root type-info node of the game-object classes: __tf10GameObject (0x00312dd0) returns its address after
// registering "10GameObject" when the first word is zero, and every derived class's type-info function passes it
// to __rtti_si as the base. Only the first word is tested by those functions; what the second word holds is
// not established here (the registration call may store a name pointer in it). This unit supplies the storage
// only; no code.
extern "C" {
unsigned char __ti10GameObject[8] __attribute__((aligned(4)));
}
