// Observed: ReleaseMaterial__8VIRasteriR12VIDictionary at 0x0110dc20 is 232 bytes. The integer
// argument is, in effect, the address of a material (its word at offset 4 is read, so it is a
// pointer; the symbol's signature calls it an int). The word at material+4 is decremented and
// stored; if the result is positive the function returns zero. Otherwise:
//   1. if the material's word at offset 36 is not -1, the word at offset 8 of the 24-byte entry with
//      that index, in the array whose pointer is at raster offset 19460, is set to -1;
//   2. for the first (word at material+8) layers, the layers being 144-byte records starting at
//      material+48, each whose first word is not -1 gets
//      ReleaseSurface__8VIRasteriR12VIDictionary(raster, thatWord, dictionary); the layer
//      count is re-read after each such call;
//   3. Erase__t6VIList1Z10VIMateriali(raster+19416, material) is called and the result is zero
//      if it is not negative and -1 otherwise.
// Inferred: material+4 is a reference count; layer records start with a surface index; the
// array at raster+19460 holds per-material GS state keyed by the word at +36.
// Unknown: the remainder of the material and layer layouts and of the 24-byte entries.
extern "C" {
struct VIRaster;
struct VIDictionary;

int ReleaseSurface__8VIRasteriR12VIDictionary(VIRaster* raster, int index, VIDictionary& dictionary);
int Erase__t6VIList1Z10VIMateriali(void* list, int material);

int ReleaseMaterial__8VIRasteriR12VIDictionary(VIRaster* raster, int material, VIDictionary& dictionary) {
    char* base = reinterpret_cast<char*>(raster);
    char* m = reinterpret_cast<char*>(material);
    int references = *reinterpret_cast<int*>(m + 4) - 1;
    *reinterpret_cast<int*>(m + 4) = references;
    if (references > 0) return 0;
    int slot = *reinterpret_cast<int*>(m + 36);
    if (slot != -1) *reinterpret_cast<int*>(*reinterpret_cast<char**>(base + 19460) + slot * 24 + 8) = -1;
    int count = *reinterpret_cast<int*>(m + 8);
    if (count > 0) {
        const int* layer = reinterpret_cast<const int*>(m + 48);
        for (int i = 0; i < count; ++i) {
            int surface = *layer;
            layer += 36;
            if (surface != -1) {
                ReleaseSurface__8VIRasteriR12VIDictionary(raster, surface, dictionary);
                count = *reinterpret_cast<int*>(m + 8);
            }
        }
    }
    return Erase__t6VIList1Z10VIMateriali(base + 19416, material) < 0 ? -1 : 0;
}
}
