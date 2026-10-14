// Observed sbuff at 0x007dd840. The .bss neighbour sbuffin is 0x40 bytes later;
// callers pass it as a 64-byte receive buffer. The ELF gives no size, so 64
// comes from that spacing and from those callers. The image holds no data.
// This is the stream buffer; the .bss word at 0x008e5a00 with the same ELF
// name is a different object. Adjacency to sbuffin is not guaranteed by the
// separate object.
extern "C" {
alignas(64) unsigned char sbuff[64];
}
