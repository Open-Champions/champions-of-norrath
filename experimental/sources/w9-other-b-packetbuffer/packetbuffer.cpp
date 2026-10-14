// Observed: packetbuffer starts at 0x00442668 and the next symbol, coreInterfaceBrightness, starts 8512
// bytes later; the symbol table size is also 8512. All file bytes are 0. clearPacketBuffer__Fv,
// commProcessReal__Fv and getBuffer__Fii take its address; advancePacketshift__Fv and
// commProcessReal__Fv also form an address 132 bytes into it.
// Inferred: a byte buffer for network packets, from the function names. The size is the symbol size.
// Unknown: the record layout inside the buffer and what sits at offset 132.

extern "C" unsigned char packetbuffer[8512] = {};
static_assert(sizeof(packetbuffer) == 8512, "packetbuffer");
