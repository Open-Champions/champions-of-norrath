// Observed: ConversionTableSize.180 at 0x0118e698 is one initialised word with the value 32. It sits directly after
// the 64-byte _8VIString$ConversionTable1252 (0x0118e658), and 64 bytes is 32 sixteen-bit entries.
// Inferred: a function-local static of a VIString routine (the ".180" suffix is the compiler's local-static
// numbering), holding the entry count of that conversion table. Which function reads it is not established here.
// Unknown: the reader, and whether anything writes it (it is defined as the initial value, not constant).
int norrath_conversion_table_size __asm__("ConversionTableSize.180") = 32;
