#include "display_dialog_pos.cpp"
int main() {
    if (sizeof(displayDialogPos) != 4) return 1;
    if (displayDialogPos != 0) return 2;
    displayDialogPos = 1;
    if (displayDialogPos != 1) return 3;
    return 0;
}
