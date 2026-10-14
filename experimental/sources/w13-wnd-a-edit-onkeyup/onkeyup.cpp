// Observed: OnKeyUp__9VIWndEditi at 0x010ad778 is 88 bytes. With (this, key): the word at +384 is cleared when it
// equals key. If the word sCompactKeyboard (0x01192bc8, a 4-byte static) equals 1 the function then returns.
// Otherwise: key below 136 clears +384 and returns; key from 136 to 139 returns with no further change; key of
// 140 or more clears +384 and, when key is exactly 142, also clears the word at +48.
// Inferred: +384 holds the key currently being held for repeat, and the compact-keyboard flag disables the
// special-key handling; both readings come from the comparisons only.
// Unknown: the meaning of key values 136..142, and the owner and type of sCompactKeyboard.
extern "C" {
extern int sCompactKeyboard;

void OnKeyUp__9VIWndEditi(void* self, int key) {
    int* words = static_cast<int*>(self);
    if (key == words[96]) words[96] = 0;
    if (sCompactKeyboard == 1) return;
    if (key < 136) {
        words[96] = 0;
        return;
    }
    if (key < 140) return;
    words[96] = 0;
    if (key == 142) words[12] = 0;
}
}
