// Observed removeAllQuestsFromLog__Fv at 0x001c1b50, 136 bytes.
// For each of the first nmPlayers (0x00484538, signed word) records of the
// player table (0x00408600, 23984 bytes per record) the signed word at record
// offset 21864 is the number of logged quests. Quest slot i is 64 bytes wide
// starting at record offset 17768. For every slot below the count, read afresh
// on each pass, the nine bytes of the literal "NoQuests" (including its
// terminator) are written at slot offset 0 and again at slot offset 32. The
// count is then set to zero.
extern "C" int nmPlayers;
extern "C" unsigned char player[];

extern "C" void removeAllQuestsFromLog__Fv() {
    for (int index = 0; index < nmPlayers; index++) {
        unsigned char* record = player + index * 23984;
        int* count = reinterpret_cast<int*>(record + 21864);
        for (int slot = 0; slot < *count; slot++) {
            unsigned char* entry = record + 17768 + slot * 64;
            for (int i = 0; i < 9; i++) {
                entry[i] = static_cast<unsigned char>("NoQuests"[i]);
                entry[32 + i] = static_cast<unsigned char>("NoQuests"[i]);
            }
        }
        *count = 0;
    }
}
