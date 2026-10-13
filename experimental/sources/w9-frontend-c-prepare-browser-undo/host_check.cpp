#include "prepare_browser_undo.cpp"
QuickLoadStamp quickLoadStamp;
QuickLoadStamp undoQuickLoadStamp;
int main() {
    for (int i = 0; i < 64; ++i) {
        quickLoadStamp.bytes[i] = static_cast<unsigned char>(i + 1);
        undoQuickLoadStamp.bytes[i] = 0xee;
    }
    prepareBrowserUndo__Fv();
    for (int i = 0; i < 64; ++i) {
        if (undoQuickLoadStamp.bytes[i] != i + 1) return 1;
        if (quickLoadStamp.bytes[i] != i + 1) return 2;
    }
    quickLoadStamp.bytes[63] = 0;  // last byte must be copied too
    prepareBrowserUndo__Fv();
    return undoQuickLoadStamp.bytes[63] == 0 ? 0 : 3;
}
