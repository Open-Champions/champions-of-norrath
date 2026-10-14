// gameLineClear at 0x002798d8, 100 bytes, in SLUS_205.65.
// Observed: gameLines (0x008d57a8, local BSS) is an array of pointers and gameLineCount
// (0x00485ea8, local word) is how many are in use. For each index below the count, a nonzero
// pointer is passed to the Line destructor _$_4Line with flag value 3. The count is read again
// after each destructor call, and the loop test uses that fresh value. Afterwards the count is
// set to zero. A count of zero or less skips the loop and still stores zero. Nothing is returned.
// Inferred: bit 0 of the flag makes the destructor free the object, as the destructor's own
// body tests it; bit 1 is not used there. Unknown: the capacity of gameLines.
extern "C" int gameLineCount;
extern "C" void* gameLines[];
extern "C" void destroyLine(void* line, int flags) asm("_$_4Line");

extern "C" void gameLineClear__Fv() {
    int count = gameLineCount;
    int index = 0;
    if (count > 0) {
        void** slot = gameLines;
        do {
            void* line = *slot++;
            if (line != 0) {
                destroyLine(line, 3);
                count = gameLineCount;
            }
            ++index;
        } while (index < count);
    }
    gameLineCount = 0;
}
