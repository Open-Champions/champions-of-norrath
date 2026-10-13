// Observed modelGetNmShadowPoints__FP12_modelHeader at 0x0016cd50, extent 20.
// Reads the unsigned byte at header+36, multiplies it by 16, adds that to the
// header address, and returns the 32-bit word at that address plus 4. Nothing is
// written.
// Observed use: modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f
// reads the same word and compares it, signed, with its index argument before it
// addresses a record, so the word is an entry count. "Nm" in the engine's names
// appears to mean a number (nmHalos is a count). The result is returned as int.
// Inferred: the number of shadow points of the model. Unknown: the record layout
// before the count and the meaning of the byte at +36 beyond the 16-byte unit.
extern "C" int modelGetNmShadowPoints__FP12_modelHeader(const unsigned char* header) {
    const unsigned char* table = header + header[36] * 16;
    return *reinterpret_cast<const int*>(table + 4);
}
