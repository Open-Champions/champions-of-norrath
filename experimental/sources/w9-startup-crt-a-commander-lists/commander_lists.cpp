// Observed class static storage, all in the initialized data of the executable and all zero there:
//   _14WoodElfSoldier$m_commanderList  0x0042dd90   40 bytes (10 words)
//   _3Orc$m_commanderList              0x0042ce10   80 bytes (20 words)
//   _6Wraith$s_rWraithList             0x0042bd18  240 bytes (60 words)
// The sizes are the symbol sizes. The static initializers that name these (WoodElf soldier, Orc and Wraith
// resource-map units) clear every word in order and, at exit, destroy the same words as list heads from the last to
// the first, so each word is read here as one list head pointer. That is inferred from those routines; the element
// type is not otherwise observed. The zero values are written as zero-initialized definitions, which a compiler
// normally places in uninitialized data; the executable has them in initialized data. Fresh compilation is not an
// original-byte match.
extern "C" {
unsigned WoodElfSoldier_commanderList[10] __asm__("_14WoodElfSoldier$m_commanderList");
unsigned Orc_commanderList[20] __asm__("_3Orc$m_commanderList");
unsigned Wraith_rWraithList[60] __asm__("_6Wraith$s_rWraithList");
}
