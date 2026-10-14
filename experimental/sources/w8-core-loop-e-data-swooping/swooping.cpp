// Observed swooping at 0x00486b70. Four bytes in .sbss, so zero at load.
// The text accesses it with three word loads and five word stores. What it
// controls is not established here.
extern "C" {
int swooping;
}
