// Host check only; not a build input.
#include <string.h>
#include <string>
static unsigned char arena[0x1000];
#define NORRATH_WORD_ADDRESS(word) (static_cast<void*>(arena + (word)))
static unsigned wordOf(const void* p) { return p ? static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena) : 0; }
#define NORRATH_ADDRESS_WORD(pointer) wordOf(pointer)
#define memset host_memset
#include "fixup.cpp"
#undef memset
extern "C" void* host_memset(void* d, int v, unsigned int n) { ::memset(d, v, n); return d; }
static bool allocFails; static unsigned lastNew; static std::string tr;
extern "C" int strlen16__8VIStringPCUs(const unsigned short* t) { int n = 0; while (t[n]) ++n; return n; }
extern "C" void* __builtin_vec_new(unsigned int size) { lastNew = size; tr += "new" + std::to_string(size) + ";"; return allocFails ? nullptr : arena + 0x800; }
extern "C" void __builtin_vec_delete(void* p) { tr += "del" + std::to_string(static_cast<unsigned char*>(p) - arena) + ";"; }
static void setText(const char* s, unsigned at) {
    unsigned short* t = reinterpret_cast<unsigned short*>(arena + at);
    size_t n = strlen(s);
    for (size_t i = 0; i <= n; ++i) t[i] = static_cast<unsigned char>(s[i]);
}
int main() {
    alignas(8) unsigned char table[16];
    unsigned* w = reinterpret_cast<unsigned*>(table);
    unsigned* grid = reinterpret_cast<unsigned*>(arena + 0x800);
    // "a\tb\r\nc\n\td" : row 0 has 2 columns, row 1 has 1, row 2 has 2
    setText("a\tb\r\nc\n\td", 0x100);
    w[2] = 0x100; w[3] = 0x55; w[0] = w[1] = 9;
    allocFails = false; tr.clear();
    if (FixupTextBuffer__13VIStringTable(table) != 0) return 1;
    if (w[0] != 3 || w[1] != 2 || w[3] != 0x800 || tr != "new24;") return 2;
    unsigned short* t = reinterpret_cast<unsigned short*>(arena + 0x100);
    if (grid[0] != 0x100 || grid[1] != 0x100 + 2 * 2) return 3;
    // row 1 starts after the newline at unit 4 (unit 5), row 2 after the newline at unit 6 (unit 7)
    if (grid[2] != 0x100 + 2 * 5 || grid[4] != 0x100 + 2 * 7 || grid[5] != 0x100 + 2 * 8) return 4;
    if (t[1] != 0 || t[3] != 0 || t[4] != 0 || t[6] != 0 || t[7] != 0 || t[0] != 'a' || t[2] != 'b' || t[5] != 'c') return 5;
    // allocation failure: text and the (zero) index word are released as the image does
    setText("x", 0x100);
    w[2] = 0x100; w[3] = 0x55; allocFails = true; tr.clear();
    if (FixupTextBuffer__13VIStringTable(table) != -1 || tr != "new4;del256;") return 6;
    if (w[0] != 0 || w[1] != 0 || w[2] != 0 || w[3] != 0) return 7;
    // no separators: one row, one column
    setText("abc", 0x100);
    w[2] = 0x100; allocFails = false;
    if (FixupTextBuffer__13VIStringTable(table) != 0 || w[0] != 1 || w[1] != 1 || grid[0] != 0x100) return 8;
    return 0;
}
