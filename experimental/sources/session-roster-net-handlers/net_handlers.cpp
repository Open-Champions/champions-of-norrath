// Observed handler tables and send mask, written as 32-bit words because the
// target stores 4-byte code addresses here. Sizes are the gaps to the next
// symbol. All are zero in the file.
//   commSendmask  0x00486614  4 bytes (global, ELF size 4)
//   msgHandler    0x004f3ed0  64 bytes = 16 slots
//   pktHandler    0x004f3f10  16 bytes = 4 slots
// The registrars store one word past the end of each table when it is full;
// that word is the first word of the next object, so these are separate
// objects here and the full-table store is not modeled by this file.
extern "C" {
unsigned commSendmask;
unsigned msgHandler[16];
unsigned pktHandler[4];
}
