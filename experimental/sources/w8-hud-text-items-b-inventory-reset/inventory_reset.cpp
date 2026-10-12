// Observed inventoryReset__Fv at 0x001b1808, 60 bytes.
// For each of the first nmPlayers (0x00484538) records of the table named
// inventory (0x00427288, 116 bytes per record, 464 bytes in the symbol table)
// it clears the words at record offsets 24 and 36. A count of zero or less does
// nothing. The meaning of those two words is not established. The table and
// nmPlayers are not defined here.
extern "C" int nmPlayers;
extern "C" unsigned char inventory[];

extern "C" void inventoryReset__Fv() {
    unsigned char* record = inventory;
    for (int remaining = nmPlayers; remaining > 0; --remaining) {
        *reinterpret_cast<int*>(record + 24) = 0;
        *reinterpret_cast<int*>(record + 36) = 0;
        record += 116;
    }
}
