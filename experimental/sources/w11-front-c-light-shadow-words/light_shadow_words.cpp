// Observed saveValid at 0x00483ab8 (4 bytes in .data, zero) and saveBestList at 0x004fbf80
// (bss, symbol size 0; mediumRunList is 16 bytes later). lightSetupForCharacter and
// lightCharacterShadow read and write them: the three best lights are stored and the valid
// word is set to 1. Three pointers are used (indexes 0 to 2); the slot is 16 bytes.
// Inferred: a cache of the last three chosen lights. Unknown: when saveValid is cleared.
extern "C" {
int saveValid;
void* saveBestList[3];
}
