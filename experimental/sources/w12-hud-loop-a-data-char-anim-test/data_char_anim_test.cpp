// Observed charAnimTest at 0x00483f34, 4 bytes, section .sdata/.data (D).
// One initialised word of 4 bytes, zero at start. drawAttachments__6PlayerP11_drawRecordf loads it and inventoryDrawPlayer__FiiP14AnimationStatei loads and stores it, as whole words.
// Inferred: An int flag or counter, zero at start.
// Unknown: Its meaning; the name is the one the image carries.
extern "C" {
int charAnimTest = 0;
}
