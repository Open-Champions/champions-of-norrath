#include "../save-and-quests-bytes/bytes.hpp"
// Observed addQuestToLog__FPcT0 at 0x001c1a60, 236 bytes. Each player record
// (symbol player at 0x00408600, 23984-byte stride, nmPlayers records) holds a
// quest log: up to 64 entries of 64 bytes starting at record byte 17768 (name
// text first, then a description text at entry byte 32), with the entry count
// as a word at record byte 21864. Walking the records in order, a record whose
// count is already 64 or more ends the whole function, and so does the first
// record that already has an entry with this name (strcmp equal). Otherwise
// the name and description are copied (unbounded) into the entry at the count
// and the count grows by one. nmPlayers is read again after every record.
// A non-positive nmPlayers does nothing.
extern "C" {
char* strcpy(char* destination, const char* source);
int strcmp(const char* left, const char* right);
extern int nmPlayers;
extern unsigned char player[];

void addQuestToLog__FPcT0(char* name, char* description) {
    if (nmPlayers <= 0) return;
    unsigned char* record = player;
    int done = 0;
    do {
        const int count = sq::signed_word(sq::read_word(record + 21864));
        if (count >= 64) return;
        for (int index = 0; index < count; ++index) {
            if (strcmp(reinterpret_cast<char*>(record + 17768 + index * 64), name) == 0) return;
        }
        strcpy(reinterpret_cast<char*>(record + 17768 + count * 64), name);
        strcpy(reinterpret_cast<char*>(record + 17800 + count * 64), description);
        sq::write_word(record + 21864, unsigned(count) + 1u);
        record += 23984;
        done += 1;
    } while (done < nmPlayers);
}
}
