#include "../save-and-quests-bytes/bytes.hpp"
// Observed removeQuestFromLog__FPc at 0x001be638, 364 bytes. It uses the same
// player records and quest-log layout as addQuestToLog (64-byte entries from
// record byte 17768, description at entry byte 32, count word at record byte
// 21864). For each of nmPlayers records (re-read after each) every entry whose
// name equals the argument has nine bytes copied over its name and over its
// description from a nine-byte constant in the image (the bytes 'h' 'i' 't',
// five zeros and 'c'; the last byte belongs to the next literal there). The
// last matching index is kept. If one was found, every later entry is moved
// down one place by copying its name and description text (strcpy), and the
// count word drops by one. A record that has no match, or a count of zero or
// less, is left alone. Unlike the add call, a record never ends the walk.
extern "C" {
int strcmp(const char* left, const char* right);
char* strcpy(char* destination, const char* source);
extern int nmPlayers;
extern unsigned char player[];

void removeQuestFromLog__FPc(char* name) {
    static const unsigned char fill[9] = {'h', 'i', 't', 0, 0, 0, 0, 0, 'c'};
    for (int slot = 0; slot < nmPlayers; ++slot) {
        unsigned char* record = player + slot * 23984;
        int found = -1;
        for (int index = 0; index < sq::signed_word(sq::read_word(record + 21864)); ++index) {
            unsigned char* entry = record + index * 64;
            if (strcmp(name, reinterpret_cast<char*>(entry + 17768)) == 0) {
                for (int byte = 0; byte < 9; ++byte) entry[17768 + byte] = fill[byte];
                found = index;
                for (int byte = 0; byte < 9; ++byte) entry[17800 + byte] = fill[byte];
            }
        }
        if (found != -1) {
            for (int index = found; index < sq::signed_word(sq::read_word(record + 21864)) - 1; ++index) {
                unsigned char* entry = record + index * 64;
                strcpy(reinterpret_cast<char*>(entry + 17768), reinterpret_cast<char*>(entry + 17832));
                strcpy(reinterpret_cast<char*>(entry + 17800), reinterpret_cast<char*>(entry + 17864));
            }
            sq::write_word(record + 21864, unsigned(sq::signed_word(sq::read_word(record + 21864)) - 1));
        }
    }
}
}
