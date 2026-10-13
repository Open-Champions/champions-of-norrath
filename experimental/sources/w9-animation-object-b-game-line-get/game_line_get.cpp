// Observed gameLineGet__FPc at 0x279ad0, extent 124.
// Searches the table of line pointers gameLines (0x8d57a8) for a line whose name
// matches, ignoring case. The count is the signed word gameLineCount (0x485ea8),
// re-read after every comparison. Each table entry points at a line whose first
// word is its name pointer. Returns the index of the first match, or -1 when none
// matches or the count is not positive.
// norrath_game_line_get is the name the wave-maker constructor imports; it is
// defined as a second symbol for the same code.
// Unknown: the full layout of a line record and the table capacity.
struct LineName {
    const char* name;
};

extern "C" int strcasecmp(const char* left, const char* right);
extern "C" LineName* gameLines[];
extern "C" int gameLineCount;

extern "C" int gameLineGet__FPc(char* name) {
    for (int i = 0; i < gameLineCount; ++i) {
        if (strcasecmp(gameLines[i]->name, name) == 0) return i;
    }
    return -1;
}

#if defined(__mips__)
extern "C" int norrath_game_line_get(char* name) __attribute__((alias("gameLineGet__FPc")));
#endif
