// Observed: GetCaretPos__9VIWndEditP8VIVect2I at 0x010ad908 is 136 bytes. With (this, out): rows are 8-byte records
// at the pointer in the word at +432, counted by the signed word at +436 (nothing is read when it is <= 0 and the
// result is -1). The caret is the word at +404. Each record holds two signed 16-bit values at record offsets 4
// (a length) and 6 (a flag). Walking the records with a running start offset (0 for the first): when start+length is
// below the caret the start advances to start+length and the walk continues; otherwise out[1] gets the record index
// and out[0] the caret minus the start, except that when the flag is nonzero and the caret minus the start equals the
// length the result is out[0] = 0 and out[1] = index+1. The result is 0 then; -1 when the caret lies beyond every
// record. out is untouched on the -1 paths.
// Inferred: this maps a character offset to a (column, row) pair for the wrapped rows computed by
// ComputeTextRows__9VIWndEdit, the flag marking a row that ended in a line break so a caret at its end moves to
// the next row.
// Unknown: the record names, the flag's exact meaning, and the order of out's two words being (column, row).
extern "C" {
int GetCaretPos__9VIWndEditP8VIVect2I(const void* self, int* out) {
    const unsigned char* base = static_cast<const unsigned char*>(self);
    const int count = *reinterpret_cast<const int*>(base + 436);
    if (count <= 0) return -1;
    const int caret = *reinterpret_cast<const int*>(base + 404);
    const unsigned char* row =
        reinterpret_cast<const unsigned char*>(static_cast<unsigned long>(*reinterpret_cast<const unsigned*>(base + 432)));
    int start = 0;
    for (int index = 0; index < count; ++index, row += 8) {
        const int length = *reinterpret_cast<const short*>(row + 4);
        const int end = start + length;
        if (end < caret) {
            start = end;
            continue;
        }
        const int flag = *reinterpret_cast<const short*>(row + 6);
        if (flag != 0 && caret - start == length) {
            out[0] = 0;
            out[1] = index + 1;
        } else {
            out[1] = index;
            out[0] = caret - start;
        }
        return 0;
    }
    return -1;
}
}
