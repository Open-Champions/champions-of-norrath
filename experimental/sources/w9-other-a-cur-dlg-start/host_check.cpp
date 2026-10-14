#include "cur_dlg_start.cpp"
int main() {
    if (sizeof(curDlgStart) != 4) return 1;
    if (curDlgStart != 0) return 2;
    curDlgStart = 1;
    if (curDlgStart != 1) return 3;
    return 0;
}
