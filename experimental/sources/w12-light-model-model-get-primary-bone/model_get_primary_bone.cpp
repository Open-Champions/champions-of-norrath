// Observed modelGetPrimaryBone__FP12_modelHeaderi at 0x0016c778, extent 48. Arguments: a model
// header and an index. Signed header bytes at +18 (a) and +38 (b) give a stride of
// b * (a * 4 + 4). The result is the signed byte at header + 46 + index * 10 + stride, widened
// to int. No bounds check is made on either value.
// Inferred: the bytes after the 46-byte prefix are 10-byte per-index records, one byte of which
// (offset 0 here) is read as a bone number. Unknown: the meaning of +18 and +38, and the range of
// valid indices.
extern "C" {
int modelGetPrimaryBone__FP12_modelHeaderi(const unsigned char* model, int index) {
    int a = static_cast<signed char>(model[18]);
    int b = static_cast<signed char>(model[38]);
    int stride = b * (a * 4 + 4);
    return static_cast<signed char>(model[stride + index * 10 + 46]);
}
}
