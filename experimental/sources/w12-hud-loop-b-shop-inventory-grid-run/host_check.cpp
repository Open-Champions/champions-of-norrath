// Not an EE plan input. Build: c++ -std=c++17 -Wall -Wextra -Werror -ffp-contract=off -fno-builtin -DW12_HUD_HOST host_check.cpp
// 32-bit pointer words are handles: top byte picks a registered host block, low 24 bits the offset in it.
#include "shop_inventory_grid_run.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" {
int buySellVertSpacing = 34, gameControlPaused, shopMsgHandle = 9, fullCount, maxDistance, itemToDestroy, levelFlags;
unsigned padBackButton = 0x80, padAcceptButton = 0x4, infoButton = 0x2, equipButton = 0x8, dropButton = 0x10;
signed char helpMessageOn;
signed char multiHelpMessageOn[4];
int sellMode[2], shopDisableMouse[2], shopMouseDecay[2], hideAbilityAdjustMouse[2];
alignas(8) unsigned char netLocalPad[4 * 88], zeroPad[96], player[4 * 23984], shop[232], statsScreen[232], inventory[232], shopList[3 * 4616], combineTable[256];
unsigned char* rcursor;
unsigned shopGrid, invGrid;
}
static std::vector<std::pair<unsigned char*, size_t>> blocks;
static unsigned reg(unsigned char* p, size_t n) { blocks.push_back({p, n}); return static_cast<unsigned>(blocks.size()) << 24; }
__UINTPTR_TYPE__ hostWordAddress(unsigned word) {
    const unsigned i = word >> 24;
    return i == 0 || i > blocks.size() ? 0 : reinterpret_cast<__UINTPTR_TYPE__>(blocks[i - 1].first) + (word & 0xffffff);
}
unsigned hostAddressWord(const void* a) {
    for (size_t i = 0; i < blocks.size(); ++i) {
        const unsigned char* b = blocks[i].first;
        if (static_cast<const unsigned char*>(a) >= b && static_cast<const unsigned char*>(a) < b + blocks[i].second)
            return static_cast<unsigned>((i + 1) << 24) + static_cast<unsigned>(static_cast<const unsigned char*>(a) - b);
    }
    return 0;
}
static std::string trace;
static int localAnswer = 1, yesno, featRank, valueAnswer, equippedAnswer, compareAnswer = 1, attrAnswer = 1;
static float weightAnswer;
static std::string num(long v) { return std::to_string(v); }
extern "C" {
int netPlayerIsLocal__Fi(int) { return localAnswer; }
void* lumpFindResource__FPcT0(char*, char* n) { trace += std::string("snd:") + n + ";"; return nullptr; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void*, float) {}
void updateTaskBar__FiP12screenStruct(int p, void*) { trace += "bar" + num(p) + ";"; }
void shopRunMouseLogic__FiiP12screenStructi(int p, int s, void*, int a) { trace += "mouse(" + num(p) + "," + num(s) + "," + num(a) + ");"; }
int compareToItem__Q24item11descriptionPQ24item11description(void*, void*) { return compareAnswer; }
int playerGetFeatRank__Fix(int, long long) { return featRank; }
float getWeight__4itemPQ24item11description(void*) { return weightAnswer; }
int getValue__4itemPQ24item11description(void*) { return valueAnswer; }
int netSendMessage__FiPUci(int h, unsigned char* b, int n) {
    trace += "send" + num(h) + ":";
    for (int i = 0; i < n; ++i) trace += num(b[i]) + (i + 1 < n ? "," : "");
    trace += ";";
    return 0;
}
void playerRecomputeWeight__Fi(int p) { trace += "weight" + num(p) + ";"; }
void playerRecomputeAbilityModifiers__Fv() { trace += "abil;"; }
unsigned short* textGetString__FPc(const char* n) { static unsigned short buf[64]; trace += std::string("txt:") + n + ";"; buf[0] = 1; return buf; }
void multiHudAddHelpMessage__FiPUsT1i(int p, unsigned short*, unsigned short*, int s) { trace += "help(" + num(p) + "," + num(s) + ");"; }
void playerPlaySound__Fii(int p, int s) { trace += "psound(" + num(p) + "," + num(s) + ");"; }
void doHelpfulMessage__FiiP12screenStructiiPQ24item11description(int s, int p, void*, int m, int f, void* c) { trace += "dhm(" + num(s) + "," + num(p) + "," + num(m) + "," + num(f) + "," + (c ? "c" : "-") + ");"; }
int hudRunYesNo__FPUsiiii(unsigned short*, int a, int p, int x, int b) { trace += "yesno(" + num(a) + "," + num(p) + "," + num(x) + "," + num(b) + ");"; return yesno; }
int itemEquipped__FiPQ24item11description(int, void*) { return equippedAnswer; }
void msgUnequipItem__FiPQ24item11description(int p, void*) { trace += "unequip" + num(p) + ";"; }
void msgEquipItem__FiPQ24item11descriptioni(int p, void*, int a) { trace += "equip" + num(p) + "," + num(a) + ";"; }
int strcmp(const char* a, const char* b) { while (*a && *a == *b) { ++a; ++b; } return static_cast<unsigned char>(*a) - static_cast<unsigned char>(*b); }
char* strstr(const char* a, const char* b) { trace += std::string("strstr:") + b + ";"; (void)a; return nullptr; }
int sprintf(char* out, const char* f, ...) { trace += "sprintf;"; std::strcpy(out, f); return 0; }
int objectFindInBox__FG6Point3T0PP10GameObjecti(Point3, Point3, void** out, int) { out[0] = nullptr; return 0; }
void countItems__FP10GameObject(void*) {}
void objectIterateGrid__FPFP10GameObject_v(void (*)(void*)) { trace += "iterate;"; }
void msgInventoryGridDropItem__Fiiciiii(int p, int slot, char a, int c, int r, int s, int b) {
    trace += "drop(" + num(p) + "," + num(slot) + "," + num(a) + "," + num(c) + "," + num(r) + "," + num(s) + "," + num(b) + ");";
}
int shopItemSetAttributes__FiiPcPQ24item11description(int, int, char*, void*) { trace += "setattr;"; return attrAnswer; }
void msgInvItemSetAttributes__FiiPcPQ24item11description(int p, int t, char*, void*) { trace += "invattr(" + num(p) + "," + num(t) + ");"; }
}
alignas(8) static unsigned char gridA[0x100000], gridB[0x100000], descBlock[512], worldBlock[1400], heldCell[128];
static int bad;
static void expect(const char* what, const std::string& got, const std::string& want) {
    if (got != want) { std::printf("FAIL %s\n got  %s\n want %s\n", what, got.c_str(), want.c_str()); bad = 1; }
}
static void setw(unsigned char* b, int off, int v) { std::memcpy(b + off, &v, 4); }
static void setf(unsigned char* b, int off, float v) { std::memcpy(b + off, &v, 4); }
static unsigned char* screenBlock;
alignas(8) static unsigned char screenStore[232];
static unsigned char cursorTexture[8];
static const int kCol = 2, kRow = 3;
static unsigned char* cellOf(int mode, int tab, int side) {
    return (mode == 0 ? gridA : gridB) + tab * 0xbdd8 + side * 0x23988 + kRow * 72 + kCol * 5400;
}
static void press(unsigned v) { setw(netLocalPad + 88, 12, v); setw(netLocalPad + 88, 16, v); }
static void pressHeld(unsigned v) { setw(netLocalPad + 88, 16, v); }
static void pressNow(unsigned v) { setw(netLocalPad + 88, 12, v); }
static void reset(int pn) {
    blocks.clear();
    std::memset(gridA, 0, sizeof gridA); std::memset(gridB, 0, sizeof gridB); std::memset(descBlock, 0, sizeof descBlock);
    std::memset(worldBlock, 0, sizeof worldBlock); std::memset(heldCell, 0, sizeof heldCell); std::memset(player, 0, sizeof player);
    std::memset(screenStore, 0, sizeof screenStore); std::memset(shop, 0, sizeof shop); std::memset(statsScreen, 0, sizeof statsScreen);
    std::memset(netLocalPad, 0, sizeof netLocalPad); std::memset(combineTable, 0, sizeof combineTable); std::memset(shopList, 0, sizeof shopList);
    shopGrid = reg(gridA, sizeof gridA); invGrid = reg(gridB, sizeof gridB);
    const unsigned descW = reg(descBlock, sizeof descBlock), worldW = reg(worldBlock, sizeof worldBlock), heldW = reg(heldCell, sizeof heldCell);
    (void)heldW;
    setw(player + pn * 23984, 21872, static_cast<int>(worldW));
    trace.clear(); gameControlPaused = 0; helpMessageOn = 0; std::memset(multiHelpMessageOn, 0, sizeof multiHelpMessageOn);
    sellMode[0] = sellMode[1] = shopDisableMouse[0] = shopDisableMouse[1] = shopMouseDecay[0] = shopMouseDecay[1] = hideAbilityAdjustMouse[0] = hideAbilityAdjustMouse[1] = 0;
    localAnswer = 1; yesno = 0; featRank = 0; valueAnswer = 10; equippedAnswer = 0; compareAnswer = 1; weightAnswer = 1.0f; attrAnswer = 1;
    rcursor = cursorTexture; std::memset(cursorTexture, 0, sizeof cursorTexture);
    setw(screenStore, 80, 0); setf(screenStore, 68, 17.0f + kCol * 32 + 5); setf(screenStore, 72, kRow * 34 + 5 - 140 + 229.0f);
    setw(screenStore, 0, 0); setw(screenStore, 36, 0); setw(screenStore, 40, 0);
    unsigned char* cell = cellOf(1, 0, 0);
    setw(cell, 0, static_cast<int>(descW)); descBlock[28] = 1; descBlock[29] = 0; cell[33] = 255;
    std::memcpy(cellOf(0, 0, 0), cell, 0); setw(cellOf(0, 0, 0), 0, static_cast<int>(descW));
    short count = 5; std::memcpy(cell + 4, &count, 2); std::memcpy(cellOf(0, 0, 0) + 4, &count, 2);
    setw(player + pn * 23984, 232, 1000); setf(player + pn * 23984, 308, 10.0f); setf(player + pn * 23984, 312, 100.0f);
    (void)screenBlock;
}
static void run(int pn, int mode, int side) { shopInventoryGridRun__FiiiP12screenStruct(pn, mode, side, reinterpret_cast<int*>(screenStore)); }

int main() {
    // Negative player: nothing.
    reset(1); run(-1, 0, 0); expect("negative", trace, "");
    // Idle pass: task bar and mouse logic, then nothing for a cursor on an empty-handed cell without buttons.
    reset(1); run(1, 0, 0); expect("idle", trace, "bar1;mouse(1,0,2239);");
    reset(1); setw(screenStore, 80, 5); run(1, 0, 0); expect("tab 5", trace, "bar1;mouse(1,0,500);");
    reset(1); shopDisableMouse[0] = 1; run(1, 0, 0); expect("mouse disabled", trace, "bar1;");
    // Buy with the info button held (mode 0): sends the shop message when a stock entry matches.
    reset(1); press(0x2); run(1, 0, 0);
    expect("buy", trace, "bar1;mouse(1,0,2239);snd:menu_shop_buy.anm;weight1;");
        reset(1); press(0x2); valueAnswer = 5000; run(1, 0, 0);
    expect("cannot afford", trace, "bar1;mouse(1,0,2239);snd:menu_shop_nobuy.anm;txt:CantBuyTitle;txt:CantBuy;help(1,0);");
    reset(1); press(0x2); setw(player + 23984, 4936, 64); run(1, 0, 0);
    expect("pack full", trace, "bar1;mouse(1,0,2239);snd:menu_shop_nobuy.anm;");
    reset(1); press(0x2); weightAnswer = 95.0f; run(1, 0, 0);
    expect("too heavy", trace, "bar1;mouse(1,0,2239);snd:menu_shop_nobuy.anm;txt:CantHoldTitle;txt:CantHold;help(1,0);psound(1,0);");
    // A stack of 2+ reads the held word (+16) instead of the pressed word (+12).
    reset(1); pressHeld(0x2); run(1, 0, 0); expect("held button", num(trace.find("snd:menu_shop_buy") != std::string::npos), "1");
    reset(1); pressNow(0x2); { short one = 1; std::memcpy(cellOf(0, 0, 0) + 4, &one, 2); } run(1, 0, 0);
    expect("single item uses pressed word", num(trace.find("snd:menu_shop_buy") != std::string::npos), "1");
    reset(1); pressHeld(0x2); { short one = 1; std::memcpy(cellOf(0, 0, 0) + 4, &one, 2); } run(1, 0, 0);
    expect("single item ignores held word", num(trace.find("snd:menu_shop_buy") == std::string::npos), "1");
    // Sell (mode 1): the sell flag is raised, the item is sold when not equipped; equipped items ask first.
    reset(1); press(0x2); run(1, 1, 0);
    expect("sell", trace, "bar1;mouse(1,0,2239);strstr:spear;snd:menu_shop_buy.anm;send9:1,1,2,0,0,0,0,0,3,0,0,0,1;weight1;abil;");
    reset(1); press(0x2); equippedAnswer = 1; yesno = 0; run(1, 1, 0);
    expect("sell asks when equipped", trace, "bar1;mouse(1,0,2239);strstr:spear;txt:AreYouSure;yesno(-150,1,-160,0);");
    expect("decay", num(shopMouseDecay[0]), "100");
    reset(1); press(0x2); equippedAnswer = 1; yesno = 1; run(1, 1, 0);
    expect("sell cancelled", num(shopMouseDecay[0]) + num(sellMode[0]), "00");
    reset(1); press(0x2); { unsigned long long f = 0x4000000000ull; std::memcpy(cellOf(1, 0, 0) + 8, &f, 8); } run(1, 1, 0);
    expect("unsellable", trace, "bar1;mouse(1,0,2239);strstr:spear;snd:menu_shop_nobuy.anm;txt:CantSellTitle;txt:CantSell;help(1,0);");
    // Equip/unequip (mode 2) with the equip button; drop with the drop button.
    reset(1); press(0x8); run(1, 2, 0);
    expect("equip", trace, "bar1;mouse(1,0,2239);dhm(0,1,2,0,c);equip1,0;");
    reset(1); press(0x8); equippedAnswer = 1; run(1, 2, 0);
    expect("unequip", num(trace.find("unequip1;") != std::string::npos), "1");
    reset(1); press(0x10); run(1, 2, 0);
    expect("drop", trace, "bar1;mouse(1,0,2239);dhm(0,1,2,0,c);iterate;drop(1,0,0,2,3,0,0);");
    std::printf(bad ? "FAIL\n" : "ok: shop inventory grid run\n");
    return bad;
}
