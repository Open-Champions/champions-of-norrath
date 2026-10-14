#define NORRATH_W8_HOST 1
#include <cstdio>
#include <cstring>
static const char* const names[] = {"a.tex", "b.etex", "noext", "x.y.tex", "c.", "d.tex.bak", ".tex"};
extern const char* norrath_w8_word_to_string(unsigned word);
const char* norrath_w8_word_to_string(unsigned word) { return names[word]; }
#include "lump_find_by_type.cpp"
static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
int main() {
    // header: 7 entries; each entry is name index, then two filler words.
    unsigned directory[1 + 7 * 3] = {};
    directory[0] = 7u | (0xBEEFu << 16);
    for (unsigned i = 0; i < 7; ++i) { directory[1 + i * 3] = i; directory[2 + i * 3] = 0x11u; directory[3 + i * 3] = 0x22u; }
    unsigned char* base = reinterpret_cast<unsigned char*>(directory);
    char tex[] = "tex";
    char etex[] = "etex";
    char none[] = "zzz";
    char empty[] = "";
    expect(lumpFindByType__FPvPci(directory, tex, 0) == base + 4, "first .tex entry");
    expect(lumpFindByType__FPvPci(directory, tex, 1) == base + 4 + 36, "skip one: x.y.tex (noext, c. and b.etex are not matches)");
    expect(lumpFindByType__FPvPci(directory, tex, 2) == base + 4 + 12 * 6, "skip two: .tex");
    expect(lumpFindByType__FPvPci(directory, tex, 3) == nullptr, "skip past the last match");
    expect(lumpFindByType__FPvPci(directory, etex, 0) == base + 4 + 12, "etex entry");
    expect(lumpFindByType__FPvPci(directory, none, 0) == nullptr, "no such extension");
    expect(lumpFindByType__FPvPci(directory, empty, 0) == base + 4 + 12 * 4, "empty extension matches the name ending in a dot");
    expect(lumpFindByType__FPvPci(directory, tex, -1) == nullptr, "negative skip never stops on a match");
    directory[0] = 0;
    expect(lumpFindByType__FPvPci(directory, tex, 0) == nullptr, "empty directory");
    directory[0] = 1u | (0xBEEFu << 16);
    expect(lumpFindByType__FPvPci(directory, tex, 0) == base + 4, "count uses only the low half");
    return failures;
}
