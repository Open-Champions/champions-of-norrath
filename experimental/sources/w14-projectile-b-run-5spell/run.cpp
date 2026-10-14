// Observed run__5SpellG6Point3s at 0x003149f8, 36 bytes (a weak definition). The arguments are the spell object, the address of a
// Point3 and a halfword. The original copies the 12 bytes at the Point3 address into its own stack frame, never reads them again and
// returns: no memory outside its frame is written and nothing is called. The copy has no effect, so the body is empty.
// Inferred: the base-class default of a spell's run step (derived classes supply their own). Unknown: the meaning of the point and
// of the halfword. Nothing here ran on the EE.
extern "C" void run__5SpellG6Point3s(void*, void*, short) {}
