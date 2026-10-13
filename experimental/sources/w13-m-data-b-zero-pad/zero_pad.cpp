// zeroPad: the zero-filled 88-byte pad record that input code selects while the game is paused.
//
// Observed: the private image has a zero-filled (.bss) label zeroPad at 0x007d03e8. The next label
// in that section is at 0x007d0440, giving an 88-byte extent. A second label of the same name
// sits at 0x007d04e0; this unit does not supply it, and the other occurrence at 0x007cc570 is a
// separate object used by a different unit.
// Inferred: the extent is 88 bytes because callers index 88-byte pad records, and the next label
// starts at +0x58. Zero contents follow from the section type.
// Unknown: whether the original object is aligned beyond 8 bytes and the purpose of the pad fields
// beyond their use as zero input state.
extern "C" {
alignas(8) unsigned char zeroPad[88];
}
