// Observed: Backspace__9VIWndEdit at 0x010ac4c8 is 180 bytes. With this it first stores GetTime__7VITimer() (64 bits)
// at +416. Then, when CaretOffset__9VIWndEdit(this) is not positive it returns 0. Otherwise, with the 16-bit text
// pointer at +424 and the signed word at +428 (a capacity or length in 16-bit units): it calls memmove(text + c1 - 1,
// text + c2, 2 * (the word at +428 - c3)) where c1, c2 and c3 are three further separate CaretOffset results, then
// SetCaretOffset__9VIWndEditi(this, CaretOffset(this) - 1) with a fifth separate call, and returns 1. CaretOffset is
// therefore called five times in total, in that order.
// Inferred: this deletes the character before the caret by moving the tail one place down; +416 is a last-edit time
// (the same slot OnKeyDown__9VIWndEditi uses for the blink phase).
// Unknown: the field names, the exact meaning of the word at +428, and the result type (0 or 1).
extern "C" {
unsigned long long GetTime__7VITimer();
int CaretOffset__9VIWndEdit(void* self);
int SetCaretOffset__9VIWndEditi(void* self, int offset);
void* memmove(void* destination, const void* source, unsigned size);

int Backspace__9VIWndEdit(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned long long*>(bytes + 416) = GetTime__7VITimer();
    if (CaretOffset__9VIWndEdit(self) <= 0) return 0;
    unsigned short* destination = reinterpret_cast<unsigned short*>(
                                      static_cast<unsigned long>(*reinterpret_cast<unsigned*>(bytes + 424))) +
                                  CaretOffset__9VIWndEdit(self) - 1;
    const unsigned short* source = reinterpret_cast<const unsigned short*>(
                                       static_cast<unsigned long>(*reinterpret_cast<unsigned*>(bytes + 424))) +
                                   CaretOffset__9VIWndEdit(self);
    const int count = *reinterpret_cast<int*>(bytes + 428) - CaretOffset__9VIWndEdit(self);
    memmove(destination, source, count * 2);
    SetCaretOffset__9VIWndEditi(self, CaretOffset__9VIWndEdit(self) - 1);
    return 1;
}
}
