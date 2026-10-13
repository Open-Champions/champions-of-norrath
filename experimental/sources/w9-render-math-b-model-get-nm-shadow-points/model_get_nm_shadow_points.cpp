// Observed modelGetNmShadowPoints__FP12_modelHeader at 0x0016cd50, extent 20.
// Reads the unsigned byte at header+36, multiplies it by 16, adds that to the
// header address, and returns the pointer-sized word at that address plus 4.
// Nothing is written.
// Inferred: the byte at +36 counts 16-byte records that follow the header start
// (a table of records), and the word at +4 of the record after them is the shadow
// point data pointer named by the function. Unknown: the record layout and the
// pointee type; the result is returned as an untyped pointer.
extern "C" const void* modelGetNmShadowPoints__FP12_modelHeader(const unsigned char* header) {
    const unsigned char* record = header + header[36] * 16;
    return *reinterpret_cast<const void* const*>(record + 4);
}
