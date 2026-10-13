#include <cstring>
extern "C" {
int syncRandState;
int gameDifficulty;
void* makeRandomPotion__4itemPQ24item11description(void*);
}

static char gotName[40];
static void* gotDesc;
static int gotA, gotB, calls, stateAtCall;
extern "C" void* create__4itemPcPQ24item11descriptionii(char* n, void* d, int a, int b) {
    std::strcpy(gotName, n); gotDesc = d; gotA = a; gotB = b; ++calls; stateAtCall = syncRandState;
    return (void*)0x4242;
}

int main() {
    int desc;
    // first draw <= 0x8c9f -> mana table, two steps, index 11 - 3*d
    const char* mana[] = {"lesrejuvpotion", "smallrejuvpotion", "minorrejuvpotion"};
    for (int d = 1; d <= 3; ++d) {
        syncRandState = 0; gameDifficulty = d; calls = 0;
        void* r = makeRandomPotion__4itemPQ24item11description(&desc);
        if (r != (void*)0x4242 || calls != 1 || gotDesc != &desc || gotA != -1 || gotB != 0) return 1;
        // d = 1 -> index 8, d = 2 -> index 5, d = 3 -> index 2
        static const char* expect[] = {"", "lesrejuvpotion", "rejuvpotion", "exrejuvpotion"};
        if (std::strcmp(gotName, expect[d])) return 2 + d;
        if ((unsigned)syncRandState != 1196435762u || (unsigned)stateAtCall != 1196435762u) return 6;
    }
    (void)mana;
    // 0x8c9f < first draw <= 0xea5f -> health table
    static const char* health[] = {"", "leshealingpotion", "healingpotion", "exhealingpotion"};
    for (int d = 1; d <= 3; ++d) {
        syncRandState = 809; gameDifficulty = d;
        makeRandomPotion__4itemPQ24item11description(&desc);
        if (std::strcmp(gotName, health[d])) return 10 + d;
        if ((unsigned)syncRandState != 2825714499u) return 14;
    }
    // first draw > 0xea5f -> recallpotion; one step; difficulty irrelevant
    for (int d = 0; d <= 3; ++d) {
        syncRandState = 1754; gameDifficulty = d; calls = 0;
        void* r = makeRandomPotion__4itemPQ24item11description(&desc);
        if (r != (void*)0x4242 || std::strcmp(gotName, "recallpotion") || calls != 1) return 20;
        if ((unsigned)syncRandState != 3933481073u || (unsigned)stateAtCall != 3933481073u) return 21;
        if (gotDesc != &desc || gotA != -1 || gotB != 0) return 22;
    }
    return 0;
}
