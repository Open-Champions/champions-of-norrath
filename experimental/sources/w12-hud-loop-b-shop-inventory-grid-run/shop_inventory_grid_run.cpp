// Observed shopInventoryGridRun__FiiiP12screenStruct at 0x001b8a28, 5132 bytes. Arguments: player pn,
// mode (0 buy from the shop grid, 1 sell from the inventory grid, 2 inventory with the equip / drop
// buttons, 3 inventory combine), side (0 or 1, the screen half) and a screen record (ints / floats:
// [0] x, [5] move direction, [6] [7] cursor cell, [9] [10] scroll, [12], [15] timer, [17] [18] mouse
// position, [19], [20] tab, [21], [22] help shown, [24] picked cell, [25] held cell, [26] [27] its
// column and row, [28]). rec is player + pn * 23984; pad is netLocalPad + 88 * pn, or zeroPad while
// gameControlPaused is set. A negative pn returns at once.
//  1. Back: with shopDisableMouse, shopMouseDecay, hideAbilityAdjustMouse (all [side]) and sellMode
//     [side] zero, padBackButton in pad word +12, multiHelpMessageOn[pn] and helpMessageOn zero: the
//     back sound "menu_shop_back.anm" (local players) and, for mode < 2, the shop screen record of the
//     side is reset ([19] = 3; [5] [9] [10] [6] [7] [16] [15] zero; then [6] = [21] and [9] = 39 * [21]);
//     for mode 2 and 3 statsScreen[side] [9] and [6] are cleared.
//  2. updateTaskBar(pn, screen) runs when the tab is below 4, or statsScreen[side] word 5 is zero.
//     With no mouse flags, shopRunMouseLogic(pn, side, screen, tab == 5 ? 500 : 2239) runs.
//  3. The cursor cell: column ((int)([17] - [0] - 17.0) / 32, at least -4), row ((int)([18] + (140 -
//     rcursor half height - top) + [10] - 229.0) / 34, -1 when negative); top = max(0, [9] - 2 *
//     buySellVertSpacing).
//  4. For the 75 rows by 9 columns of the grid (shopGrid when mode is 0 else invGrid, plus tab * 0xbdd8,
//     side * 0x23988, row * 72, column * 5400), tabs 0..2 only, the cursor cell with a nonzero description
//     word is handled with the button words (pad +12 pressed, +16 held; infoButton for modes 0 and 1,
//     padAcceptButton for 2 and 3, chosen by whether the stack count (halfword +4) is 21 or more
//     for description type 8 or 2 or more otherwise), as in the source below. Modes: 0 buy (afford,
//     pack space, weight, message to the shop), 1 sell (with a yes/no for equipped items), 2 equip /
//     unequip / drop, 3 combine through combineTable.
//  5. Unless a help message was shown, modes 2 and 3 end with doHelpfulMessage(side, pn, inventory
//     [side], mode, 0, 0).
// Inferred: this is the per-frame input handler of the shop / inventory item grids.
// Unknown: the meaning of the item flag bits 2^35, 2^38, 2^24 and the screen words [12], [28].
extern "C" {
extern int buySellVertSpacing;
extern int gameControlPaused;
extern unsigned padBackButton, padAcceptButton, infoButton, equipButton, dropButton;
extern signed char helpMessageOn;
extern signed char multiHelpMessageOn[];
extern int sellMode[];
extern int shopDisableMouse[2];
extern int shopMouseDecay[2];
extern int hideAbilityAdjustMouse[2];
extern unsigned char netLocalPad[];
extern unsigned char zeroPad[];
extern unsigned char player[];
extern unsigned char shop[];
extern unsigned char statsScreen[];
extern unsigned char inventory[];
extern unsigned char shopList[];
extern unsigned char combineTable[];
extern unsigned char* rcursor;
extern unsigned shopGrid;
extern unsigned invGrid;
extern int shopMsgHandle;
extern int fullCount;
extern int maxDistance;
extern int itemToDestroy;
extern int levelFlags;
struct Point3 { float x, y, z; };
int netPlayerIsLocal__Fi(int player);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume);
void updateTaskBar__FiP12screenStruct(int player, void* screen);
void shopRunMouseLogic__FiiP12screenStructi(int player, int side, void* screen, int a);
int compareToItem__Q24item11descriptionPQ24item11description(void* self, void* other);
int playerGetFeatRank__Fix(int player, long long feat);
float getWeight__4itemPQ24item11description(void* cell);
int getValue__4itemPQ24item11description(void* cell);
int netSendMessage__FiPUci(int handle, unsigned char* bytes, int count);
void playerRecomputeWeight__Fi(int player);
void playerRecomputeAbilityModifiers__Fv();
unsigned short* textGetString__FPc(const char* name);
void multiHudAddHelpMessage__FiPUsT1i(int player, unsigned short* title, unsigned short* body, int side);
void playerPlaySound__Fii(int player, int sound);
void doHelpfulMessage__FiiP12screenStructiiPQ24item11description(int side, int player, void* screen, int mode, int flag, void* cell);
int hudRunYesNo__FPUsiiii(unsigned short* text, int a, int player, int x, int b);
int itemEquipped__FiPQ24item11description(int player, void* item);
void msgUnequipItem__FiPQ24item11description(int player, void* item);
void msgEquipItem__FiPQ24item11descriptioni(int player, void* item, int a);
int strcmp(const char* a, const char* b);
char* strstr(const char* a, const char* b);
int sprintf(char* out, const char* format, ...);
int objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** out, int max);
void countItems__FP10GameObject(void* object);
void objectIterateGrid__FPFP10GameObject_v(void (*callback)(void*));
void msgInventoryGridDropItem__Fiiciiii(int player, int slot, char a, int column, int row, int side, int b);
int shopItemSetAttributes__FiiPcPQ24item11description(int player, int tab, char* name, void* cell);
void msgInvItemSetAttributes__FiiPcPQ24item11description(int player, int tab, char* name, void* item);
}

#ifdef W12_HUD_HOST
__UINTPTR_TYPE__ hostWordAddress(unsigned word);
unsigned hostAddressWord(const void* address);
#endif

namespace {
template <class T>
inline T load(const unsigned char* base, int offset) {
    T value;
    __builtin_memcpy(&value, base + offset, sizeof value);
    return value;
}
template <class T>
inline void store(unsigned char* base, int offset, T value) { __builtin_memcpy(base + offset, &value, sizeof value); }
// Pointer words of the image (cell descriptions, held-cell word, world object word) are 32 bits.
inline unsigned char* fromWord(unsigned word) {
#ifdef W12_HUD_HOST
    return reinterpret_cast<unsigned char*>(hostWordAddress(word));
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
inline unsigned toWord(const void* pointer) {
#ifdef W12_HUD_HOST
    return hostAddressWord(pointer);
#else
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
#endif
}
inline unsigned char* describe(const unsigned char* cell) { return fromWord(load<unsigned>(cell, 0)); }
inline void playFx(int player, const char* name) {
    if (netPlayerIsLocal__Fi(player) != 0)
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>(name)), nullptr, 1.0f);
}
// title text, then body text, then the multi-player help message.
inline void helpMessage(int player, const char* title, const char* body, int side) {
    unsigned short* t = textGetString__FPc(title);
    unsigned short* b = textGetString__FPc(body);
    multiHudAddHelpMessage__FiPUsT1i(player, t, b, side);
}
}

extern "C" void shopInventoryGridRun__FiiiP12screenStruct(int pn, int mode, int side, int* screenInts) {
    unsigned char* screen = reinterpret_cast<unsigned char*>(screenInts);
    float* floats = reinterpret_cast<float*>(screenInts);
    if (pn < 0) return;
    unsigned char* record = player + pn * 23984;
    unsigned char* pad = gameControlPaused != 0 ? zeroPad : netLocalPad + pn * 88;
    int top = screenInts[9] - buySellVertSpacing * 2;
    if (top < 0) top = 0;
    if (shopDisableMouse[side] == 0 && shopMouseDecay[side] == 0 && hideAbilityAdjustMouse[side] == 0 &&
        (load<unsigned>(pad, 12) & padBackButton) != 0 && multiHelpMessageOn[pn] == 0 && helpMessageOn == 0 && sellMode[side] == 0) {
        playFx(pn, "menu_shop_back.anm");
        if (mode < 2) {
            int* s = reinterpret_cast<int*>(shop + side * 116);
            s[19] = 3;
            s[5] = 0;
            s[9] = 0;
            s[10] = 0;
            s[6] = 0;
            s[7] = 0;
            reinterpret_cast<float*>(s)[16] = 0.0f;
            const int tabTarget = s[21];
            reinterpret_cast<float*>(s)[15] = 0.0f;
            s[6] = tabTarget;
            s[9] = tabTarget * 39;
        } else {
            int* s = reinterpret_cast<int*>(statsScreen + side * 116);
            s[9] = 0;
            s[6] = 0;
        }
    }
    if (screenInts[20] < 4 || load<int>(statsScreen + side * 116, 20) == 0) updateTaskBar__FiP12screenStruct(pn, screenInts);
    if (shopDisableMouse[side] == 0 && shopMouseDecay[side] == 0 && hideAbilityAdjustMouse[side] == 0)
        shopRunMouseLogic__FiiP12screenStructi(pn, side, screenInts, screenInts[20] == 5 ? 500 : 2239);
    // Cursor cell.
    const int half = static_cast<short>(load<unsigned short>(rcursor, 2)) >> 1;
    const int columnValue = static_cast<int>(floats[17] - static_cast<float>(screenInts[0]) - 17.0f);
    const float lineBase = static_cast<float>(-half - top + 140);
    const int rowValue = static_cast<int>(floats[18] + lineBase + static_cast<float>(screenInts[10]) - 229.0f);
    int cursorColumn = columnValue / 32;
    int cursorRow = rowValue / 34;
    if (columnValue < -4) cursorColumn = -4;
    if (rowValue < 0) cursorRow = -1;
    int shown = 0;
    for (int row = 0; row < 75; ++row) {
        for (int column = 0; column < 9; ++column) {
            const int tab = screenInts[20];
            const unsigned gridBase = mode == 0 ? shopGrid : invGrid;
            unsigned char* cell = fromWord(gridBase + static_cast<unsigned>(tab) * 0xbdd8u + static_cast<unsigned>(side) * 0x23988u +
                                           static_cast<unsigned>(row * 72 + column * 5400));
            if (record[240] != 0 && tab == 5) continue;
            if (!(tab < 3)) continue;
            if (cursorRow != row) continue;
            if (cursorColumn != column) continue;
            unsigned char* description = describe(cell);
            if (description == nullptr) continue;
            // Button states: pressed (pad +12) and held (pad +16).
            unsigned state[2];
            const unsigned button = mode < 2 ? infoButton : padAcceptButton;
            state[0] = load<unsigned>(pad, 12) & button;
            state[1] = load<unsigned>(pad, 16) & button;
            int stacked = 0;
            const short count = load<short>(cell, 4);
            if (static_cast<signed char>(description[28]) == 8) stacked = count >= 21 ? 1 : 0;
            else stacked = count >= 2 ? 1 : 0;
            unsigned char* inventoryCell = fromWord(invGrid + static_cast<unsigned>(side) * 0x23988u + static_cast<unsigned>(tab) * 0xbdd8u +
                                                    static_cast<unsigned>(row * 72 + column * 5400));
            if (mode == 0) {
                if (multiHelpMessageOn[pn] != 0) continue;
                if (helpMessageOn != 0) continue;
                if (state[stacked] == 0) continue;
                float factor = 1.0f;
                if (static_cast<signed char>(description[28]) == 8 && playerGetFeatRank__Fix(pn, 0x10000) != 0) factor = 0.5f;
                const float weight = getWeight__4itemPQ24item11description(cell) * factor;
                const int value = getValue__4itemPQ24item11description(cell);
                if (load<int>(record, 232) < value) {
                    playFx(pn, "menu_shop_nobuy.anm");
                    unsigned short* t = textGetString__FPc("CantBuyTitle");
                    unsigned short* b = textGetString__FPc("CantBuy");
                    multiHudAddHelpMessage__FiPUsT1i(pn, t, b, side);
                    continue;
                }
                if (load<int>(record, 4936 + tab * 4616) >= 64) {
                    playFx(pn, "menu_shop_nobuy.anm");
                    continue;
                }
                if (!(load<float>(record, 308) + weight <= load<float>(record, 312))) {
                    playFx(pn, "menu_shop_nobuy.anm");
                    unsigned short* t = textGetString__FPc("CantHoldTitle");
                    unsigned short* b = textGetString__FPc("CantHold");
                    multiHudAddHelpMessage__FiPUsT1i(pn, t, b, side);
                    playerPlaySound__Fii(pn, 0);
                    continue;
                }
                playFx(pn, "menu_shop_buy.anm");
                unsigned char message[16];
                unsigned char* writer = message;
                unsigned char* found = nullptr;
                for (int page = 0; page < 3; ++page) {
                    unsigned char* pageBase = shopList + page * 4616;
                    const int items = load<int>(pageBase, 4608);
                    for (int i = 0; i < items; ++i) {
                        unsigned char* candidate = pageBase + i * 72;
                        if (compareToItem__Q24item11descriptionPQ24item11description(candidate, cell) != 0) found = candidate;
                    }
                }
                if (found != nullptr) {
                    *writer++ = 0;
                    *writer++ = static_cast<unsigned char>(pn);
                    store<short>(writer, 0, 0);
                    writer += 2;
                    store<unsigned>(writer, 0, toWord(found));
                    writer += 4;
                    netSendMessage__FiPUci(shopMsgHandle, message, static_cast<int>(writer - message));
                }
                playerRecomputeWeight__Fi(pn);
                continue;
            }
            if (mode == 1) {
                getValue__4itemPQ24item11description(cell);
                if (static_cast<signed char>(description[28]) != 8) strstr(reinterpret_cast<const char*>(fromWord(load<unsigned>(description, 0))), "spear");
                if (state[stacked] != 0 && floats[15] == 0.0f) sellMode[side] = 1;
                if (sellMode[side] == 0) continue;
                if ((load<unsigned long long>(cell, 8) & 0x4000000000ull) != 0) {
                    sellMode[side] = 0;
                    playFx(pn, "menu_shop_nobuy.anm");
                    helpMessage(pn, "CantSellTitle", "CantSell", side);
                    return;
                }
                unsigned char* invCell = inventoryCell;
                unsigned char* item = record + 328 + tab * 4616 + load<short>(invCell, 34) * 72;
                int choice = 3;
                if (itemEquipped__FiPQ24item11description(pn, item) != 0) {
                    shopMouseDecay[side] = 100;
                    unsigned short* text = textGetString__FPc("AreYouSure");
                    choice = hudRunYesNo__FPUsiiii(text, -150, pn, side == 1 ? 160 : -160, 0);
                    if (choice != 2 && choice == 1) {
                        shopMouseDecay[side] = 0;
                        sellMode[side] = 0;
                    }
                }
                if (choice < 2) continue;
                shopMouseDecay[side] = 0;
                playFx(pn, "menu_shop_buy.anm");
                if (load<unsigned>(cell, 0) != 0) {
                    unsigned char* world = fromWord(load<unsigned>(record, 21872));
                    if (static_cast<signed char>(world[1130]) == 0) {
                        world[1130] = 1;
                        unsigned char message[16];
                        message[0] = 1;
                        message[1] = static_cast<unsigned char>(pn);
                        store<short>(message, 2, static_cast<short>(column));
                        store<int>(message, 4, load<short>(cell, 34));
                        store<short>(message, 8, static_cast<short>(row));
                        message[10] = description[29];
                        message[11] = static_cast<unsigned char>(side);
                        message[12] = 1;
                        netSendMessage__FiPUci(shopMsgHandle, message, 13);
                    }
                }
                playerRecomputeWeight__Fi(pn);
                playerRecomputeAbilityModifiers__Fv();
                sellMode[side] = 0;
                continue;
            }
            if (mode == 2) {
                if (multiHelpMessageOn[pn] != 0) continue;
                if (helpMessageOn != 0) continue;
                shown = 1;
                doHelpfulMessage__FiiP12screenStructiiPQ24item11description(side, pn, inventory + side * 116, 2, 0, cell);
                if (floats[15] != 0.0f) continue;
                const unsigned dropState = load<unsigned>(pad, stacked != 0 ? 16 : 12) & dropButton;
                store<int>(screen, 112, 0);
                store<unsigned>(screen, 96, toWord(cell));
                if ((load<unsigned>(pad, 12) & equipButton) != 0) {
                    if ((load<unsigned long long>(cell, 8) & 0x800000000ull) != 0) {
                        store<unsigned>(screen, 100, toWord(cell));
                        mode = 3;
                        store<int>(screen, 88, shown);
                        store<int>(screen, 104, column);
                        store<int>(screen, 108, row);
                        continue;
                    }
                    unsigned char* item = record + 328 + tab * 4616 + load<short>(inventoryCell, 34) * 72;
                    if (itemEquipped__FiPQ24item11description(pn, item) != 0) msgUnequipItem__FiPQ24item11description(pn, item);
                    else msgEquipItem__FiPQ24item11descriptioni(pn, item, 0);
                    continue;
                }
                if (dropState == 0) continue;
                if ((load<unsigned long long>(cell, 8) & 0x4000000000ull) != 0) {
                    playFx(pn, "menu_shop_nobuy.anm");
                    helpMessage(pn, "CantDropQuestTitle", "CantDropQuestBody", side);
                    return;
                }
                unsigned char* world = fromWord(load<unsigned>(record, 21872));
                const float* at = reinterpret_cast<const float*>(world + 16);
                void* found[130];
                objectFindInBox__FG6Point3T0PP10GameObjecti(Point3{at[0] - 240.0f, at[1] - 240.0f, at[2] - 12000.0f}, Point3{at[0] + 240.0f, at[1] + 240.0f, at[2] + 12000.0f}, found, 128);
                int near = 0;
                if (found[0] != nullptr)
                    for (void** p = found; *p != nullptr; ++p) ++near;
                fullCount = 0;
                maxDistance = 0;
                itemToDestroy = 0;
                objectIterateGrid__FPFP10GameObject_v(countItems__FP10GameObject);
                if ((levelFlags & 0x4000) != 0) fullCount = fullCount << 2;
                if (fullCount >= 128 && itemToDestroy != 0) {
                    unsigned char* victim = fromWord(static_cast<unsigned>(itemToDestroy));
                    const unsigned char* entry = fromWord(load<unsigned>(victim, 72)) + 8;
                    typedef void (*Destroy)(void*, int);
                    reinterpret_cast<Destroy>(fromWord(load<unsigned>(entry, 4)))(victim + load<short>(entry, 0), 3);
                    itemToDestroy = 0;
                }
                if (!(near < 33)) {
                    playFx(pn, "menu_shop_nobuy.anm");
                    unsigned short* t = textGetString__FPc("CantDropTitle");
                    unsigned short* b = textGetString__FPc("CantDrop");
                    multiHudAddHelpMessage__FiPUsT1i(pn, t, b, side);
                    continue;
                }
                if (load<unsigned>(cell, 0) == 0) continue;
                if (static_cast<signed char>(world[1130]) != 0) continue;
                world[1130] = 1;
                msgInventoryGridDropItem__Fiiciiii(pn, load<short>(cell, 34), static_cast<char>(description[29]), column, row, side, 0);
                continue;
            }
            if (mode != 3) continue;
            if (multiHelpMessageOn[pn] != 0) continue;
            if (helpMessageOn != 0) continue;
            shown = 1;
            doHelpfulMessage__FiiP12screenStructiiPQ24item11description(side, pn, inventory + side * 116, 3, 0, cell);
            if (floats[15] != 0.0f) continue;
            const unsigned char* pageGrid = fromWord(invGrid + static_cast<unsigned>(side) * 0x23988u + static_cast<unsigned>(tab) * 0xbdd8u);
            store<int>(screen, 112, 0);
            store<unsigned>(screen, 96, toWord(cell));
            if ((load<unsigned>(pad, 12) & equipButton) == 0) continue;
            const unsigned char marker = cell[33];
            bool rejected = false;
            if (marker != 255) {
                helpMessage(pn, "CantCombineTitle", "CantCombineMagic", side);
                rejected = true;
            } else if (static_cast<signed char>(description[28]) == 8 || description[29] >= 2 || (load<unsigned long long>(cell, 8) & 0x1000000ull) != 0) {
                helpMessage(pn, "CantCombineTitle", "CantCombine", side);
                rejected = true;
            }
            if (rejected) {
                playFx(pn, "menu_shop_nobuy.anm");
                continue;
            }
            if (load<unsigned>(combineTable, 0) == 0) return;
            unsigned char* held = fromWord(load<unsigned>(screen, 100));
            int entryIndex = 0, wordBase = 0, doubled = 0;
            const unsigned char* entry = combineTable;
            bool matched = false;
            do {
                if (strcmp(reinterpret_cast<const char*>(fromWord(load<unsigned>(describe(held), 0))), reinterpret_cast<const char*>(fromWord(load<unsigned>(entry, 0)))) == 0) {
                    const unsigned resultName = load<unsigned>(combineTable + 4, (wordBase + static_cast<signed char>(description[29])) * 4);
                    if (resultName == 0) {
                        playFx(pn, "menu_shop_nobuy.anm");
                        helpMessage(pn, "CantCombineTitle", "CantCombine", side);
                        return;
                    }
                    char text[64];
                    sprintf(text, "%s", reinterpret_cast<const char*>(fromWord(resultName)));
                    matched = true;
                    break;
                }
                entry += 12;
                doubled += 2;
                wordBase += 3;
                ++entryIndex;
            } while (load<unsigned>(entry, 0) != 0);
            (void)matched;
            const int nameBase = doubled + entryIndex;
            if (load<unsigned>(combineTable, nameBase * 4) == 0) return;
            int lastResult = 0;
            for (int r = 0; r < 75; ++r) {
                unsigned char* scan = const_cast<unsigned char*>(pageGrid) + r * 72;
                for (int c = 8; c >= 0; --c) {
                    unsigned char* candidate = scan;
                    scan += 5400;
                    if (compareToItem__Q24item11descriptionPQ24item11description(candidate, cell) != 0)
                        lastResult = shopItemSetAttributes__FiiPcPQ24item11description(
                            pn, screenInts[20], reinterpret_cast<char*>(fromWord(load<unsigned>(combineTable + 4, (nameBase + static_cast<signed char>(description[29])) * 4))),
                            candidate);
                }
            }
            if (lastResult == 0) return;
            unsigned char* item = record + 328 + tab * 4616 + load<short>(cell, 34) * 72;
            msgInvItemSetAttributes__FiiPcPQ24item11description(
                pn, screenInts[20], reinterpret_cast<char*>(fromWord(load<unsigned>(combineTable, (nameBase + static_cast<signed char>(description[29])) * 4 + 4))), item);
            unsigned char* heldCell = fromWord(load<unsigned>(screen, 100));
            unsigned char* heldDescription = describe(heldCell);
            mode = 2;
            msgInventoryGridDropItem__Fiiciiii(pn, load<short>(heldCell, 34), static_cast<char>(heldDescription[29]), load<int>(screen, 104), load<int>(screen, 108), side, 2);
            playerRecomputeWeight__Fi(pn);
            store<int>(screen, 88, 0);
            continue;
        }
    }
    if (shown != 0) return;
    if (mode == 2 || mode == 3) doHelpfulMessage__FiiP12screenStructiiPQ24item11description(side, pn, inventory + side * 116, mode, 0, nullptr);
}
