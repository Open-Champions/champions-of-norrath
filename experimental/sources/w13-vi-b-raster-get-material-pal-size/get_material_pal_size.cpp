// Observed: GetMaterialPalSize__8VIRasteri at 0x01113b80, 32 bytes. The word at
// raster + 19436 points to a table of 12-byte entries. The entry selected by the int
// argument has a pointer in its first word; the function returns the 32-bit word at
// offset 8 of what that pointer addresses. No range check is made.
// Inferred: each entry refers to a material or palette record whose word +8 is a size.
// Unknown: the record types, the table's length, and the unit of the returned size.
extern "C" {
int GetMaterialPalSize__8VIRasteri(void* raster, int index) {
    unsigned char* table = *reinterpret_cast<unsigned char**>(static_cast<unsigned char*>(raster) + 19436);
    unsigned char* record = *reinterpret_cast<unsigned char**>(table + index * 12);
    return *reinterpret_cast<int*>(record + 8);
}
}
