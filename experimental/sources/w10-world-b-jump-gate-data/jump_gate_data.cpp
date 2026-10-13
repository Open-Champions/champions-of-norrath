// Zero-initialised jump-gate storage, with addresses and extents from the private image's
// symbol table (SLUS_205.65). All are file-less (BSS) in the original.
//   lastDest           0x00486d8d  1 byte,  signed byte read and written by the task code
//   jumpGateState      0x00486d8e  1 byte,  the task state, read with an unsigned byte load
//   jumpGateWho        0x00486d8f  1 byte,  signed player index
//   jumpGateTime       0x00486d98  4 bytes, float; initJumpEffect stores 0.0f and the run
//                                  task adds a constant to it
//   jumpGateDestCount  0x00486da0  1 byte,  signed count; jumpGateSave/Load store it as the low
//                                  byte of a word and copy jumpGateEntries after it
//   jumpGateHurtDest   0x00486da4  4 bytes, a text pointer; non-null makes the task run use it
//                                  as the destination name (the loads are 32-bit words)
//   jumpGateEntries    0x008d60e0  480 bytes, fifteen 32-byte NUL-terminated names (the
//                                  480-byte copy in jumpGateSave/Load; strcmp/strcpy on 32-byte steps)
// The symbol table gives jumpGateHurtDest and the other small fields no size, so the widths
// above are the access widths seen in the code. Unknown: whether the entries array is exactly
// 480 bytes in the original (the following symbol begins later), and the meaning of the
// state values beyond the ones the task code uses.
extern "C" {
signed char lastDest;
unsigned char jumpGateState;
signed char jumpGateWho;
float jumpGateTime;
signed char jumpGateDestCount;
char* jumpGateHurtDest;
char jumpGateEntries[480];
}
