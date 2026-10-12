extern "C" {
unsigned char player[4 * 23984];
void hudAssignQuickSpell__Fiii(int index, int slot, int spell);
}

static int local_flag;
static int local_query;
static int lookups, plays;
static const char *lump_seen, *name_seen;
static void* seq_seen;
static void* obj_seen;
static float vol_seen;
static char token;

extern "C" {
int netPlayerIsLocal__Fi(int index) {
    local_query = index;
    return local_flag;
}
void* lumpFindResource__FPcT0(const char* lump, const char* name) {
    lookups += 1;
    lump_seen = lump;
    name_seen = name;
    return &token;
}
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume) {
    plays += 1;
    seq_seen = sequence;
    obj_seen = object;
    vol_seen = volume;
}
}

static bool streq(const char* a, const char* b) {
    while (*a && *a == *b) { ++a; ++b; }
    return *a == *b;
}

static signed char& q(int index, int which) {
    return reinterpret_cast<signed char&>(player[index * 23984 + 23580 + which]);
}

static void setq(int index, int a, int b) {
    q(index, 0) = static_cast<signed char>(a);
    q(index, 1) = static_cast<signed char>(b);
}

int main() {
    local_flag = 0;
    setq(1, 3, 4);
    hudAssignQuickSpell__Fiii(1, 0, 9);          // slot 0, new spell
    if (lookups != 0 || plays != 0 || local_query != 1) return 1;
    if (q(1, 0) != 9 || q(1, 1) != 4) return 2;
    setq(1, 3, 4);
    hudAssignQuickSpell__Fiii(1, 0, 4);          // slot 0, same as other: swap
    if (q(1, 0) != 4 || q(1, 1) != 3) return 3;
    setq(1, 3, 4);
    hudAssignQuickSpell__Fiii(1, 5, 9);          // other slot, new spell
    if (q(1, 0) != 3 || q(1, 1) != 9) return 4;
    setq(1, 3, 4);
    hudAssignQuickSpell__Fiii(1, 1, 3);          // other slot, same as first: swap
    if (q(1, 0) != 4 || q(1, 1) != 3) return 5;
    setq(1, 3, 4);
    hudAssignQuickSpell__Fiii(1, 0, 3);          // slot 0, same as own value
    if (q(1, 0) != 3 || q(1, 1) != 4) return 6;
    setq(2, -1, -1);
    hudAssignQuickSpell__Fiii(2, 0, -1);
    if (q(2, 0) != -1 || q(2, 1) != -1) return 7;
    setq(2, 5, 6);
    hudAssignQuickSpell__Fiii(2, 0, 0x105);      // wider than a byte: never equal
    if (q(2, 0) != 5 || q(2, 1) != 6) return 8;
    local_flag = 1;
    setq(0, 1, 2);
    hudAssignQuickSpell__Fiii(0, 0, 8);
    if (lookups != 1 || plays != 1) return 9;
    if (!streq(lump_seen, "fx.lmp") || !streq(name_seen, "menu_spell_assign.anm")) return 10;
    if (seq_seen != &token || obj_seen != 0 || vol_seen != 1.0f) return 11;
    if (q(0, 0) != 8 || q(0, 1) != 2) return 12;
    return 0;
}
