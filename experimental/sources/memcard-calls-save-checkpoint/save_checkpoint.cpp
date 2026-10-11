// Observed: MC_SaveCheckPoint__Fiii at 0x00250d48 is 780 bytes. The third
// argument is not read. It scans player the same way MC_SaveGameBrowser
// does and returns when no record qualifies. curAct of -1 skips the name
// search. Otherwise it asks gameGetFullLevelName for a stack name and walks
// actLevelCount / allActs. Each level entry is 8 bytes. The compare is
// strncmp of the table name against the stack name for strlen of the table
// name. No match shows textGetString("CheckPoint") through
// hudSetInfoMessage with 180, -1, 0 and returns. A match compares the
// current act's first level name with strcmp. Equal names call
// MC_HasThisLevelBeenSaved, and a zero result skips the message. Any other
// result, and every unequal name, compares the stack name with "nas".
// Anything else shows either the CheckPoint text or getLongLevelName,
// selected by a zero or nonzero second argument. updateRamSlot then receives
// the first argument and 0, MC_CheckPointAge is cleared, and each player
// copies 12 bytes from object+16 into MC_SpawnPoint. A zero object word
// instead copies player[playersAlive[0]] into that index's 12-byte slot.
// The sequential slot still advances.
// Inferred: player stride is 23984. Spawn records are 12 bytes.
// Unknown: the meaning of the third argument and of the 12 copied bytes.

extern "C" int nmPlayers;
extern "C" unsigned char player[];
extern "C" int curAct;
extern "C" int actCount;
extern "C" int actLevelCount[];
extern "C" unsigned char* allActs[];
extern "C" int playersAlive[];
extern "C" unsigned char MC_SpawnPoint[];
extern "C" int MC_CheckPointAge;
extern "C" void gameGetFullLevelName__FPc(char* name);
extern "C" int strlen(const char* text);
extern "C" int strncmp(const char* left, const char* right, unsigned count);
extern "C" int strcmp(const char* left, const char* right);
extern "C" const char* textGetString__FPc(const char* key);
extern "C" const char* getLongLevelName__FPc(const char* name);
extern "C" void hudSetInfoMessage__FPUsiic(const char* text, int first, int second, int third);
extern "C" int MC_HasThisLevelBeenSaved__Fv();
extern "C" void updateRamSlot__Fii(int first, int second);

namespace {
unsigned load_word(const unsigned char* at) {
    return static_cast<unsigned>(at[0]) | (static_cast<unsigned>(at[1]) << 8) |
           (static_cast<unsigned>(at[2]) << 16) | (static_cast<unsigned>(at[3]) << 24);
}

float load_float(const unsigned char* at) {
    union {
        unsigned bits;
        float value;
    } pun;
    pun.bits = load_word(at);
    return pun.value;
}

void copy12(const unsigned char* from, unsigned char* to) {
    for (int index = 0; index < 12; ++index) to[index] = from[index];
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

int living_player() {
    int count = nmPlayers;
    int any = 0;
    if (count > 0) {
        unsigned char* record = player + 172;
        do {
            unsigned word = load_word(record + 21700);
            if (word != 0) {
                unsigned flags = load_word(object_from(word) + 12);
                if ((flags & 8u) == 0 && 0.0f < load_float(record)) any = 1;
            }
            record += 23984;
            count -= 1;
        } while (count != 0);
    }
    return any;
}

int find_act(const char* query) {
    int acts = actCount;
    if (acts <= 0) return -1;
    for (int act = 0; act < acts; ++act) {
        int levels = actLevelCount[act];
        if (levels <= 0) continue;
        unsigned char* table = allActs[act];
        for (int level = 0; level < levels; ++level) {
            const char* entry = name_from(table + level * 8);
            if (strncmp(entry, query, static_cast<unsigned>(strlen(entry))) == 0) return act;
        }
    }
    return -1;
}

void copy_spawns() {
    int count = nmPlayers;
    if (count <= 0) return;
    unsigned char* destination = MC_SpawnPoint;
    unsigned char* slot = player + 21872;
    do {
        unsigned word = load_word(slot);
        slot += 23984;
        if (word != 0) {
            copy12(object_from(word) + 16, destination);
        } else {
            int index = playersAlive[0];
            unsigned char* other = player + index * 23984 + 21872;
            unsigned other_word = load_word(other);
            copy12(object_from(other_word) + 16, MC_SpawnPoint + index * 12);
        }
        destination += 12;
        count -= 1;
    } while (count != 0);
}
}

extern "C" void MC_SaveCheckPoint__Fiii(int first, int second, int third) {
    (void)third;
    if (living_player() == 0) return;
    if (curAct != -1) {
        char name[256];
        gameGetFullLevelName__FPc(name);
        if (find_act(name) == -1) {
            hudSetInfoMessage__FPUsiic(textGetString__FPc("CheckPoint"), 180, -1, 0);
            return;
        }
        const char* first_name = name_from(allActs[curAct]);
        if (strcmp(first_name, name) != 0 || MC_HasThisLevelBeenSaved__Fv() != 0) {
            if (strcmp(name, "nas") != 0) {
                const char* text = second == 0 ? textGetString__FPc("CheckPoint")
                                                : getLongLevelName__FPc(name);
                hudSetInfoMessage__FPUsiic(text, 180, -1, 0);
            }
        }
    }
    updateRamSlot__Fii(first, 0);
    MC_CheckPointAge = 0;
    copy_spawns();
}
