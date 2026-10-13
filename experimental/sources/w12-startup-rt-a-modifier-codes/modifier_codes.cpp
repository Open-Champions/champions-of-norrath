// Observed ModifierCodes at 0x00447f60, 8 bytes of initialised writable data in the keyboard library
// (the next object, the audio-device words, starts at 0x00447f68). The image holds the bytes
// 0xe0 0xe1 0xe2 0xe3 0xe4 0xe5 0xe6 0xe7. IsEventModifier compares an event's halfword against each of
// them in order.
// Inferred: these are the eight key codes of the modifier keys of a keyboard (USB usage ids 0xe0..0xe7).
// Unknown: whether the table is ever written; it sits in .data, not in a read-only section.
extern "C" {
unsigned char ModifierCodes[8] = {0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7};
}
