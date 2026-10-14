// Observed: gameStateData holds consecutive entries, each a zero-terminated
// string followed by one signed byte; the next entry starts after that byte,
// and a first byte of zero ends the list. scriptGameStateEntry walks the
// entries counting from zero. For the entry whose position equals the first
// argument it stores the signed byte through the second argument and returns
// the address of the entry's string. When the list ends first it returns null
// and writes nothing.
// Inferred: none.
// Unknown: who fills gameStateData, and what the strings and bytes mean.

extern "C" char gameStateData[];
extern "C" __SIZE_TYPE__ strlen(const char* text);

extern "C" char* scriptGameStateEntry__FiPi(int index, int* value) {
    char* entry = gameStateData;
    for (int position = 0; *entry != 0; ++position) {
        if (position == index) {
            *value = entry[strlen(entry) + 1];
            return entry;
        }
        entry += strlen(entry) + 2;
    }
    return 0;
}
