// Observed: SetCaretOffset__9VIWndEditi at 0x010ad890 is 120 bytes. With (this, offset): the length is
// strlen16__8VIStringPCUs of the 16-bit text pointer at +424, or 0 when that pointer is null. The offset is
// limited to that length from above and to 0 from below. The word at +408 is set to -1, the limited value is
// stored in the word at +404, and the function returns 1 when the new +404 value differs from the old one,
// else 0.
// Inferred: +424 is the edit text, +404 the caret position and +408 a second position that is cleared to -1
// (a selection anchor is a guess from the value only).
// Unknown: the field names, and the return type (the result is only ever 0 or 1).
extern "C" {
int strlen16__8VIStringPCUs(const unsigned short* text);

int SetCaretOffset__9VIWndEditi(void* self, int offset) {
    int* words = static_cast<int*>(self);
    const unsigned short* text = reinterpret_cast<const unsigned short*>(static_cast<unsigned>(words[106]));
    const int length = text ? strlen16__8VIStringPCUs(text) : 0;
    int caret = offset < length ? offset : length;
    if (caret < 0) caret = 0;
    const int old_caret = words[101];
    words[102] = -1;
    words[101] = caret;
    return caret != old_caret;
}
}
