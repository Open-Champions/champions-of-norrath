// Observed makeRandomPotion__4itemPQ24item11description at 0x00107ce8, 632 bytes.
// The argument is passed unchanged as the second parameter of
// create__4itemPcPQ24item11descriptionii(name, argument, -1, 0), whose result is
// returned. The name is chosen with the word syncRandState (0x00483e5c), stepped
// as state = state * 1664525 + 1013904223 (32-bit wrap); a draw is the high 16
// bits of the new state. Let r1 be the first draw:
//   r1 <= 0x8c9f: a second draw r2 is taken; x = r2 * 2^-16 * 0.2 + 0.16 (single
//     precision, constants as read from the image). The state keeps the second
//     step. The name comes from the 9-entry table `manaPotions` (0x00332eb8).
//   0x8c9f < r1 <= 0xea5f: the same with the table `healthPotions` (0x00332ee0).
//   r1 > 0xea5f: the name is "recallpotion" and the state keeps the first step.
// In the first two cases the table index is 11 - 3*d when x <= 1.0, 10 - 3*d when
// 1.0 < x <= 2.0, and 9 - 3*d otherwise, where d is the word gameDifficulty
// (0x00484cd8); the index is not range checked (d = 0 would read past the table;
// d in 1..3 stays inside it). With the constants above x is always below 0.36,
// so only the first index is reachable; the other two are kept as read.
// The state is stored before create is called. The strings are the nine entries
// of each table as stored in the image (the table itself is local data there).
// The meaning of the names (mana versus health, the tier order) is inferred from
// the strings only. The callee and the two globals are not defined here.
extern "C" {
extern int syncRandState;
extern int gameDifficulty;
void* create__4itemPcPQ24item11descriptionii(char* name, void* description, int a, int b);
}

static const char* manaPotions[9] = {
    "fullrejuvpotion", "giantrejuvpotion", "exrejuvpotion", "greatrejuvpotion", "largerejuvpotion",
    "rejuvpotion", "minorrejuvpotion", "smallrejuvpotion", "lesrejuvpotion",
};
static const char* healthPotions[9] = {
    "fullhealpotion", "gianthealpotion", "exhealingpotion", "greathealpotion", "largehealpotion",
    "healingpotion", "minorhealpotion", "smallhealpotion", "leshealingpotion",
};

static inline unsigned nextState(unsigned state) { return state * 0x19660du + 0x3c6ef35fu; }

extern "C" void* makeRandomPotion__4itemPQ24item11description(void* description) {
    unsigned first = nextState(static_cast<unsigned>(syncRandState));
    unsigned firstDraw = first >> 16;
    if (firstDraw > 0xea5fu) {
        syncRandState = static_cast<int>(first);
        return create__4itemPcPQ24item11descriptionii(const_cast<char*>("recallpotion"), description, -1, 0);
    }
    const char** table = firstDraw > 0x8c9fu ? healthPotions : manaPotions;
    unsigned second = nextState(first);
    float x = static_cast<float>(second >> 16) * (1.0f / 65536.0f) * 0.2f + 0.16f;
    syncRandState = static_cast<int>(second);
    int d = gameDifficulty;
    int index;
    if (x <= 1.0f) index = 11 - 3 * d;
    else if (x <= 2.0f) index = 10 - 3 * d;
    else index = 9 - 3 * d;
    return create__4itemPcPQ24item11descriptionii(const_cast<char*>(table[index]), description, -1, 0);
}
