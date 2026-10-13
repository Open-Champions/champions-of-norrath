// Host contract for drinkPotion__6Playeriiii. Not an EE plan input.
#define NORRATH_HOST_CONTRACT 1
#include <cstring>
#include <string>
static unsigned char arena[1 << 22];
unsigned char* norrath_host_base = arena;
#include "drink_potion.cpp"
static std::string trace;
extern "C" {
unsigned char player[23984 * 2 + 64];
signed char interfaceOn = 0;
int invGrid = 0;
int fadeTimer[14];
int fadeIconY[14];
int gameState = 0, local = 0, potionsLeft = 0;
int scriptGetGameState__FPc(const char* k) { trace += std::string("get:") + k + ";"; return gameState; }
void scriptSetGameState__FPci(const char* k, int v) { trace += std::string("set:") + k + "=" + std::to_string(v) + ";"; gameState = v; }
unsigned short* textGetString__FPc(const char* k) { trace += std::string("text:") + k + ";"; return 0; }
void multiHudAddHelpMessage__FiPUsT1i(int p, unsigned short*, unsigned short*, int f) { trace += "help:" + std::to_string(p) + "," + std::to_string(f) + ";"; }
int hudGetFadeIconPos__Fii(int p, int s) { trace += "pos:" + std::to_string(p) + "," + std::to_string(s) + ";"; return 1000 + s; }
void* lumpFindResource__FPcT0(const char* l, const char* n) { trace += std::string("lump:") + l + "," + n + ";"; return arena; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void*, float r) { trace += "sound:" + std::to_string(r) + ";"; }
int netPlayerIsLocal__Fi(int) { return local; }
void msgInventoryGridDropItem__Fiiciiii(int p, int h, char k, int a, int b, int c, int two) {
    trace += "drop:" + std::to_string(p) + "," + std::to_string(h) + "," + std::to_string(k) + "," + std::to_string(a) + "," +
             std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(two) + ";";
}
void removeItemFromInventory__Fiiii(int p, int i, int t, int z) { trace += "remove:" + std::to_string(i) + "," + std::to_string(t) + "," + std::to_string(z) + ";"; }
int playerGetPotionsLeft__Fii(int, int l) { trace += "left:" + std::to_string(l) + ";"; return potionsLeft; }
void playerRecomputeWeight__Fi(int p) { trace += "weight:" + std::to_string(p) + ";"; }
}
static float& F(unsigned char* rec, int o) { return *reinterpret_cast<float*>(rec + o); }
// Item names live in arena strings; rows point at "item" words that point at the names.
static unsigned putString(unsigned at, const char* s) { std::strcpy(reinterpret_cast<char*>(arena + at), s); return at; }
static void setRow(unsigned char* rec, int row, unsigned itemAt, unsigned nameAt, unsigned short count) {
    *reinterpret_cast<unsigned*>(arena + itemAt) = nameAt;
    *reinterpret_cast<unsigned*>(rec + 9560 + 72 * row) = itemAt;
    *reinterpret_cast<unsigned short*>(rec + 9564 + 72 * row) = count;
}
int main() {
    unsigned char self[1024] = {0};
    unsigned char* rec = player;                        // player index 0
    *reinterpret_cast<int*>(self + 960) = 0;
    // early returns
    *reinterpret_cast<unsigned*>(self + 12) = 0x20000;
    drinkPotion__6Playeriiii(self, 0, 0, 0, 0);
    if (!trace.empty()) return 1;
    *reinterpret_cast<unsigned*>(self + 12) = 0;
    F(rec, 172) = 0.0f;
    drinkPotion__6Playeriiii(self, 0, 0, 0, 0);
    if (!trace.empty()) return 2;
    // health potion: need 130, so target (130+1)/60 = 2; the row holds list entry 2
    F(rec, 172) = 20.0f; F(rec, 176) = 150.0f; F(rec, 180) = 0.0f; F(rec, 196) = 5.0f; F(rec, 200) = 100.0f; F(rec, 204) = 0.0f;
    putString(0x1000, "minorhealpotion");
    putString(0x1100, "smallhealpotion");
    setRow(rec, 0, 0x2000, 0x1100, 2);
    setRow(rec, 1, 0x2010, 0x1000, 2);
    *reinterpret_cast<int*>(rec + 14168) = 2;
    drinkPotion__6Playeriiii(self, 0, 1, 2, 3);
    if (F(rec, 180) != 150.0f - 20.0f) return 3;         // 3*60 = 180 would pass the maximum, so limited
    if (trace != "lump:fx.lmp,spel_cas_curelw.anm;sound:1.000000;left:0;weight:0;") return 4;
    if (*reinterpret_cast<unsigned short*>(rec + 9564 + 72) != 1) return 5;
    if (F(rec, 184) != 150.0f * single_from_bits(0x3b088888)) return 6;
    // last unit leaves the inventory
    trace.clear(); F(rec, 180) = 0.0f; *reinterpret_cast<unsigned short*>(rec + 9564 + 72) = 1; potionsLeft = 3;
    drinkPotion__6Playeriiii(self, 0, 1, 2, 3);
    if (trace != "lump:fx.lmp,spel_cas_curelw.anm;sound:1.000000;remove:1,2,0;left:0;pos:0,4;weight:0;") return 7;
    if (fadeTimer[4] != 240 || fadeIconY[4] != 1004) return 8;
    // no matching potion: banner slot 1
    trace.clear(); *reinterpret_cast<int*>(rec + 14168) = 0;
    drinkPotion__6Playeriiii(self, 0, 0, 0, 0);
    if (trace != "pos:0,1;" || fadeTimer[1] != 120 || fadeIconY[1] != 1001) return 9;
    // mana potion (kind 1), nothing to drink -> slot 2
    trace.clear(); potionsLeft = 0;
    drinkPotion__6Playeriiii(self, 1, 0, 0, 0);
    if (trace != "pos:0,2;" || fadeTimer[2] != 120) return 10;
    // kind 2: tutorial once, picks slot 5 without checking found
    trace.clear(); *reinterpret_cast<int*>(rec + 14168) = 1;
    putString(0x1200, "greatrejuvpotion");
    setRow(rec, 0, 0x2020, 0x1200, 4);
    gameState = 0;
    drinkPotion__6Playeriiii(self, 2, 0, 0, 0);
    if (trace.find("get:PLAY_firstDrink;set:PLAY_firstDrink=1;text:HelpfulHint;text:TutPotionUseBody;help:0,0;") != 0) return 11;
    if (F(rec, 204) != 360.0f && F(rec, 204) != 100.0f - 5.0f) return 12;
    // grid drop when the interface is up and the player is local
    trace.clear(); interfaceOn = 1; local = 1; invGrid = 0x100000;
    *reinterpret_cast<unsigned*>(arena + 0x100000 + 0x17bb0 + 2 * 5400 + 3 * 72) = 0x3000;
    *reinterpret_cast<short*>(arena + 0x100000 + 0x17bb0 + 2 * 5400 + 3 * 72 + 34) = 77;
    arena[0x3000 + 29] = 9;
    F(rec, 204) = 0.0f;
    drinkPotion__6Playeriiii(self, 2, 2, 3, 4);
    if (trace.find("drop:0,77,9,2,3,4,2;") == std::string::npos) return 13;
    return 0;
}
