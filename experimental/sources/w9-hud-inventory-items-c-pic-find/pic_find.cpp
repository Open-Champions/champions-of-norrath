// Observed picFind__4itemPc at 0x00100258, 364 bytes (a file-local member of item).
// Argument: a picture name such as "name12.ext". It copies the name to an 80-byte
// local buffer, cuts it at the last '.' (when there is one), and returns 0 for an
// empty result. It then finds the trailing run of decimal digits: the run starts
// at the last character if that is a digit and extends left while the characters
// are digits, but the character at index 0 is never counted. The digits are read
// with atoi when the run is not empty (otherwise the wanted number is -1), the
// string is cut at the start of the run, and the stem is compared with strcmp to
// the name word (+4) of each of the nmPicGroups 12-byte group records that
// picInit set up at _fbss (_4item$picGroup, 0x00486800). The first equal group
// has a count (+8) and a list (+0) of 10-byte entries whose first halfword is a
// number; the first entry whose halfword equals the wanted number is returned (as
// a pointer). A stem match whose entries lack that number keeps searching later
// groups. The result is 0 when nothing matches.
// Record words are 32 bits (the low half of the address).
// Unknown: what the rest of the 10-byte entry holds and what the numbers mean.
#include "../inventory-items-record/record.hpp"
extern "C" {
extern unsigned char* _fbss;
extern int _4item$nmPicGroups;
char* strcpy(char* to, const char* from);
char* strrchr(const char* text, int c);
unsigned strlen(const char* text);
int strcmp(const char* left, const char* right);
int atoi(const char* text);
}

namespace {
bool digit(char c) { return c >= '0' && c < ':'; }
}

extern "C" const unsigned char* picFind__4itemPc(const char* name) {
    using namespace inventory_items;
    char buffer[80];
    strcpy(buffer, name);
    if (strrchr(buffer, '.') != 0) *strrchr(buffer, '.') = 0;
    if (buffer[0] == 0) return 0;
    char* run = buffer + strlen(buffer) - 1;
    if (buffer < run && digit(*run)) {
        --run;
        while (buffer < run && digit(*run)) --run;
        ++run;
    } else {
        ++run;
    }
    int wanted = atoi(run);
    if (*run == 0) wanted = -1;
    *run = 0;
    if (_4item$nmPicGroups <= 0) return 0;
    for (int group = 0; group < _4item$nmPicGroups; ++group) {
        const unsigned* record = reinterpret_cast<const unsigned*>(_fbss + group * 12);
        if (strcmp(buffer, resolve<char>(record[1])) == 0) {
            const int count = *reinterpret_cast<const int*>(_fbss + group * 12 + 8);
            const unsigned char* entry = resolve<unsigned char>(*reinterpret_cast<const unsigned*>(_fbss + group * 12));
            for (int index = 0; index < count; ++index, entry += 10) {
                if (*reinterpret_cast<const short*>(entry) == wanted) return entry;
            }
        }
    }
    return 0;
}
