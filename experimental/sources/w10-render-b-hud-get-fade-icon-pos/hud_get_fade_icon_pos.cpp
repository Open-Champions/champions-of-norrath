// Observed hudGetFadeIconPos__Fii at 0x001c7738, extent 180.
// (row, target). fadeTimer (0x0035b7b8) is read as rows of seven ints, 28 bytes per
// row; the row is selected by the first argument. The result counts the columns i
// (0..6) of that row which satisfy all of:
//   - fadeTimer[row][i] is nonzero
//   - not (i == 5 and fadeTimer[row][2] is nonzero)
//   - not (i == 4 and fadeTimer[row][1] is nonzero)
//   - not (i == 6 and fadeTimer[row][3] is nonzero)
//   - i differs from target
// Inferred: a slot index among active icons, skipping the target and the three
// columns shadowed by another; the meaning of the columns is not established.
// The row count of fadeTimer is not bounded here.
extern "C" {
extern int fadeTimer[][7];

int hudGetFadeIconPos__Fii(int row, int target) {
    const int* t = fadeTimer[row];
    int count = 0;
    for (int i = 0; i < 7; ++i) {
        if (t[i] == 0) continue;
        if (i == 5 && t[2] != 0) continue;
        if (i == 4 && t[1] != 0) continue;
        if (i == 6 && t[3] != 0) continue;
        if (i != target) ++count;
    }
    return count;
}
}
