// Host contract for the dialog state objects. Not an EE plan input.
#include "dialog_state.cpp"
static_assert(sizeof(DLG_NewWorld) == 64, "DLG_NewWorld extent");
static_assert(sizeof(dlgName) == 256, "dlgName extent");
int main() {
    if (dialogCreature || dialogIdleAnimation || dialogMutex || dlgArrayPtr || dlgPos) return 1;
    for (int i = 0; i < 64; ++i) if (DLG_NewWorld[i]) return 2;
    for (int i = 0; i < 128; ++i) if (dlgName[i]) return 3;
    return 0;
}
