// Observed: FixupTextBuffer__13VIStringTable at 0x0101b5f0, 472 bytes. Argument: the table object. Object words used: +0 (row
// count), +4 (column count), +8 (pointer to 16-bit text), +12 (pointer to an array of pointers). In order it:
//   1. n = strlen16__8VIStringPCUs(text) + 1; sets the words at +0 and +4 to 1;
//   2. scans the n units (the terminating zero included) with a running column count that starts at 1: a tab (9) adds one to it; a
//      newline (10) raises the +4 word to the larger of itself and the running count, resets the count to 1 and adds one to +0.
//      The last line's count is not folded in, because only newlines do it;
//   3. allocates (+0 * +4) * 4 bytes with __builtin_vec_new and stores the result at +12;
//   4. when the allocation is non-null: memset to zero, element 0 = the text start, then a second pass over the n units with a
//      row and column cursor: a tab clears the unit, advances the column and stores the address of the next unit at element
//      (row * (+4 word) + column); a carriage return (13) clears the unit only; a newline clears the unit, advances the row, resets
//      the column to 0 and stores the next unit's address at element (row * (+4 word)). There is no bounds check on that index.
//      Returns 0;
//   5. when the allocation is null: __builtin_vec_delete of the text (if non-null) and of the +12 word (if non-null), then +12, +0,
//      +4 and +8 are zeroed and the result is -1.
// Inferred: splits a tab/newline-delimited text blob into a grid of string starts in place (separators become zero units).
// This version keeps the four object words as 32-bit words, as the image does, so it can be exercised on a 64-bit host.
// Unknown: the object's size; a final line with more tabs than the +4 word exceeds the allocation (observed, not guarded).

// Words that hold 32-bit target addresses: a host check redefines these macros.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#define NORRATH_ADDRESS_WORD(pointer) static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer))
#endif

extern "C" {
int strlen16__8VIStringPCUs(const unsigned short* text);
void* __builtin_vec_new(unsigned int size);
void __builtin_vec_delete(void* pointer);
void* memset(void* destination, int value, unsigned int length);

int FixupTextBuffer__13VIStringTable(unsigned char* self) {
    int* rows = reinterpret_cast<int*>(self);
    int* cols = reinterpret_cast<int*>(self + 4);
    unsigned* text = reinterpret_cast<unsigned*>(self + 8);
    unsigned* index = reinterpret_cast<unsigned*>(self + 12);

    int count = strlen16__8VIStringPCUs(static_cast<unsigned short*>(NORRATH_WORD_ADDRESS(*text))) + 1;
    *cols = 1;
    *rows = 1;
    int column = 1;
    const unsigned short* scan = static_cast<unsigned short*>(NORRATH_WORD_ADDRESS(*text));
    for (int left = count; left > 0; --left) {
        unsigned short c = *scan++;
        if (c == 9) {
            ++column;
        } else if (c == 10) {
            int widest = *cols;
            if (!(column < widest)) widest = column;
            column = 1;
            *cols = widest;
            *rows = *rows + 1;
        }
    }

    int result = -1;
    *index = NORRATH_ADDRESS_WORD(__builtin_vec_new(static_cast<unsigned int>(*rows * *cols) * 4));
    if (*index != 0) {
        memset(NORRATH_WORD_ADDRESS(*index), 0, static_cast<unsigned int>(*rows * *cols) * 4);
        unsigned* grid = static_cast<unsigned*>(NORRATH_WORD_ADDRESS(*index));
        grid[0] = *text;
        int row = 0;
        int col = 0;
        for (int i = 0; i < count; ++i) {
            unsigned short* at = static_cast<unsigned short*>(NORRATH_WORD_ADDRESS(*text)) + i;
            unsigned short c = *at;
            if (c == 9) {
                ++col;
                *at = 0;
                grid[row * *cols + col] = NORRATH_ADDRESS_WORD(at + 1);
            } else if (c == 13) {
                *at = 0;
            } else if (c == 10) {
                ++row;
                *at = 0;
                col = 0;
                grid[row * *cols] = NORRATH_ADDRESS_WORD(at + 1);
            }
        }
        result = 0;
    }
    if (result != 0) {
        if (*text != 0) __builtin_vec_delete(NORRATH_WORD_ADDRESS(*text));
        if (*index != 0) __builtin_vec_delete(NORRATH_WORD_ADDRESS(*index));
        *index = 0;
        *rows = 0;
        *cols = 0;
        *text = 0;
    }
    return result;
}
}
