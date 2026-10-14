#include <cstring>

extern "C" {
alignas(8) unsigned char player[4 * 23984];
const unsigned char* skills[64];
void hudAssignSpellBookSpell__Fiiil(int index, int entry_index, int mode, int spell);
}

static int feat_calls;
static long long feat_arg;
static int feat_player, feat_entry;
static int rank_calls;
static int rank_player, rank_spell;
static int rank_return = 42;
static int local_flag;
static int plays, lookups;
static void* seq_seen;
static void* object_seen;
static float volume_seen;
static const char *lump_seen, *name_seen;
static char token;
static int order_feat_before_play = -1;

extern "C" {
int playerGetSkillRank__Fii(int p, int s) {
    rank_calls += 1;
    rank_player = p;
    rank_spell = s;
    return rank_return;
}
void givePlayerFeat__Fxii(long long feat, int p, int e) {
    feat_calls += 1;
    feat_arg = feat;
    feat_player = p;
    feat_entry = e;
}
int netPlayerIsLocal__Fi(int) { return local_flag; }
void* lumpFindResource__FPcT0(const char* lump, const char* name) {
    lookups += 1;
    lump_seen = lump;
    name_seen = name;
    return &token;
}
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume) {
    plays += 1;
    seq_seen = sequence;
    object_seen = object;
    volume_seen = volume;
    order_feat_before_play = feat_calls;
}
}

static unsigned char* entry(int pl, int e) { return player + pl * 23984 + 21976 + e * 104; }
static short& code(int pl, int e) { return *reinterpret_cast<short*>(entry(pl, e) + 32); }
static long long& word72(int pl, int e) { return *reinterpret_cast<long long*>(entry(pl, e) + 72); }

static unsigned char skill_a[16], skill_b[16];

static void reset() {
    std::memset(player, 0, sizeof player);
    std::memset(skills, 0, sizeof skills);
    skill_a[8] = 0x31;
    skill_b[8] = 0x32;
    skills[5] = skill_a;
    skills[9] = skill_b;
    feat_calls = rank_calls = plays = lookups = 0;
    local_flag = 0;
    order_feat_before_play = -1;
}

static bool is_text(int pl, int e) { return std::strcmp(reinterpret_cast<const char*>(entry(pl, e)), "newstyle") == 0; }

int main() {
    // Mode 1, nothing holds the spell: only step 3 runs.
    reset();
    hudAssignSpellBookSpell__Fiiil(2, 3, 1, 5);
    if (feat_calls != 0 || rank_calls != 1 || rank_player != 2 || rank_spell != 5) return 1;
    if (!is_text(2, 3) || code(2, 3) != 5) return 2;
    if (entry(2, 3)[37] != 0x31 || entry(2, 3)[36] != 42) return 3;
    if (plays != 0 || lookups != 0) return 4;
    for (int e = 0; e < 8; ++e) if (e != 3 && code(2, e) != 0) return 5;

    // Mode 1, another entry already holds the spell: it takes entry e's old code.
    reset();
    code(1, 3) = 9;                  // old code of the selected entry
    std::memset(entry(1, 3), 0, 9);
    std::memcpy(entry(1, 3), "newstyle", 9);
    code(1, 6) = 5;                  // entry 6 holds spell 5
    word72(1, 6) = 77;
    hudAssignSpellBookSpell__Fiiil(1, 3, 1, 5);
    if (word72(1, 6) != -1 || code(1, 6) != 9) return 6;
    if (!is_text(1, 6)) return 7;
    if (entry(1, 6)[37] != 0x32 || entry(1, 6)[36] != 42) return 8;
    if (rank_calls != 2 || rank_spell != 5) return 9;      // once for the target, once for the selection
    if (code(1, 3) != 5 || entry(1, 3)[37] != 0x31) return 10;
    if (feat_calls != 0) return 11;

    // Same, but the selected entry's text lacks "newstyle": a feat is granted.
    reset();
    code(0, 2) = 9;
    code(0, 7) = 5;
    hudAssignSpellBookSpell__Fiiil(0, 2, 1, 5);
    if (feat_calls != 1 || feat_arg != 0x20000 || feat_player != 0 || feat_entry != 7) return 12;
    if (word72(0, 7) != -1 || code(0, 7) != 9) return 13;
    if (code(0, 2) != 5) return 14;

    // Selected code of -1: the found entry is marked, nothing else happens in step 2.
    reset();
    code(0, 1) = -1;
    code(0, 4) = 5;
    hudAssignSpellBookSpell__Fiiil(0, 1, 1, 5);
    if (word72(0, 4) != -1 || code(0, 4) != -1) return 15;
    if (feat_calls != 0) return 16;

    // Last match wins.
    reset();
    code(3, 0) = 5;
    code(3, 7) = 5;
    code(3, 4) = 6;
    hudAssignSpellBookSpell__Fiiil(3, 4, 1, 5);
    if (word72(3, 7) != -1 || word72(3, 0) != 0) return 17;
    if (feat_calls != 1 || feat_entry != 7) return 18;       // entry 4's text is empty
    if (code(3, 0) != 0 || code(3, 7) != 6 || code(3, 4) != 5) return 19;   // entry 0 cleared; 7 took the old code

    // Other mode: match on the 64-bit word, then grant the spell as a feat.
    reset();
    word72(2, 1) = 0x1234;
    word72(2, 5) = 0x1234;
    code(2, 6) = 3;
    hudAssignSpellBookSpell__Fiiil(2, 6, 0, 0x1234);
    if (word72(2, 5) != -1 || code(2, 5) != 3) return 20;
    if (word72(2, 1) != 0x1234) return 21;
    if (feat_calls != 2) return 22;
    if (feat_arg != 0x1234 || feat_player != 2 || feat_entry != 6) return 23;
    // Other mode with a negative spell is sign-extended.
    reset();
    word72(0, 0) = -2;
    hudAssignSpellBookSpell__Fiiil(0, 0, 4, -2);
    if (word72(0, 0) != -1) return 24;
    if (feat_calls != 2 || feat_arg != -2) return 25;

    // Local player: the sequence plays after the changes.
    reset();
    local_flag = 1;
    hudAssignSpellBookSpell__Fiiil(1, 0, 1, 9);
    if (lookups != 1 || plays != 1 || seq_seen != &token || object_seen != 0 || volume_seen != 1.0f) return 26;
    if (std::strcmp(lump_seen, "fx.lmp") != 0 || std::strcmp(name_seen, "menu_spell_assign.anm") != 0) return 27;
    if (entry(1, 0)[37] != 0x32) return 28;
    return 0;
}
