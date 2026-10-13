// Observed: CreateMaterial__8VIRasteri at 0x01113818 is 100 bytes. It calls
// Add__t6VIList1Z10VIMaterial(raster+19416). A negative result is returned as -1. Otherwise,
// with the result read as an address: if the integer argument is nonzero it increments the word at
// result+4; in both cases it stores the 64-bit value at offset 17872 of the raster (the value
// SetTime stores) at result+24. The result itself is returned.
// Inferred: the list at 19416 holds materials; the result is the new element (its address); the
// word at +4 is a use count that a nonzero argument asks to take a reference on; +24 is a
// last-used time.
// Unknown: the layout of the rest of a material element, and what a negative Add result means.
extern "C" {
struct VIRaster;

int Add__t6VIList1Z10VIMaterial(void* list);

int CreateMaterial__8VIRasteri(VIRaster* raster, int takeReference) {
    char* base = reinterpret_cast<char*>(raster);
    int element = Add__t6VIList1Z10VIMaterial(base + 19416);
    if (element < 0) return -1;
    char* material = reinterpret_cast<char*>(element);
    if (takeReference != 0) *reinterpret_cast<int*>(material + 4) += 1;
    *reinterpret_cast<long long*>(material + 24) = *reinterpret_cast<long long*>(base + 17872);
    return element;
}
}
