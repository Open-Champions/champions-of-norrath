// Observed: sIconDefinition at 0x011e8c20 and zero in the image. The next symbol (sAsyncMode) is at
// 0x011e8fec, so the object spans at most 972 bytes; SetIcon__16VIIconDefinitionPvi writes it as far as +968
// to +971, and the VIMemCard functions and the static initializer address it.
// Inferred: one VIIconDefinition object (the class whose methods SetIcon, SetTitle and Ascii2Sjis are in
// the image) built by the static initializer; 972 bytes is the extent to the next symbol, which may include
// padding.
// Unknown: the exact class size and field layout; symbols in this region have no size in the symbol table.
extern "C" {
unsigned char sIconDefinition[972];
}
