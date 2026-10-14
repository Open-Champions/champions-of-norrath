// Observed copyScreenColor at 0x00486a48: 4 bytes of zero-filled storage (small BSS). makeSamplePacket__Fb stores a pointer into the copyScreen packet
// here and doSuperSample__Fiib loads it as a word.
// Inferred: nothing beyond those accesses. No field meaning is claimed.
// Unknown: what the value represents and its full range.
extern "C" {
unsigned char* copyScreenColor;
}
