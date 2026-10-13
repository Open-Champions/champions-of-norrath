// Observed: mtcStart (0x00485c78) and mtcExit (0x00485c7c) are two adjacent 4-byte words,
// zero in the file. mtcInitialize copies two words of the chosen block record into them
// (mtcExit first, then mtcStart).
// Inferred: none. Unknown: their readers and meaning; no other authored source reads them.
extern "C" {
int mtcStart = 0;
int mtcExit = 0;
}
static_assert(sizeof(mtcStart) == 4 && sizeof(mtcExit) == 4, "observed sizes");
