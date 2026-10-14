// Observed: updateRamSlot__Fii at 0x002591d8 is 1824 bytes. Arguments: first
// (stored as a byte at header +109) and second.
// Level name: a 256-byte stack area holds the name. When the byte gameOver
// (0x00484f99) is nonzero, 13 bytes are copied into it from a read-only
// literal at 0x00474a68 (the 12-character identifier below plus a terminator).
// Otherwise gameGetFullLevelName__FPc(name) fills it. The name is then looked
// up like the checkpoint saver does: for each act below actCount, for each
// level below actLevelCount[act], an 8-byte entry of allActs[act] has a name
// pointer in word 0, and strncmp(entry name, name, strlen(entry name)) equal
// to zero selects that act. No match returns before anything is written, and
// MC_SaveLevelEntities is not called.
// Header at ramSlot (0x0081e240): when gameOver is nonzero the 13 bytes of the
// literal are copied to +0, otherwise strcpy(+0, levelName + 81); then
// strcpy(+20, levelName + 161) (levelName is 0x0034b660, 322 bytes). Word +100
// is curPlaySeconds (0x00486ca4), half word +104 is 23984, half word +106 is 1
// when second is nonzero and otherwise the low half of nmPlayers
// (0x00484538), byte +108 is gameDifficulty (0x00484cd8), byte +109 is first.
// The cursor then starts at +112. Camera::Save(gameCam, cursor) and
// jumpGateSave(cursor) each return a length that advances the cursor. 4096
// bytes of levelSeeds (0x004349a8) are copied, then the word gameAutoMapMode
// (0x00484f30).
// Player block: if second is nonzero, the word 1 is stored; the index is the
// first i below nmPlayers with netPlayerIsLocal(i) nonzero (nmPlayers when
// none); convertPointersToIndicies(i); the player record's word at +21872
// (player is 0x00408600, stride 23984) names an object; two bytes at
// object+56 and +57, 12 bytes from object+16, then 23984 bytes from
// tempPlayerStruct (0x007e05e0) record i are written. Then the cursor moves a
// further 3 x 23998 bytes (14 + 23984 per record). If second is zero, the word
// nmPlayers is stored, then for i in 0..3: when i is below nmPlayers,
// convertPointersToIndicies(i) runs and the object word of player i is used
// for the 14-byte header, falling back to player validLocalPlayer
// (0x00484f40) when that is nonnull and validLocalPlayer is not negative, and
// otherwise to the first player below nmPlayers with a nonnull object word
// (an index left as validLocalPlayer when none exists); the 23984 bytes of
// tempPlayerStruct record i follow. Records at or past nmPlayers advance the
// cursor 23998 without writing.
// The cursor is then moved up to a multiple of 4 from ramSlot. For
// index = 0, 1, ...: scriptGameStateEntry(index, &value) returns a name; zero
// ends the list. Otherwise strcpy(cursor, name), word +32 = value, cursor += 36.
// Finally MC_SaveLevelEntities(name) runs with the stack name.
// Inferred: the object named by the word at player+21872 is the player's
// game object; the 23984-byte copies are whole player records; the strings at
// levelName+81 and +161 are the two slot title lines.
// Unknown: header bytes not named above (+13..+19, +20 onward to +99, +100
// plus the part of the title string after the terminator); what the object
// bytes at +56, +57 and the 12 bytes at +16 mean; what happens when the object
// word of the chosen fallback is zero (the code reads through it).
extern "C" {
extern unsigned char gameOver;
extern int nmPlayers;
extern int curPlaySeconds;
extern unsigned char gameDifficulty;
extern void* gameCam;
extern unsigned char levelSeeds[4096];
extern int gameAutoMapMode;
extern unsigned char player[];
extern unsigned char tempPlayerStruct[];
extern int validLocalPlayer;
extern int actCount;
extern int actLevelCount[];
extern unsigned char* allActs[];
extern char levelName[322];
extern unsigned char ramSlot[];

void gameGetFullLevelName__FPc(char* name);
int Save__6CameraPc(void* camera, unsigned char* out);
int jumpGateSave__FPc(unsigned char* out);
int netPlayerIsLocal__Fi(int index);
void convertPointersToIndicies__Fi(int index);
const char* scriptGameStateEntry__FiPi(int index, int* value);
void MC_SaveLevelEntities__FPc(char* name);
unsigned long strlen(const char* text);
int strncmp(const char* left, const char* right, unsigned long count);
char* strcpy(char* destination, const char* source);
void* memcpy(void* destination, const void* source, unsigned long size);
}

namespace {
const unsigned player_stride = 23984;

unsigned load_word(const unsigned char* at) {
    return static_cast<unsigned>(at[0]) | (static_cast<unsigned>(at[1]) << 8) |
           (static_cast<unsigned>(at[2]) << 16) | (static_cast<unsigned>(at[3]) << 24);
}

#ifdef NORRATH_HOST_ARENA
extern "C" const unsigned char* norrath_host_objects[];
extern "C" const char* norrath_host_names[];
const unsigned char* object_from(unsigned word) { return norrath_host_objects[word]; }
const char* name_from(const unsigned char* entry) { return norrath_host_names[load_word(entry)]; }
#else
const unsigned char* object_from(unsigned word) {
    return reinterpret_cast<const unsigned char*>(word);
}
const char* name_from(const unsigned char* entry) {
    return reinterpret_cast<const char*>(load_word(entry));
}
#endif

const unsigned char* object_of(int index) {
    return object_from(load_word(player + index * player_stride + 21872));
}

bool level_in_an_act(const char* name) {
    int acts = actCount;
    int act = 0;
    for (; act < acts; ++act) {
        int levels = actLevelCount[act];
        if (levels <= 0) continue;
        const unsigned char* table = allActs[act];
        for (int level = 0; level < levels; ++level) {
            const char* entry = name_from(table + level * 8);
            if (strncmp(entry, name, strlen(entry)) == 0) return true;
        }
    }
    return false;
}

unsigned char* put_object_header(unsigned char* cursor, const unsigned char* object) {
    cursor[0] = object[56];
    cursor[1] = object[57];
    memcpy(cursor + 2, object + 16, 12);
    return cursor + 14;
}

unsigned char* put_player_data(unsigned char* cursor, int index) {
    memcpy(cursor, tempPlayerStruct + index * player_stride, player_stride);
    return cursor + player_stride;
}
}  // namespace

extern "C" void updateRamSlot__Fii(int first, int second) {
    char name[256];
    if (gameOver != 0) {
        memcpy(name, "gameComplete", 13);
    } else {
        gameGetFullLevelName__FPc(name);
    }
    if (!level_in_an_act(name)) return;

    unsigned char* const slot = ramSlot;
    unsigned char* cursor = slot;
    if (gameOver != 0) {
        memcpy(cursor, "gameComplete", 13);
    } else {
        strcpy(reinterpret_cast<char*>(cursor), levelName + 81);
    }
    strcpy(reinterpret_cast<char*>(cursor + 20), levelName + 161);
    memcpy(cursor + 100, &curPlaySeconds, 4);
    const unsigned short record_size = static_cast<unsigned short>(player_stride);
    memcpy(cursor + 104, &record_size, 2);
    const unsigned short count = second != 0 ? static_cast<unsigned short>(1)
                                              : static_cast<unsigned short>(nmPlayers);
    memcpy(cursor + 106, &count, 2);
    cursor[108] = gameDifficulty;
    cursor[109] = static_cast<unsigned char>(first);
    cursor += 112;

    cursor += Save__6CameraPc(gameCam, cursor);
    cursor += jumpGateSave__FPc(cursor);
    memcpy(cursor, levelSeeds, 4096);
    cursor += 4096;
    memcpy(cursor, &gameAutoMapMode, 4);
    cursor += 4;

    if (second != 0) {
        const int one = 1;
        memcpy(cursor, &one, 4);
        cursor += 4;
        int index = 0;
        while (index < nmPlayers && netPlayerIsLocal__Fi(index) == 0) ++index;
        convertPointersToIndicies__Fi(index);
        cursor = put_object_header(cursor, object_of(index));
        cursor = put_player_data(cursor, index);
        cursor += 3 * (14 + player_stride);
    } else {
        memcpy(cursor, &nmPlayers, 4);
        cursor += 4;
        for (int index = 0; index < 4; ++index) {
            if (index >= nmPlayers) {
                cursor += 14 + player_stride;
                continue;
            }
            convertPointersToIndicies__Fi(index);
            const unsigned char* object = object_of(index);
            if (load_word(player + index * player_stride + 21872) == 0) {
                int choice = validLocalPlayer;
                if (choice < 0 || load_word(player + choice * player_stride + 21872) == 0) {
                    for (int search = 0; search < nmPlayers; ++search) {
                        if (load_word(player + search * player_stride + 21872) != 0) {
                            choice = search;
                            break;
                        }
                    }
                }
                object = object_of(choice);
            }
            cursor = put_object_header(cursor, object);
            cursor = put_player_data(cursor, index);
        }
    }

    while ((static_cast<unsigned>(cursor - slot) & 3u) != 0) ++cursor;
    int value = 0;
    for (int index = 0;; ++index) {
        const char* entry = scriptGameStateEntry__FiPi(index, &value);
        if (entry == nullptr) break;
        strcpy(reinterpret_cast<char*>(cursor), entry);
        memcpy(cursor + 32, &value, 4);
        cursor += 36;
    }
    MC_SaveLevelEntities__FPc(name);
}
