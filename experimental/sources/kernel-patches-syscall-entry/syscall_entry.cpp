// Observed: _SyscallEntry at 0x00444a18, a 4-byte word. The image bytes are
// zero. GetSystemCallEntry loads it as a base address, and
// InitSystemCallTableAddress stores the agreed search result in it.
// Inferred: none.
// Unknown: none. The original is in .data; this profile places a zero in .bss.
extern "C" {
unsigned _SyscallEntry = 0;
}
