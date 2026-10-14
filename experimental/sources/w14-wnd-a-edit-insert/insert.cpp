// Observed: Insert__9VIWndEditUs at 0x010ac3b8 is 268 bytes. With (this, ch), only the low 16 bits of ch are used.
// It returns 0 without changes when the word at +444 is zero and ch is 32, and also when
// CaretOffset__9VIWndEdit(this) + 1 is not below the signed word at +428. Otherwise it stores GetTime__7VITimer()
// (64 bits) at +416 and, with the 16-bit text pointer at +424 and n the word at +428, does in order (each CaretOffset
// term being its own call, six calls in all counting the length test above): memmove(text + c1 + 1, text + c2,
// 2 * (n - c3) - 2); text[n - 1] = 0; text[CaretOffset()] = ch; then SetCaretOffset__9VIWndEditi(this,
// CaretOffset() + 1) and returns 1.
// Inferred: this inserts one character at the caret, shifting the tail up and keeping the buffer terminated; +444 is
// a flag that allows spaces and +428 the buffer size in 16-bit units including the terminator.
// Unknown: the field names, and the result type (0 or 1).
extern "C" {
unsigned long long GetTime__7VITimer();
int CaretOffset__9VIWndEdit(void* self);
int SetCaretOffset__9VIWndEditi(void* self, int offset);
void* memmove(void* destination, const void* source, unsigned size);

static inline unsigned short* text_of(unsigned char* bytes) {
    return reinterpret_cast<unsigned short*>(static_cast<unsigned long>(*reinterpret_cast<unsigned*>(bytes + 424)));
}

int Insert__9VIWndEditUs(void* self, unsigned argument) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    const unsigned short ch = static_cast<unsigned short>(argument);
    if (*reinterpret_cast<int*>(bytes + 444) == 0 && ch == 32) return 0;
    if (!(CaretOffset__9VIWndEdit(self) + 1 < *reinterpret_cast<int*>(bytes + 428))) return 0;
    *reinterpret_cast<unsigned long long*>(bytes + 416) = GetTime__7VITimer();
    unsigned short* destination = text_of(bytes) + CaretOffset__9VIWndEdit(self) + 1;
    const unsigned short* source = text_of(bytes) + CaretOffset__9VIWndEdit(self);
    const int count = *reinterpret_cast<int*>(bytes + 428) - CaretOffset__9VIWndEdit(self);
    memmove(destination, source, count * 2 - 2);
    text_of(bytes)[*reinterpret_cast<int*>(bytes + 428) - 1] = 0;
    text_of(bytes)[CaretOffset__9VIWndEdit(self)] = ch;
    SetCaretOffset__9VIWndEditi(self, CaretOffset__9VIWndEdit(self) + 1);
    return 1;
}
}
