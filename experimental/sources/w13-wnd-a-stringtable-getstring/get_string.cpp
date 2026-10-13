// Observed: GetString__13VIStringTableii at 0x0101bc40 is 88 bytes. With (this, id, column) it
// returns a pointer to 16-bit text. The table object holds a row count at +0, a column count at +4
// and a pointer to a block of 32-bit text pointers at +12. When id - 1 is in [0, rows) and column
// is in [0, columns) the entry at index (id - 1) * columns + column of that block is returned if it
// is nonzero. In every other case, including a zero entry, the function returns a pointer to a
// private 14-byte constant that spells "(null)" in 16-bit units (observed words 0x0028 0x006e 0x0075
// 0x006c 0x006c 0x0029 0x0000).
// Inferred: ids are 1-based string numbers and the second number selects a language or variant
// column, from the index arithmetic only.
// Unknown: the meaning of the word at +8, and what the block pointer's owner is.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" const unsigned short* norrath_host_text(unsigned address);
#define NORRATH_TEXT(address) norrath_host_text(address)
#else
#define NORRATH_TEXT(address) reinterpret_cast<const unsigned short*>(static_cast<unsigned long>(address))
#endif

namespace {
const unsigned short kNullString[7] = {0x28, 0x6e, 0x75, 0x6c, 0x6c, 0x29, 0};
}

extern "C" const unsigned short* GetString__13VIStringTableii(const void* self, int id, int column) {
    const int* words = static_cast<const int*>(self);
    const int row = id - 1;
    if (row >= 0 && row < words[0] && column >= 0 && column < words[1]) {
        const unsigned* entries = reinterpret_cast<const unsigned*>(NORRATH_TEXT(static_cast<unsigned>(words[3])));
        if (unsigned entry = entries[row * words[1] + column]) return NORRATH_TEXT(entry);
    }
    return kNullString;
}
