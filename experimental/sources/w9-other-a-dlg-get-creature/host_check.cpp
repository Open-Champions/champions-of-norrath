#include "dlg_get_creature.cpp"
int curDlg;
void* dialogCreature;
int main() {
    int marker;
    dialogCreature = &marker;
    curDlg = 0;
    if (DLG_GetDialogCreature__Fv() != nullptr) return 1;
    curDlg = 7;
    if (DLG_GetDialogCreature__Fv() != &marker) return 2;
    curDlg = -1;
    if (DLG_GetDialogCreature__Fv() != &marker) return 3;
    return 0;
}
