#include "dlg_dir.cpp"
int main() {
    char text[2];
    if (dlgDir != nullptr) return 1;
    dlgDir = text;
    if (dlgDir != text) return 2;
    return 0;
}
