// Observed msg_getPokeName__9SavepointPUsi at 0x00215910, 356 bytes. The integer argument is
// unused. Fills (or not) a 16-bit text buffer and returns 0 or 1.
//  * MC_Busy__Fv nonzero: returns 0 at once.
//  * Otherwise, when gameCheckNoSaveZone__Fv is nonzero: scans the players array (global
//    `player`, 23984 bytes per entry, entry count nmPlayers) for the first entry whose pointer
//    at entry+21872 is nonzero and whose pointee has a nonzero word at +1328; the scan index
//    is nmPlayers if there is none. If that index equals nmPlayersAlive the message key at
//    0x470728 is used, otherwise the one at 0x470738. The key text goes through
//    textGetString and hudSetInfoMessage(text, 180, -1, 0); returns 0. The buffer is not written.
//  * Otherwise, with netNetworkMode zero, the text of the key at 0x470770 is copied into the
//    buffer with ustrcpy and 1 is returned. With netNetworkMode nonzero the key at 0x470748 is
//    used when MC_CheckPointAge is at least 1801 and the signed byte at object+199 is -1,
//    otherwise the key at 0x470760; the text is copied the same way and 1 is returned.
// The five keys are the strings "SaveInactive", "SaveVehicle", "SaveGameNetPlayersBusy",
// "SaveGameNet" and "SaveGame". Unknown: what the checks mean beyond their names. Nothing here
// ran on the EE.
#include "../w10-actor-l-common/access.hpp"
extern "C" {
int MC_Busy__Fv();
int gameCheckNoSaveZone__Fv();
unsigned short* textGetString__FPc(char* key);
void hudSetInfoMessage__FPUsiic(unsigned short* text, int a, int b, char c);
void ustrcpy__FPUsT0(unsigned short* destination, unsigned short* source);
extern unsigned char player[];
extern int nmPlayers;
extern int nmPlayersAlive;
extern signed char netNetworkMode;
extern int MC_CheckPointAge;

int msg_getPokeName__9SavepointPUsi(void* self, unsigned short* out, int) {
    const unsigned char* const o = static_cast<const unsigned char*>(self);
    if (MC_Busy__Fv() != 0) return 0;
    if (gameCheckNoSaveZone__Fv() != 0) {
        const int count = nmPlayers;
        int index = 0;
        for (; index < count; ++index) {
            const unsigned char* const entry = player + index * 23984;
            const unsigned entryPointer = *reinterpret_cast<const unsigned*>(entry + 21872);
            if (entryPointer == 0) continue;
            if (*reinterpret_cast<const int*>(w10l::from_word(entryPointer) + 1328) != 0) break;
        }
        const bool same = (index == nmPlayersAlive);
        hudSetInfoMessage__FPUsiic(textGetString__FPc(const_cast<char*>(same ? "SaveInactive" : "SaveVehicle")), 180, -1, 0);
        return 0;
    }
    const char* key = "SaveGame";
    if (netNetworkMode != 0) {
        key = "SaveGameNet";
        if (MC_CheckPointAge >= 1801 && static_cast<signed char>(o[199]) == -1) key = "SaveGameNetPlayersBusy";
    }
    ustrcpy__FPUsT0(out, textGetString__FPc(const_cast<char*>(key)));
    return 1;
}
}
