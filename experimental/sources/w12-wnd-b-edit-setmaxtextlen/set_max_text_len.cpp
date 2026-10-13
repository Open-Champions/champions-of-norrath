// Observed: SetMaxTextLen__9VIWndEditi at 0x010aa938 is 188 bytes. With (this, length) it:
//  1. stores length at byte offset 428;
//  2. calls __builtin_vec_delete on the word at +424 when it is nonzero, then on the word at +432
//     when that is nonzero;
//  3. stores __builtin_vec_new(length * 2) at +424 and __builtin_vec_new(length * 8) at +432
//     (the two allocations are made in that order);
//  4. writes a zero halfword at the start of the +424 block, and, when the stored length (re-read
//     from +428) is positive, clears each of the length 8-byte records of the +432 block: a zero
//     word at record offset 0 and zero halfwords at offsets 4 and 6;
//  5. stores zero at +436 and at +400, then calls SetCaretOffset__9VIWndEditi(this, 0).
// Nothing is returned. A non-positive length skips the record loop but still allocates.
// Inferred: the +424 block is a text buffer of 16-bit units and the +432 block is a per-character
// record array; +436, +400 and the caret offset are reset because the contents changed.
// Unknown: the record fields' meaning; the fields at +400 and +436; behavior for a failed
// allocation (the result is used unchecked).
extern "C" {
void* __builtin_vec_new(unsigned long size);
void __builtin_vec_delete(void* pointer);
void SetCaretOffset__9VIWndEditi(void* self, int offset);

static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word));
}
static inline unsigned word_of(void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}

void SetMaxTextLen__9VIWndEditi(void* self, int length) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    *reinterpret_cast<int*>(bytes + 428) = length;
    if (unsigned block = *reinterpret_cast<unsigned*>(bytes + 424)) __builtin_vec_delete(pointer_of(block));
    if (unsigned block = *reinterpret_cast<unsigned*>(bytes + 432)) __builtin_vec_delete(pointer_of(block));
    unsigned char* text = static_cast<unsigned char*>(__builtin_vec_new(length * 2));
    *reinterpret_cast<unsigned*>(bytes + 424) = word_of(text);
    unsigned char* records = static_cast<unsigned char*>(__builtin_vec_new(length * 8));
    *reinterpret_cast<unsigned*>(bytes + 432) = word_of(records);
    *reinterpret_cast<unsigned short*>(text) = 0;
    for (int left = *reinterpret_cast<int*>(bytes + 428); left > 0; --left, records += 8) {
        *reinterpret_cast<unsigned*>(records) = 0;
        *reinterpret_cast<unsigned short*>(records + 4) = 0;
        *reinterpret_cast<unsigned short*>(records + 6) = 0;
    }
    *reinterpret_cast<unsigned*>(bytes + 436) = 0;
    *reinterpret_cast<unsigned*>(bytes + 400) = 0;
    SetCaretOffset__9VIWndEditi(self, 0);
}
}
