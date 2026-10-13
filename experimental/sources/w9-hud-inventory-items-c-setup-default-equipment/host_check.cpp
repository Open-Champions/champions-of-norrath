// Host check for setupDefaultEquipment__Fi (includes the source; build the file alone with -fno-builtin).
// No standard headers: the check supplies strstr and sprintf itself and keeps a text log of the calls.
#define NORRATH_INVENTORY_HOST 1
#include <initializer_list>
#include "setup_default_equipment.cpp"
alignas(8) unsigned char player[2 * 23984];
unsigned char levelName[400];
alignas(8) unsigned char pad[2 * 88];
int cheatMode;
signed char MC_Debug;
signed char armorDebug[4];
unsigned eRandState;
alignas(8) unsigned char WarriorSkillTree[56 * 4], ClericSkillTree[56 * 4], ShadowKnightSkillTree[56 * 4], WizardSkillTree[56 * 4];
const void* inventory_items::host_resolve(unsigned) { return 0; }

static char logbuf[1 << 16]; static unsigned used;
static void put(const char* s) { while (*s) logbuf[used++] = *s++; logbuf[used] = 0; }
static void put_int(long long v) { char t[24]; int n = 0; bool neg = v < 0; unsigned long long u = neg ? -v : v; do { t[n++] = char('0' + u % 10); u /= 10; } while (u); if (neg) logbuf[used++] = '-'; while (n) logbuf[used++] = t[--n]; logbuf[used] = 0; }
static bool same(const char* a, const char* b) { while (*a && *a == *b) { ++a; ++b; } return *a == *b; }
static bool starts(const char* a, const char* b) { while (*b && *a == *b) { ++a; ++b; } return *b == 0; }
static int count_of(const char* word) { int n = 0; for (const char* p = logbuf; *p; ++p) if (starts(p, word)) ++n; return n; }
static const char* find_text(const char* word) { for (const char* p = logbuf; *p; ++p) if (starts(p, word)) return p; return 0; }

extern "C" {
char* strstr(const char* t, const char* n) { for (; *t; ++t) if (starts(t, n)) return const_cast<char*>(t); return *n ? 0 : const_cast<char*>(t); }
int sprintf(char* b, const char* f, ...) { // only "name%d" with one int, taken from the stack frame of the caller via va_arg builtin
    __builtin_va_list ap; __builtin_va_start(ap, f); int v = __builtin_va_arg(ap, int); __builtin_va_end(ap);
    char* o = b; for (; *f; ++f) { if (f[0] == '%' && f[1] == 'd') { char t[12]; int n = 0; unsigned u = v; do { t[n++] = char('0' + u % 10); u /= 10; } while (u); while (n) *o++ = t[--n]; ++f; } else *o++ = *f; } *o = 0; return int(o - b); }
void givePlayerFeat__Fxii(long long f, int p, int v) { put("feat("); put_int(f); put(","); put_int(p); put(","); put_int(v); put(")\n"); }
void playerSetFeatRank__Fixi(int p, long long f, int r) { put("rank("); put_int(p); put(","); put_int(f); put(","); put_int(r); put(")\n"); }
static int next_slot;
void create__4itemPcPQ24item11descriptionii(char* n, void* d, int a, int b) { put("create("); put(n); put(","); put_int(a); put(","); put_int(b); put(")\n"); (void)d; }
int addItemToInventory__FiPQ24item11descriptioni(int p, void* d, int a) { put("add("); put_int(p); put(","); put_int(a); put(",m"); put_int(static_cast<unsigned char*>(d)[32]); put(")=" ); put_int(next_slot); put("\n"); return next_slot++; }
void equipItem__FiPQ24item11descriptioni(int p, void* s, int m) { put("equip("); put_int(p); put(","); put_int(static_cast<unsigned char*>(s) - (player + p * 23984)); put(","); put_int(m); put(")\n"); }
void givePlayerSkill__Fii(int s, int p) { put("skill("); put_int(s); put(","); put_int(p); put(")\n"); }
void playerSetSkillRank__Fiii(int p, int s, int r) { put("skillrank("); put_int(p); put(","); put_int(s); put(","); put_int(r); put(")\n"); }
void playerRecomputeWeight__Fi(int p) { put("weight("); put_int(p); put(")\n"); }
}
static unsigned& rw(unsigned char* b, unsigned o) { return *reinterpret_cast<unsigned*>(b + o); }
static void reset(int owner, int kind) {
    for (unsigned char& b : player) b = 0; for (unsigned char& b : pad) b = 0; for (unsigned char& b : levelName) b = 0;
    for (signed char& b : armorDebug) b = 0; used = 0; logbuf[0] = 0; next_slot = 10; MC_Debug = 0; cheatMode = 0; eRandState = 1;
    for (unsigned char* t : {WarriorSkillTree, ClericSkillTree, ShadowKnightSkillTree, WizardSkillTree}) { for (int i = 0; i < 56 * 4; ++i) t[i] = 0; t[0] = 'E'; t[1] = 'N'; t[2] = 'D'; }
    rw(player + 23984 * owner, 80) = static_cast<unsigned>(kind);
    rw(player + 23984 * owner, 84) = 0;
}
static const char* skills(int kind) { switch (kind) { case 1: return "skill(60,1)\nskillrank(1,60,1)\nskill(16,1)\nskillrank(1,16,1)\nweight(1)\n"; case 4: return "skill(60,1)\nskillrank(1,60,1)\nskill(23,1)\nskillrank(1,23,1)\nweight(1)\n"; case 5: return "weight(1)\n";
    default: return "skill(60,1)\nskillrank(1,60,1)\nskill(61,1)\nskillrank(1,61,1)\nweight(1)\n"; } }
static bool tail_is(const char* t) { const char* p = logbuf; unsigned n = 0; while (t[n]) ++n; if (used < n) return false; return same(p + used - n, t); }
int main() {
    // starting kit, class 0 (leather armor equipped from list 1, one weapon equipped from list 0)
    reset(1, 0);
    setupDefaultEquipment__Fi(1);
    if (!starts(logbuf, "create(leatherarmor1+armorbonus-55+requiredlevel1,0,0)\nadd(1,1,m2)=10\nequip(1,5664,1)\n")) return 1;      // 4944 + 72 * 10
    if (count_of("equip(") != 5) return 2;
    if (!find_text("create(axe+damageincrease-25+requiredlevel1,0,0)\nadd(1,1,m7)=14\nequip(1,1336,1)\n") ) return 3;
    if (!find_text("create(recallpotion,-1,0)\nadd(1,0,m7)=15\nadd(1,0,m7)=16\ncreate(leshealingpotion,-1,0)\nadd(1,1,m7)=17\nadd(1,1,m7)=18\ncreate(lesrejuvpotion,-1,0)\nadd(1,1,m7)=19\nadd(1,1,m7)=20\n")) return 4;
    if (!tail_is(skills(0))) return 5;
    // class 4 gets cloth with mark 7 and a quarterstaff; class 3 two shorter weapons and two arrows
    reset(1, 4); setupDefaultEquipment__Fi(1);
    if (!starts(logbuf, "create(cloth1armor1+armorbonus-25+requiredlevel1,0,0)\nadd(1,1,m7)") || !find_text("create(quarterstaff+damageincrease-35+requiredlevel1,0,0)") || !tail_is(skills(4))) return 6;
    reset(1, 3); setupDefaultEquipment__Fi(1);
    if (!find_text("create(shortsword+damageincrease-45+requiredlevel1,1,0)") || !find_text("create(shortbow+damageincrease-10+requiredlevel1,0,0)") || count_of("create(arrows,0,0") != 2 || !tail_is(skills(3))) return 7;
    reset(1, 2); setupDefaultEquipment__Fi(1);
    if (!find_text("create(shortsword+damageincrease-35+requiredlevel1,0,0)") || !tail_is(skills(2))) return 8;
    reset(1, 1); setupDefaultEquipment__Fi(1);
    if (!find_text("create(mace+damageincrease-25+requiredlevel1,0,0)") || !tail_is(skills(1))) return 9;
    reset(1, 5); setupDefaultEquipment__Fi(1);                       // unknown class: armor and potions but no weapon and no skills
    if (count_of("equip(") != 4 || find_text("damageincrease") == 0 ? false : count_of("damageincrease") != 0) return 10;
    if (!tail_is(skills(5))) return 11;
    // skirmish levels get no kit at all
    reset(1, 0); for (const char* t = "xskirmx"; ; ++t) { levelName[81 + (t - "xskirmx")] = *t; if (!*t) break; }
    setupDefaultEquipment__Fi(1);
    if (count_of("create(") != 0 || !tail_is(skills(0)) || cheatMode != 0) return 12;
    // debug pad flag: sets cheatMode, walks the class feat table up to END
    reset(1, 1); rw(pad + 88, 8) = 2;
    { unsigned char* t = ClericSkillTree; for (int i = 0; i < 56 * 4; ++i) t[i] = 0; const char* names[3] = {"aaa", "bbb", "END"}; for (int i = 0; i < 3; ++i) { for (int k = 0; names[i][k]; ++k) t[56 * i + k] = names[i][k]; t[56 * i + 3] = 0; *reinterpret_cast<long long*>(t + 56 * i + 40) = 100 + i; } }
    setupDefaultEquipment__Fi(1);
    if (cheatMode != 1 || !starts(logbuf, "feat(100,1,-1)\nrank(1,100,1)\nfeat(101,1,-1)\nrank(1,101,1)\ncreate(leatherarmor1")) return 13;      // MC_Debug is zero: the normal kit follows
    reset(1, 5); rw(pad + 88, 8) = 2; setupDefaultEquipment__Fi(1);       // class 5 has no feat table
    if (count_of("feat(") != 0) return 14;
    // everything list: MC_Debug set, armorDebug[0] set, class 0
    reset(1, 0); rw(pad + 88, 8) = 2; MC_Debug = 1; armorDebug[0] = 1;
    setupDefaultEquipment__Fi(1);
    if (!starts(logbuf, "create(lightrod,0,0)\nadd(1,1,m255)=10\ncreate(scepter,0,0)")) return 15;
    if (count_of("create(gaspotion,-1,0") != 10 || count_of("create(firepotion") != 10 || count_of("create(explodingpotion") != 10 || count_of("create(whetstone") != 10) return 16;
    if (count_of("create(ring+resistfire10,0") != 1 || count_of("create(ring+resistfire10,5") != 1 || count_of("create(ring+resistfire10") != 6 ||
        count_of("create(earring") != 8 || count_of("create(choker") != 4 || count_of("create(amulet") != 3) return 17;
    if (count_of("create(spear,0,0)\nadd(1,1,m3)") != 2 || count_of("create(mace,0,0)\nadd(1,1,m3)") != 1) return 18;
    if (count_of("create(leatherarmor1,-1,0)\nadd(1,1,m0)") != 1 || count_of("create(leatherarmor4,-1,0)\nadd(1,1,m56)") != 1 || count_of("create(leatherboots4,-1,0)") != 5) return 19;
    if (count_of("create(leatherboots3,-1,0)\nadd(1,1,m8)") != 1 || count_of("create(leathergloves2,-1,0)\nadd(1,1,m1)") != 1 || count_of("create(leatherhelmet2,-1,0)\nadd(1,1,m2)") != 1) return 20;
    if (count_of("create(chain") != 0 || count_of("create(plate") != 0 || count_of("create(cloth") != 0 || count_of("create(shield") != 0) return 21;
    if (count_of("equip(") != 6) return 22;                       // one weapon equip (record+84 is not 5) and the five level-1 piece-0 armor pieces
    if (!tail_is(skills(0))) return 23;
    // armorDebug[3] chooses the shield list and no armor sets
    reset(1, 0); rw(pad + 88, 8) = 2; MC_Debug = 1; armorDebug[3] = 1; armorDebug[1] = 1;
    setupDefaultEquipment__Fi(1);
    if (count_of("create(shield1,0,0") != 1 || count_of("create(rshield3,2,0") != 1 || count_of("create(leather") != 0 || count_of("create(chain") != 0) return 24;
    // with no armor set chosen the LCG picks one: eRandState = 1 * 1664525 + 1013904223, then (state >> 16) % 3
    reset(1, 0); rw(pad + 88, 8) = 2; MC_Debug = 1;
    setupDefaultEquipment__Fi(1);
    if (eRandState != 1664525u + 1013904223u) return 25;
    { const unsigned pick = (eRandState >> 16) % 3u; if (armorDebug[pick] != 1 || armorDebug[0] + armorDebug[1] + armorDebug[2] != 1) return 26; }
    // class 4 uses the cloth sets
    reset(1, 4); rw(pad + 88, 8) = 2; MC_Debug = 1; armorDebug[2] = 1;
    setupDefaultEquipment__Fi(1);
    if (count_of("create(cloth3armor1,-1,0)\nadd(1,1,m0)") != 1 || count_of("create(cloth3boots4,-1,0)\nadd(1,1,m56)") != 1 || count_of("create(cloth3boots3,-1,0)\nadd(1,1,m8)") != 1 || count_of("create(plate") != 0) return 27;
    // the word at record+84 decides which weapon is equipped: not 5 -> the broadsword, 5 -> the mace
    for (int five = 0; five < 2; ++five) {
        reset(1, 0); rw(pad + 88, 8) = 2; MC_Debug = 1; armorDebug[0] = 1; rw(player + 23984, 84) = five ? 5 : 0;
        setupDefaultEquipment__Fi(1);
        const char* broadsword = find_text("create(broadsword+stunresistance50,1,0)\nadd(1,1,m255)=");
        const char* mace = find_text("create(mace,0,0)\nadd(1,1,m3)=");
        if (!broadsword || !mace) return 28;
        const char* after_broadsword = broadsword; while (*after_broadsword != '\n') ++after_broadsword; ++after_broadsword;
        while (*after_broadsword != '\n') ++after_broadsword; ++after_broadsword;
        const char* after_mace = mace; while (*after_mace != '\n') ++after_mace; ++after_mace;
        while (*after_mace != '\n') ++after_mace; ++after_mace;
        if (starts(after_broadsword, "equip(") == (five != 0)) return 29;
        if (starts(after_mace, "equip(") != (five != 0)) return 30;
        if (count_of("equip(") != 6) return 31;
    }
    return 0;
}
