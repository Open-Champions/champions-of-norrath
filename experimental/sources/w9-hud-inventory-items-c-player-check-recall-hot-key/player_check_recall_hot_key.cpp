// Observed playerCheckRecallHotKey__Fv at 0x001ab1e8, 416 bytes.
// For each of the localNmPlayersAlive entries of localPlayersAlive (the count is
// re-read every pass), with p the player number and object the pointer word at
// record+21872 of p's 23984-byte player record, the player is skipped when any of
// these holds, tested in this order:
//   gameControlPaused is non-zero; gameIsPaused() is non-zero; DLG_Active() is
//   non-zero; the halfword at object+1188 is non-zero; the byte recallOn is
//   non-zero; the byte hudNetWaiting is non-zero; netGetCameraCustomControl(p) is
//   non-zero; engineTaskActive("jumpgate") is non-zero; engineTaskActive("pausemenu")
//   is non-zero; the word at object+1328 is non-zero; the word at object+12 has
//   bit 0x400000 set.
// Otherwise, with pad = the 88-byte record p of netLocalPad, msgHudRecall((char)p)
// runs only when (pad word at +8) & actionButton[p] and (pad word at +12) & 0x100
// are both non-zero. The first four tests happen before the later ones are
// evaluated, so a paused game never calls the later routines.
// Unknown: what the object fields and pad words mean beyond these tests.
#include "../inventory-items-record/record.hpp"
extern "C" {
extern int gameControlPaused;
extern int localNmPlayersAlive;
extern int localPlayersAlive[];
extern signed char recallOn;
extern signed char hudNetWaiting;
extern unsigned actionButton[];
extern unsigned char netLocalPad[];
int gameIsPaused__Fv();
int DLG_Active__Fv();
int netGetCameraCustomControl__Fi(int player);
int engineTaskActive__FPc(char* name);
void msgHudRecall__Fc(char player);
}

namespace {
char jumpgate_name[] = "jumpgate";
char pausemenu_name[] = "pausemenu";
}

extern "C" void playerCheckRecallHotKey__Fv() {
    using namespace inventory_items;
    for (int index = 0; index < localNmPlayersAlive; ++index) {
        const int owner = localPlayersAlive[index];
        const unsigned char* object = resolve<unsigned char>(*reinterpret_cast<const unsigned*>(player_record(owner) + 21872));
        if (gameControlPaused != 0) continue;
        if (gameIsPaused__Fv() != 0) continue;
        if (DLG_Active__Fv() != 0) continue;
        if (*reinterpret_cast<const short*>(object + 1188) != 0) continue;
        if (recallOn != 0) continue;
        if (hudNetWaiting != 0) continue;
        if (netGetCameraCustomControl__Fi(owner) != 0) continue;
        if (engineTaskActive__FPc(jumpgate_name) != 0) continue;
        if (engineTaskActive__FPc(pausemenu_name) != 0) continue;
        if (*reinterpret_cast<const int*>(object + 1328) != 0) continue;
        if ((*reinterpret_cast<const unsigned*>(object + 12) & 0x400000u) != 0) continue;
        const unsigned char* pad = netLocalPad + owner * 88;
        if ((*reinterpret_cast<const unsigned*>(pad + 8) & actionButton[owner]) == 0) continue;
        if ((*reinterpret_cast<const unsigned*>(pad + 12) & 0x100u) == 0) continue;
        msgHudRecall__Fc(static_cast<char>(owner));
    }
}
