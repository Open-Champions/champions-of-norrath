// Zero-initialised line table storage. gameLineCount is the 4-byte file zero at 0x00485ea8
// (the next symbol, needEnforce, begins at 0x00485ec0). gameLines begins at 0x008d57a8 and
// the next symbol, p1, begins at 0x008d5938: 400 bytes, 100 pointer slots. The add routine
// has no capacity test, and gameLineUpdatePosition indexes the table by a stored line index.
// Under the reviewed -G0 profile the file-backed zero lands in BSS; the value is zero either way.
// Unknown: the declared pointee type of the slots (each is the pointer to a line object whose
// first word is a name pointer).
extern "C" {
int gameLineCount;
void* gameLines[100];
}
