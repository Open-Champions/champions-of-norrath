// Observed sentEndMsg at 0x00486b60. Four bytes in .sbss, so zero at load.
// The text accesses it with one word load and two word stores. Its meaning
// beyond the name is unknown.
extern "C" {
int sentEndMsg;
}
