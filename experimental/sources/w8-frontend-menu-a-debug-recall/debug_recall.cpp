// Observed debugRecall__Fi at 0x0017ddb0, 804 bytes, returns nothing. The
// argument is a player index; player records are 23984 bytes apart in
// `player`. One caller in the authored sources is the HUD message callback.
//
// Observed flow:
//  1. debugClearStartMenu__Fv().
//  2. If gameCheckNoSaveZone__Fv() is non-zero, show the text for
//     "ErrorCannotRecall" with hudSetInfoMessage(text, 180, -1, 0) and stop.
//  3. If the float at record+172 is not above zero, show "ErrorCannotRecallDead"
//     the same way and stop.
//  4. A "hub" is true when the string at levelName+81 equals one of
//     "kelethin", "gnomehub", "beachhub", "elfhub", "elfhubn", "airhub".
//     In a hub, if the string at player+36 equals "None" show
//     "ErrorNoRecallTarget" (same call shape) and stop.
//  5. Outside a hub, when the record's entity word (record+21872) is set,
//     the float at record+172 is above zero and the count word at record+14168
//     is above zero, the 72-byte entries starting at record+9552 are
//     searched in order. An entry holds a pointer at +8 whose first word is
//     a name pointer; the first name equal to "recallpotion" is used up by
//     decrementing the halfword at entry+12, and when that reaches zero
//     removeItemFromInventory(index, entry number, 2, 0) is called. The
//     count is re-read each step. If nothing was found outside a hub, show
//     "ErrorNoRecallCharges" with the player index as the third argument
//     and stop.
//  6. Otherwise (hub, or a potion was used) recallOn is set to 60 and, for
//     each of the nmPlayersAlive indices in playersAlive, a 96-byte block
//     is allocated; when that succeeds the 12 bytes at (that player's entity
//     word)+16 are copied and passed to the RecallEffect constructor
//     (__12RecallEffectG6Point3). Then the "spel_gatescroll.anm" resource of
//     "fx.lmp" is played at gain 1.0 with a null object; stream 0 is faded
//     when bit 0x40 of its play status is set; gamePause and particlesPaused
//     are cleared and gameControlPaused is set to 60.
// Inferred: that +172 is a health-like float and that the entry layout is
// what the reads imply; the meaning is not established. Unknown: the type of
// record+36 and what the hub strings are compared against beyond the names.
//
// Entity and definition words are 32-bit addresses on the target. They are
// read through address_from_word, which a host check can replace.
extern "C" {
void debugClearStartMenu__Fv();
int gameCheckNoSaveZone__Fv();
unsigned short* textGetString__FPc(const char* key);
void hudSetInfoMessage__FPUsiic(unsigned short* text, int duration, int player, char extra);
int strcmp(const char* a, const char* b);
void removeItemFromInventory__Fiiii(int player, int entry, int a, int b);
void* blockAlloc__Fi(int bytes);
struct norrath_point3 {
    float x, y, z;
};
void __12RecallEffectG6Point3(void* self, norrath_point3* position);
void* lumpFindResource__FPcT0(const char* lump, const char* name);
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float gain);
int soundGetStreamPlayStatus__Fi(int stream);
void soundFadeStream__Fi(int stream);
extern unsigned char player[];
extern char levelName[];
extern int nmPlayersAlive;
extern int playersAlive[];
extern unsigned char recallOn;
extern int gamePause;
extern int gameControlPaused;
extern unsigned char particlesPaused;
}

#ifndef NORRATH_ADDRESS_FROM_WORD
#define NORRATH_ADDRESS_FROM_WORD(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

namespace {
unsigned load_word(const unsigned char* bytes) {
    return static_cast<unsigned>(bytes[0]) | (static_cast<unsigned>(bytes[1]) << 8) |
           (static_cast<unsigned>(bytes[2]) << 16) | (static_cast<unsigned>(bytes[3]) << 24);
}
unsigned short load_half(const unsigned char* bytes) {
    return static_cast<unsigned short>(bytes[0] | (bytes[1] << 8));
}
void store_half(unsigned char* bytes, unsigned short value) {
    bytes[0] = static_cast<unsigned char>(value);
    bytes[1] = static_cast<unsigned char>(value >> 8);
}
float load_float(const unsigned char* bytes) {
    unsigned word = load_word(bytes);
    float value;
    __builtin_memcpy(&value, &word, sizeof value);
    return value;
}
unsigned char* address_from_word(unsigned word) {
    return NORRATH_ADDRESS_FROM_WORD(word);
}
void show_message(const char* key, int player_argument) {
    hudSetInfoMessage__FPUsiic(textGetString__FPc(key), 180, player_argument, 0);
}
}

extern "C" {
void debugRecall__Fi(int index) {
    debugClearStartMenu__Fv();
    if (gameCheckNoSaveZone__Fv()) {
        show_message("ErrorCannotRecall", -1);
        return;
    }
    unsigned char* record = player + index * 23984;
    if (load_float(record + 172) <= 0.0f) {
        show_message("ErrorCannotRecallDead", -1);
        return;
    }
    const char* here = levelName + 81;
    bool hub = strcmp("kelethin", here) == 0 || strcmp("gnomehub", here) == 0 ||
               strcmp("beachhub", here) == 0 || strcmp("elfhub", here) == 0 ||
               strcmp("elfhubn", here) == 0 || strcmp("airhub", here) == 0;
    if (hub && strcmp(reinterpret_cast<const char*>(player + 36), "None") == 0) {
        show_message("ErrorNoRecallTarget", -1);
        return;
    }
    if (!hub) {
        bool found = false;
        if (load_word(record + 21872) != 0 && load_float(record + 172) > 0.0f &&
            static_cast<int>(load_word(record + 14168)) > 0) {
            unsigned char* entry = record + 9552;
            for (int n = 0; n < static_cast<int>(load_word(record + 14168)); ++n, entry += 72) {
                unsigned char* definition = address_from_word(load_word(entry + 8));
                if (strcmp(reinterpret_cast<const char*>(address_from_word(load_word(definition))),
                           "recallpotion") == 0) {
                    found = true;
                    unsigned short left = static_cast<unsigned short>(load_half(entry + 12) - 1);
                    store_half(entry + 12, left);
                    if (left == 0) removeItemFromInventory__Fiiii(index, n, 2, 0);
                    break;
                }
            }
        }
        if (!found) {
            show_message("ErrorNoRecallCharges", index);
            return;
        }
    }
    recallOn = 60;
    for (int i = 0; i < nmPlayersAlive; ++i) {
        void* effect = blockAlloc__Fi(96);
        if (effect) {
            unsigned char* entity = address_from_word(load_word(player + playersAlive[i] * 23984 + 21872));
            norrath_point3 position;
            position.x = load_float(entity + 16);
            position.y = load_float(entity + 20);
            position.z = load_float(entity + 24);
            __12RecallEffectG6Point3(effect, &position);
        }
    }
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("fx.lmp", "spel_gatescroll.anm"), 0,
                                                         1.0f);
    if (soundGetStreamPlayStatus__Fi(0) & 0x40) soundFadeStream__Fi(0);
    gamePause = 0;
    gameControlPaused = 60;
    particlesPaused = 0;
}
}
