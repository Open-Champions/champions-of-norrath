// Observed updateCursor__FiiiiiP12screenStructi at 0x001b26f0, 924 bytes. Arguments, in order:
// player, rows, columns, rowHeight, columnWidth, screen record (ints), play-sound flag. No result.
// Screen words used: [5] move direction, [6] column, [7] row, [8] y offset, [9] x offset, [15] timer.
// Nothing happens when multiHelpMessageOn[player] is nonzero, sellMode[player] is nonzero, or
// helpMessageOn is nonzero.
//  1. With a zero flag, bits 0x200000, 0x100000, 0x800000 and 0x400000 are cleared in the word at
//     netLocalPad + 8 + 88 * player.
//  2. [5] = getMoveDir__FiiiP12screenStruct(player, rows, columns, screen). With rows above zero the
//     direction is cleared when it points out of range: -1 (left) with [6] below zero, 1 (right) with
//     [6] above columns - 1, 4 (down) with rows at most [7], 2 (up) with [7] - 1 below zero.
//  3. Direction -1 or 1: [15] grows by 0.09090909 and h = evalHermite__Ffffff([15], 0, 1, 0, 0) is
//     found. [9] = [6] * columnWidth + (int)((h + 0.01) * columnWidth * direction). While
//     [9] is below ([6] + 1) * columnWidth and above ([6] - 1) * columnWidth the call returns.
//     Otherwise (with a flag and netPlayerIsLocal__Fi(player), the lump "fx.lmp" animation
//     "game_butn_browse.anm" is played at volume 1.0): [5] and [15] become 0, [6] becomes
//     [6] + direction and [9] = [6] * columnWidth.
//  4. Otherwise, with rowHeight above zero and direction 4 or 2: the same on the vertical words,
//     with the step 0.09090909 and 0.01, [8] in place of [9], [7] in place of [6], rowHeight in
//     place of columnWidth, and (direction - 3) as the signed step; the finishing row is
//     [7] + direction - 3.
// Inferred: this animates a selection cursor across a grid one cell per call burst, and 4 and 2 are
// down and up.
// Unknown: how getMoveDir encodes diagonals, and why vertical clamps need rows but vertical moves
// need rowHeight.
extern "C" {
extern signed char multiHelpMessageOn[];
extern int sellMode[];
extern signed char helpMessageOn;
extern unsigned char netLocalPad[];
int getMoveDir__FiiiP12screenStruct(int player, int rows, int columns, int* screen);
float evalHermite__Ffffff(float t, float a, float b, float c, float d);
int netPlayerIsLocal__Fi(int player);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume);
}

namespace {
inline void playBrowseSound(int player) {
    if (netPlayerIsLocal__Fi(player) != 0)
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
            lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("game_butn_browse.anm")), nullptr, 1.0f);
}
// Advances the animation along one axis. position / offset are the cell index and pixel offset words.
// Returns true while the slide is still in progress.
inline bool slide(int* screen, int player, int flag, int positionIndex, int offsetIndex, int cell, int step, float timerStep, float bias, int stepCount) {
    float& timer = *reinterpret_cast<float*>(&screen[15]);
    timer += timerStep;
    const float h = evalHermite__Ffffff(timer, 0.0f, 1.0f, 0.0f, 0.0f);
    const int position = screen[positionIndex];
    const int offset = position * cell + static_cast<int>((h + bias) * static_cast<float>(cell) * static_cast<float>(stepCount));
    screen[offsetIndex] = offset;
    if (offset < position * cell + cell && (position - 1) * cell < offset) return true;
    if (flag != 0) playBrowseSound(player);
    const int next = screen[positionIndex] + step;
    screen[5] = 0;
    timer = 0.0f;
    screen[positionIndex] = next;
    screen[offsetIndex] = next * cell;
    return false;
}
}

extern "C" void updateCursor__FiiiiiP12screenStructi(int player, int rows, int columns, int rowHeight, int columnWidth, int* screen, int flag) {
    if (multiHelpMessageOn[player] != 0) return;
    if (sellMode[player] != 0) return;
    if (helpMessageOn != 0) return;
    if (flag == 0) {
        unsigned* word = reinterpret_cast<unsigned*>(netLocalPad + 8 + player * 88);
        *word = *word & 0xffdfffffu & 0xffefffffu & 0xff7fffffu & 0xffbfffffu;
    }
    int direction = getMoveDir__FiiiP12screenStruct(player, rows, columns, screen);
    screen[5] = direction;
    if (rows > 0) {
        if (direction == -1) {
            if (screen[6] < 0) screen[5] = 0;
        } else if (direction == 1) {
            if (columns - 1 < screen[6]) screen[5] = 0;
        } else if (direction == 4) {
            if (rows < screen[7] + 1) screen[5] = 0;
        } else if (direction == 2) {
            if (screen[7] - 1 < 0) screen[5] = 0;
        }
    }
    direction = screen[5];
    if (direction == -1 || direction == 1) {
        slide(screen, player, flag, 6, 9, columnWidth, direction, 0.09090908616781235f, 0.009999999776482582f, direction);
        return;
    }
    if (rowHeight <= 0) return;
    if (direction == 4 || direction == 2) slide(screen, player, flag, 7, 8, rowHeight, direction - 3, 0.09090908616781235f, 0.009999999776482582f, direction - 3);
}
