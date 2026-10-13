// Observed: GetMaterialPalEntry__8VIRasterii at 0x01113ba0, 44 bytes. With the
// raster as this and ints a, b: the word at this + 19436 is the base of a table of
// 12-byte entries; the first word of entry [a] points to a record; the word at
// record + 12 points to an array of words; the result is word [b] of that array.
// Nothing is written.
// Inferred: the record is a material with a palette array at +12 (the material
// code elsewhere uses 144-byte layers from +48, which this does not touch).
// Unknown: bounds of a and b, the entry layout beyond its first word, and the
// member names.
extern "C" {
int GetMaterialPalEntry__8VIRasterii(const void* raster, int a, int b) {
    const unsigned char* table =
        *reinterpret_cast<const unsigned char* const*>(static_cast<const unsigned char*>(raster) + 19436);
    const unsigned char* record = *reinterpret_cast<const unsigned char* const*>(table + a * 12);
    const int* array = *reinterpret_cast<const int* const*>(record + 12);
    return array[b];
}
}
