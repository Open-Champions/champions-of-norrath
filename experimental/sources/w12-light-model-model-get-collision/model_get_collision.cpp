// Observed modelGetCollision__FP12_modelHeaderi at 0x0016c740, extent 52. Arguments: a model
// header and an index. The header byte at +37 is a count; when it is zero the result is null.
// Otherwise the result is header + count * 16 + index * 24, with no bounds check on the index.
// Inferred: the collision records are 24-byte entries placed after a block of count 16-byte
// entries. Unknown: what either record holds, and the valid index range.
extern "C" {
unsigned char* modelGetCollision__FP12_modelHeaderi(unsigned char* model, int index) {
    unsigned count = model[37];
    if (count == 0) return 0;
    return model + count * 16 + index * 24;
}
}
