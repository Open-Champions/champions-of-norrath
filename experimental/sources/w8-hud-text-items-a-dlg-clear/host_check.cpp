// Host contract for DLG_Clear__Fv. Not an EE plan input.
#define NORRATH_HOST_CONTRACT 1
static unsigned char arena[256];
unsigned char* norrath_host_base = arena;
#include "dlg_clear.cpp"
extern "C" {
unsigned anim[12];
int dialogCreature, dialogLumpName, dialogMutex, activePlayer, optionsShowing;
int swooping, DLG_SwoopMode, realDlgPos, dlgPos;
unsigned curDlg;
static void* freed[4]; static int freedCount;
void memFree__FPv(void* block) { freed[freedCount++] = block; }
}
static void fill() {
    for (int i = 0; i < 12; ++i) anim[i] = 0x1000 + i;
    dialogCreature = 5; dialogLumpName = 6; dialogMutex = 7; activePlayer = 1; optionsShowing = 8;
    swooping = -1; DLG_SwoopMode = 3; realDlgPos = 9; dlgPos = 10; curDlg = 0x1234; freedCount = 0;
}
static int checkScalars() {
    return dialogCreature == 0 && dialogLumpName == 0 && dialogMutex == 0 && activePlayer == 0 &&
           optionsShowing == 0 && swooping == 0 && DLG_SwoopMode == 0 && curDlg == 0 &&
           realDlgPos == -1 && dlgPos == -1;
}
int main() {
    fill(); anim[0] = 0x40; anim[4] = 0x80;
    DLG_Clear__Fv();
    if (freedCount != 2 || freed[0] != arena + 0x40 || freed[1] != arena + 0x80) return 1;
    if (anim[0] || anim[4]) return 2;
    if (anim[1] != 0x1001 || anim[3] != 0x1003 || anim[5] != 0x1005 || anim[8] != 0x1008) return 3;
    if (!checkScalars()) return 4;
    fill(); anim[0] = 0; anim[4] = 0x80;
    DLG_Clear__Fv();
    if (freedCount != 1 || freed[0] != arena + 0x80 || anim[4] != 0) return 5;
    fill(); anim[0] = 0; anim[4] = 0;
    DLG_Clear__Fv();
    if (freedCount != 0 || !checkScalars()) return 6;
    return 0;
}
