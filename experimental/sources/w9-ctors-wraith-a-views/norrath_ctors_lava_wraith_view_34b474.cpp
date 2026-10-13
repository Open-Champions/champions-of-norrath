// Observed: 0x34b474 is a word inside the sized .data symbol skills (0x34b440,
// 252 bytes). Its initial value is 0x00602de8, the address of the .bss symbol
// skillFireWeapons; its neighbours are the same kind of value 0x68 apart.
// Observed: the LavaMonster constructor loads this word and uses the result as
// an object (virtual-style method lookup at +96). Inferred: the word is one
// entry of a pointer table; the table's owner is another package's concern.
// Unknown: whether the game writes this word after start-up.
extern "C" {
extern unsigned char skillFireWeapons[] asm("skillFireWeapons");
void* norrath_ctors_lava_wraith_view_34b474 = skillFireWeapons;
}
