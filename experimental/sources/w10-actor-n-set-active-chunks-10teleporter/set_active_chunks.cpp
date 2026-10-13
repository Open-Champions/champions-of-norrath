// Observed setActiveChunks__10Teleporter at 0x00312880, 8 bytes (weak): a bare return. It reads
// no argument, writes no memory, makes no call and returns no value. It sits in the Teleporter
// table's virtual functions; which table slot refers to it is not established here.
// Nothing here was executed on the EE.
extern "C" void setActiveChunks__10Teleporter() {}
