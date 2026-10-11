#include <csetjmp>
#include <cstring>
extern "C" unsigned char netNetworkMode = 0;
extern "C" int nmPlayers = 1;
extern "C" int gameDifficulty;
extern "C" int player_race[4] __asm__("playerRace.1193");
extern "C" const char* race_names[6] __asm__("raceNames.1194");
extern "C" void initChars__FiPPc(int argc, char** argv);

static jmp_buf exit_jump;
static int exit_code;
static int exited;
static int char_scans;
struct Call {
    int index;
    int race;
    int field;
    int one;
    int minus_one;
    int zero;
    const char* name;
    const char* trailing;
};
static Call calls[8];
static int ncalls;

extern "C" void exit(int code) {
    exit_code = code;
    exited = 1;
    longjmp(exit_jump, 1);
}
extern "C" int strcmp(const char* left, const char* right) {
    const char* flag = "--char";
    bool same = true;
    for (int i = 0; flag[i] != 0 || left[i] != 0; ++i) {
        if (left[i] != flag[i]) {
            same = false;
            break;
        }
        if (flag[i] == 0) break;
    }
    if (same) ++char_scans;
    for (int i = 0;; ++i) {
        unsigned char l = static_cast<unsigned char>(left[i]);
        unsigned char r = static_cast<unsigned char>(right[i]);
        if (l != r) return l < r ? -1 : 1;
        if (l == 0) return 0;
    }
}
extern "C" void playerInit__FiiPcScScScScT2(int index, int race, char* name, signed char field, signed char one,
                                            signed char minus_one, signed char zero, char* trailing) {
    calls[ncalls].index = index;
    calls[ncalls].race = race;
    calls[ncalls].field = field;
    calls[ncalls].one = one;
    calls[ncalls].minus_one = minus_one;
    calls[ncalls].zero = zero;
    calls[ncalls].name = name;
    calls[ncalls].trailing = trailing;
    ++ncalls;
}

static void reset(unsigned char mode, int players) {
    netNetworkMode = mode;
    nmPlayers = players;
    gameDifficulty = 9;
    player_race[0] = 1;
    player_race[1] = 2;
    player_race[2] = 2;
    player_race[3] = 7;
    ncalls = 0;
    char_scans = 0;
    exited = 0;
    exit_code = 0;
}
static int run(int argc, char** argv) {
    if (setjmp(exit_jump) == 0) initChars__FiPPc(argc, argv);
    return 0;
}
static int expect_call(int n, int index, int race, const char* name, int field) {
    if (ncalls <= n) return 1;
    const Call& call = calls[n];
    if (call.index != index || call.race != race || call.field != field) return 1;
    if (std::strcmp(call.name, name) != 0) return 1;
    if (call.one != 1 || call.minus_one != -1 || call.zero != 0 || call.trailing != 0) return 1;
    return 0;
}

int main() {
    char* only[] = {const_cast<char*>("prog")};
    reset(0, 9);
    run(1, only);
    if (exited || nmPlayers != 2 || gameDifficulty != 1 || char_scans != 0) return 1;
    if (expect_call(0, 0, 1, "Toonces", 5) || expect_call(1, 1, 2, "Edna", 0) || ncalls != 2) return 2;

    reset(1, 1);
    run(1, only);
    if (exited || nmPlayers != 1 || ncalls != 1) return 3;
    if (expect_call(0, 0, 1, "Toonces", 5)) return 4;

    char* male[] = {const_cast<char*>("prog"), const_cast<char*>("foo"), const_cast<char*>("--char"),
                    const_cast<char*>("wood_elf")};
    reset(0, 1);
    run(4, male);
    if (exited || char_scans != 4 || player_race[0] != 2) return 5;
    if (expect_call(0, 0, 2, "Toonces", 0) || expect_call(1, 1, 2, "Edna", 0) || ncalls != 2) return 6;

    char* female[] = {const_cast<char*>("prog"), const_cast<char*>("--char"), const_cast<char*>("barbarian"),
                      const_cast<char*>("--female")};
    reset(0, 1);
    run(4, female);
    if (exited || player_race[0] != 1 || player_race[1] != 2) return 7;
    if (expect_call(0, 0, 1, "Toonces", 5)) return 8;

    char* dark[] = {const_cast<char*>("prog"), const_cast<char*>("--female"), const_cast<char*>("--char"),
                    const_cast<char*>("dark_elf")};
    reset(1, 1);
    run(4, dark);
    if (exited || nmPlayers != 1 || player_race[0] != 9) return 9;
    if (expect_call(0, 0, 9, "Toonces", static_cast<signed char>(0x88))) return 10;

    char* erudite[] = {const_cast<char*>("prog"), const_cast<char*>("--char"), const_cast<char*>("erudite"),
                       const_cast<char*>("--female")};
    reset(1, 4);
    run(4, erudite);
    if (exited || player_race[0] != 7) return 11;
    if (expect_call(0, 0, 7, "Toonces", -1) || expect_call(3, 3, 7, "Gaylord", -1) || ncalls != 4) return 12;

    char* missing[] = {const_cast<char*>("prog"), const_cast<char*>("--char"), const_cast<char*>("unknown")};
    reset(0, 1);
    run(3, missing);
    if (!exited || exit_code != -1 || ncalls != 0 || player_race[0] != 1) return 13;

    char* bare[] = {const_cast<char*>("prog"), const_cast<char*>("--char")};
    reset(0, 1);
    run(2, bare);
    if (exited || player_race[0] != 1 || ncalls != 2) return 14;

    char* only_female[] = {const_cast<char*>("prog"), const_cast<char*>("--female")};
    reset(0, 1);
    run(2, only_female);
    if (exited || player_race[0] != 1 || expect_call(0, 0, 1, "Toonces", 5)) return 15;

    const char* saved = race_names[0];
    race_names[0] = 0;
    char* named[] = {const_cast<char*>("prog"), const_cast<char*>("--char"), const_cast<char*>("barbarian")};
    reset(1, 3);
    run(3, named);
    race_names[0] = saved;
    if (!exited || exit_code != -1 || ncalls != 0 || nmPlayers != 3) return 16;
    return 0;
}
