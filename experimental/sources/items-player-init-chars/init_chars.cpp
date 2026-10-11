// Observed initChars__FiPPc at 0x2cfe98, 824 bytes. When netNetworkMode is 0 it
// stores 2 in nmPlayers. It always stores 1 in gameDifficulty. A --char argument
// selects a raceNames.1194 entry and writes playerRace.1193[0] to twice that
// index, plus one when --female is also present. An unmatched name calls exit.
// Each player then receives playerInit with the race word, one of the four
// observed names, and the byte at race * 4 in the 40-byte table.
extern "C" unsigned char netNetworkMode;
extern "C" int nmPlayers;
extern "C" int gameDifficulty = 1;
extern "C" int player_race[4] __asm__("playerRace.1193") = {1, 2, 2, 7};

static const char name_barbarian[] = "barbarian";
static const char name_wood_elf[] = "wood_elf";
static const char name_high_elf[] = "high_elf";
static const char name_erudite[] = "erudite";
static const char name_dark_elf[] = "dark_elf";
static const char name_toonces[] = "Toonces";
static const char name_edna[] = "Edna";
static const char name_otis[] = "Otis";
static const char name_gaylord[] = "Gaylord";
static const char flag_char[] = "--char";
static const char flag_female[] = "--female";

extern "C" const char* race_names[6] __asm__("raceNames.1194") = {
    name_barbarian, name_wood_elf, name_high_elf, name_erudite, name_dark_elf, 0};

// Forty observed bytes at 0x47ae78. The last word is the image address
// 0x00486788; this function reads only the first byte of the selected word.
static const unsigned char race_bytes[40] = {
    0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x88, 0x67, 0x48, 0x00};

static const char* const default_names[4] = {name_toonces, name_edna, name_otis, name_gaylord};

extern "C" int strcmp(const char* left, const char* right);
extern "C" void exit(int code);
extern "C" void playerInit__FiiPcScScScScT2(int index, int race, char* name, signed char field,
                                            signed char one, signed char minus_one, signed char zero,
                                            char* trailing);

static char* argument_after(int argc, char** argv, const char* flag) {
    if (!(1 < argc)) return 0;
    for (int index = 1; index < argc; ++index) {
        if (strcmp(flag, argv[index]) == 0) {
            if (index < argc - 1) return argv[index + 1];
            return 0;
        }
    }
    return 0;
}

static char* argument_present(int argc, char** argv, const char* flag) {
    if (!(1 < argc)) return 0;
    for (int index = 1; index < argc; ++index) {
        if (strcmp(flag, argv[index]) == 0) return argv[index];
    }
    return 0;
}

static void resolve_race(int argc, char** argv, char* selected) {
    if (race_names[0] == 0) exit(-1);
    int index = 0;
    const char** cursor = race_names;
    while (*cursor != 0) {
        if (strcmp(selected, *cursor) == 0) {
            player_race[0] = index * 2;
            if (argument_present(argc, argv, flag_female) != 0) player_race[0] += 1;
            return;
        }
        ++cursor;
        ++index;
    }
    const char** walk = race_names;
    while (*walk != 0) ++walk;
    exit(-1);
}

extern "C" void initChars__FiPPc(int argc, char** argv) {
    if (netNetworkMode == 0) nmPlayers = 2;
    gameDifficulty = 1;
    char* selected = argument_after(argc, argv, flag_char);
    if (selected != 0) resolve_race(argc, argv, argument_after(argc, argv, flag_char));
    if (nmPlayers > 0) {
        for (int index = 0; index < nmPlayers; ++index) {
            int race = player_race[index];
            signed char field = static_cast<signed char>(race_bytes[race * 4]);
            playerInit__FiiPcScScScScT2(index, race, const_cast<char*>(default_names[index]), field, 1, -1, 0, 0);
        }
    }
}
