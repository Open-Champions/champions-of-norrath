// Observed: CaretOffset__9VIWndEdit at 0x010ad888 is 8 bytes. It returns the word at +404 of this.
// Inferred: +404 is the caret position that SetCaretOffset__9VIWndEditi stores.
// Unknown: the field name and the return type (the word is only ever used as a small count).
extern "C" {
int CaretOffset__9VIWndEdit(const void* self) {
    return static_cast<const int*>(self)[101];
}
}
