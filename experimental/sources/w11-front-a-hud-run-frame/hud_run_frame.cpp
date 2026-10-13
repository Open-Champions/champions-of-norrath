// Observed: HUDRunFrame__Fv at 0x001c3bc0 is 764 bytes, no arguments, no result. HUDInit__Fv
// registers it as the engine task "HUD Run" (priority 50). In order:
//  1. runGold__Fv().
//  2. When the signed byte hudNoDraw is zero, the word coreLoading is zero and the signed halfword
//     infoMessageTimer is positive: the timer drops by one; and when nmPlayers is 4 and the byte
//     netNetworkMode is zero, hudBustCovered becomes 1. (The timer store comes before the
//     nmPlayers test.)
//  3. For i from 0 while i < nmPlayers (read again at the end of each pass), with e the 88-byte
//     record i of netLocalPad: the pass does nothing more, and i advances, when any of these holds,
//     tested in this order: bit 0x100 of the word at e + 12 is clear; bit 0x80 of the word at
//     e + 8 is set; interfaceOn is nonzero; gameIsPaused__Fv() is nonzero; the int at gameCam + 52
//     is not 1; cutMode is nonzero; hudNetWaiting is nonzero; the player record's pointer word p
//     (word at player + 23984 * i + 21872) is nonzero and the int at p + 1328 is nonzero;
//     jumpGateJumping__Fv() is nonzero; engineTaskActive__FPc("jumpgate") is nonzero; recallOn is
//     nonzero; p (read again) is zero; the word at p + 12 has bit 0x400000. Otherwise, with
//     flags the word at p + 12: when bit 8 of flags is clear and the float at player record
//     + 172 is greater than 0.0, with b8 and b9 the signed bytes at record + 88 and + 89: b8 < b9
//     calls inventoryStart__Fii(i, 1) and zeroes the int levelUpFade[i]; otherwise
//     inventoryStart__Fii(i, 0). Then, for every player j below nmPlayers (read again after each
//     call) whose int at player + 23984 * j + 4936 is positive, scriptStart__FPc
//     ("*inventoryOnNotify") runs.
//  4. When the byte MC_Debug is nonzero, bit 0x4000 of the word at pad + 12 and bit 8 of the word at
//     pad + 8 are both set: the int at player + 232 is set to 0x16e360 (even with no player), and
//     givePlayerExperience__Fii(k, 20000) runs for every k below nmPlayers.
//  5. A nonzero drawBustOn drops by one; a nonzero drawCompassOn drops by one (signed byte tests,
//     stored back as bytes).
// Inferred: the pass in step 3 opens the inventory for a local player who presses the inventory
// button, with the first branch picking the screen for a player who has a pending level-up; the
// MC_Debug branch is a debug cheat that grants gold and experience.
// Unknown: what the bit 0x100/0x80 flags in netLocalPad are, what the word at p + 1328 and the
// int at record + 4936 mean, and what the byte pair at record + 88 / + 89 means.
extern "C" {
void runGold__Fv();
extern signed char hudNoDraw;
extern int coreLoading;
extern unsigned short infoMessageTimer;
extern int nmPlayers;
extern signed char netNetworkMode;
extern unsigned char hudBustCovered;
extern unsigned char netLocalPad[];
extern signed char interfaceOn;
int gameIsPaused__Fv();
extern unsigned char* gameCam;
extern signed char cutMode;
extern signed char hudNetWaiting;
extern unsigned char player[];
int jumpGateJumping__Fv();
int engineTaskActive__FPc(char* name);
extern signed char recallOn;
void inventoryStart__Fii(int player_index, int mode);
extern int levelUpFade[];
void scriptStart__FPc(char* name);
extern signed char MC_Debug;
extern signed char drawBustOn;
extern signed char drawCompassOn;
extern unsigned char pad[];
void givePlayerExperience__Fii(int player_index, int amount);
}

#ifdef W11_FRONT_A_HOST
__UINTPTR_TYPE__ hostWordAddress(unsigned word);
#endif

namespace {
inline int readInt(__UINTPTR_TYPE__ address) {
    int v;
    __builtin_memcpy(&v, reinterpret_cast<const void*>(address), 4);
    return v;
}
inline __UINTPTR_TYPE__ playerPointer(int index) {
    const unsigned word = static_cast<unsigned>(readInt(reinterpret_cast<__UINTPTR_TYPE__>(player) + index * 23984 + 21872));
#ifdef W11_FRONT_A_HOST
    return hostWordAddress(word);
#else
    return word;
#endif
}
}

extern "C" void HUDRunFrame__Fv() {
    runGold__Fv();
    if (hudNoDraw == 0 && coreLoading == 0 && static_cast<short>(infoMessageTimer) > 0) {
        infoMessageTimer = static_cast<unsigned short>(infoMessageTimer - 1);
        if (nmPlayers == 4 && netNetworkMode == 0) hudBustCovered = 1;
    }
    for (int i = 0; i < nmPlayers; ++i) {
        const __UINTPTR_TYPE__ e = reinterpret_cast<__UINTPTR_TYPE__>(netLocalPad) + 88 * i;
        if ((readInt(e + 12) & 0x100) == 0) continue;
        if ((readInt(e + 8) & 0x80) != 0) continue;
        if (interfaceOn != 0) continue;
        if (gameIsPaused__Fv() != 0) continue;
        if (readInt(reinterpret_cast<__UINTPTR_TYPE__>(gameCam) + 52) != 1) continue;
        if (cutMode != 0) continue;
        if (hudNetWaiting != 0) continue;
        __UINTPTR_TYPE__ p = playerPointer(i);
        if (p != 0 && readInt(p + 1328) != 0) continue;
        if (jumpGateJumping__Fv() != 0) continue;
        if (engineTaskActive__FPc(const_cast<char*>("jumpgate")) != 0) continue;
        if (recallOn != 0) continue;
        p = playerPointer(i);
        if (p == 0) continue;
        const int flags = readInt(p + 12);
        if ((flags & 0x400000) != 0) continue;
        unsigned char* record = player + 23984 * i;
        if ((flags & 8) == 0 && 0.0f < *reinterpret_cast<float*>(record + 172)) {
            if (static_cast<signed char>(record[88]) < static_cast<signed char>(record[89])) {
                inventoryStart__Fii(i, 1);
                levelUpFade[i] = 0;
            } else {
                inventoryStart__Fii(i, 0);
            }
        }
        for (int j = 0; j < nmPlayers; ++j) {
            if (readInt(reinterpret_cast<__UINTPTR_TYPE__>(player) + 23984 * j + 4936) > 0)
                scriptStart__FPc(const_cast<char*>("*inventoryOnNotify"));
        }
    }
    if (MC_Debug != 0 && (readInt(reinterpret_cast<__UINTPTR_TYPE__>(pad) + 12) & 0x4000) != 0 &&
        (readInt(reinterpret_cast<__UINTPTR_TYPE__>(pad) + 8) & 8) != 0) {
        *reinterpret_cast<int*>(player + 232) = 0x16e360;
        for (int k = 0; k < nmPlayers; ++k) givePlayerExperience__Fii(k, 20000);
    }
    if (drawBustOn != 0) drawBustOn = static_cast<signed char>(drawBustOn - 1);
    if (drawCompassOn != 0) drawCompassOn = static_cast<signed char>(drawCompassOn - 1);
}
