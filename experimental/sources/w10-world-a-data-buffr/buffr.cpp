// Observed buffr at 0x00442168 (.data), 1280 bytes. nm size 1280 in .data; the image bytes are all zero. The authored voice-chat pump reads and writes it.
// Inferred: A circular byte buffer with buffrout, buffrfull and buffrmode as its state (from the authored commProcessReal).
// Unknown: The element type and the capacity semantics.
extern "C" {
unsigned char buffr[1280];
}
