// Storage for the equipment and weapon cache group tables, with addresses from the private image's
// symbol table (SLUS_205.65).
//   nmEqGroups  0x004845a8  4 bytes, int count, initial 0 (global .data in the image)
//   nmWGroups   0x004845ac  4 bytes, int count, initial 0
//   eqGroup     0x00486a70  4 bytes, a 32-bit address of the equipment group records (BSS)
//   wGroup      0x00486a74  4 bytes, a 32-bit address of the weapon group records (BSS)
// Observed use (eqFind): the count and table pick of the 12-byte group records are selected by a
// flag; wGroup/nmWGroups when it is nonzero, eqGroup/nmEqGroups otherwise. The tables are words
// holding 32-bit addresses on the target, so they are declared unsigned. The symbol table gives
// eqGroup and wGroup no size; four bytes is the gap to the next symbol and the access width in
// eqFind. Unknown: who fills the tables and what the groups are called in the original. The zero
// initial counts land in BSS under the reviewed -G0 profile. Nothing here ran on the EE.
extern "C" {
int nmEqGroups = 0;
int nmWGroups = 0;
unsigned eqGroup;
unsigned wGroup;
}
