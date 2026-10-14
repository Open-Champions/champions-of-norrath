// Observed: packetshift is one 32-bit word at 0x00486624 in initialized data. The file word is 0. It is
// loaded by advancePacketshift__Fv, commProcessReal__Fv and getBuffer__Fii, and stored by
// advancePacketshift__Fv and commProcessReal__Fv.
// Inferred: a position or step counter for packetbuffer, from the three callers that also use that
// buffer; the existing authored receive routine declares it as an int.
// Unknown: its range and whether it counts bytes, packets or buffer slots.

extern "C" int packetshift = 0;
static_assert(sizeof(packetshift) == 4, "packetshift");
