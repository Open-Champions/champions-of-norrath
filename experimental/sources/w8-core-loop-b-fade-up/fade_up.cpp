// Observed coreInterfaceFadeUp__Fi at 0x002d0cb0, 20 bytes.
// The argument is used as a word index into coreInterfaceFadeUpVal (0x004447b8,
// 16 bytes, four words, each 1.0f in the file). The selected word is set to zero.
// There is no bounds check, return value or other effect.
// Inferred: the words are floats, because the file holds 0x3f800000 in each.
// Unknown: the meaning of the four slots and who reads them.
extern "C" float coreInterfaceFadeUpVal[4];

extern "C" void coreInterfaceFadeUp__Fi(int index) {
    coreInterfaceFadeUpVal[index] = 0.0f;
}
