// Observed: FixupTextBuffer__13VIStringTable at 0x0101b5f0 is 472 bytes. Argument: the table object.
// Object words used: +0 and +4 (counts), +8 (pointer to 16-bit text), +12 (pointer to an array of
// pointers). Steps: n = strlen16(text) + 1; set +0 and +4 to 1; scan the n units (the terminating
// zero included): a tab (9) adds one to a running column count (starting at 1); a newline (10)
// raises +4 to the larger of itself and the running column count, resets that count to 1 and adds
// one to +0. The last line's column count is not folded into +4 because only newlines do it.
// Then allocate (+0 * +4) * 4 bytes with __builtin_vec_new and store the result at +12. When the
// allocation succeeds: zero it with memset, store the text start in element 0, and walk the n
// units again with a row/column cursor: a tab clears the unit, advances the column and stores the
// address of the next unit at element (row * +4 + column); a carriage return (13) clears the unit
// only; a newline clears the unit, advances the row, resets the column to 0 and stores the next
// unit's address at element (row * +4). There is no bounds check on that element index. Returns 0.
// When the allocation fails: delete the text (+8) if non-null, delete +12 if non-null, zero +12,
// +0, +4 and +8, and return -1.
// Inferred: splits a tab/newline-delimited text blob into a grid of string starts in place (the
// separators become zero units); +0 is a row count and +4 a column count.
// Unknown: the object's size; the grid indices written for a final line with more tabs than +4
// exceed the allocation (observed, not guarded).
extern "C" {
int strlen16__8VIStringPCUs(const unsigned short* text);
void* __builtin_vec_new(unsigned int size);
void __builtin_vec_delete(void* pointer);
void* memset(void* destination, int value, unsigned int length);

int FixupTextBuffer__13VIStringTable(unsigned char* self) {
    int* rows = reinterpret_cast<int*>(self);
    int* cols = reinterpret_cast<int*>(self + 4);
    unsigned short** text = reinterpret_cast<unsigned short**>(self + 8);
    unsigned short*** index = reinterpret_cast<unsigned short***>(self + 12);

    int count = strlen16__8VIStringPCUs(*text) + 1;
    *cols = 1;
    *rows = 1;
    int column = 1;
    const unsigned short* scan = *text;
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
    *index = static_cast<unsigned short**>(__builtin_vec_new(static_cast<unsigned int>(*rows * *cols) * 4));
    if (*index != 0) {
        memset(*index, 0, static_cast<unsigned int>(*rows * *cols) * 4);
        unsigned short** grid = *index;
        grid[0] = *text;
        int row = 0;
        int col = 0;
        for (int i = 0; i < count; ++i) {
            unsigned short* at = *text + i;
            unsigned short c = *at;
            if (c == 9) {
                ++col;
                *at = 0;
                grid[row * *cols + col] = at + 1;
            } else if (c == 13) {
                *at = 0;
            } else if (c == 10) {
                ++row;
                *at = 0;
                col = 0;
                grid[row * *cols] = at + 1;
            }
        }
        result = 0;
    }
    if (result != 0) {
        if (*text != 0) __builtin_vec_delete(*text);
        if (*index != 0) __builtin_vec_delete(*index);
        *index = 0;
        *rows = 0;
        *cols = 0;
        *text = 0;
    }
    return result;
}
}
