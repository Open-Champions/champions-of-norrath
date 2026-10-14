// Host contract for the dialog mode words. Not an EE plan input.
#include "dialog_words.cpp"

int main() {
    if (DLG_SwoopMode != 0 || DLG_StartWithSwoopDone != 0 || DLG_FireUpShopOnExit != 0) return 1;
    DLG_SwoopMode = -3;
    DLG_StartWithSwoopDone = 1;
    DLG_FireUpShopOnExit = 1;
    if (DLG_SwoopMode != -3 || DLG_StartWithSwoopDone != 1 || DLG_FireUpShopOnExit != 1) return 2;
    return 0;
}
