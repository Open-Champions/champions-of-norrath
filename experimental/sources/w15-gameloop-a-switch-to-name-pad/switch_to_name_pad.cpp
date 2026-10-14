// Observed switchToNamePad__Fv at 0x00280908, 108 bytes, 16-byte frame. It fills the 26-byte array `fontGrid`
// (0x008d5ab8) with the lowercase letters 'a' to 'z' (index 25 holds 'z', index 0 holds 'a', written from the end),
// stores -3 into `nameGridPos`, calls setNewGridPosition__Fii(0, 0), clears the byte `showNoNamePrompt` and stores
// the float 1.0 into `otherHermiteTime`. It returns nothing.
// Inferred: this is the entry to the on-screen name entry grid (the name pad); -3 is a "no cell chosen" marker.
// Unknown: the meaning of the -3, and which cell setNewGridPosition selects for (0, 0).
extern "C" {
extern unsigned char fontGrid[];
extern int nameGridPos;
extern unsigned char showNoNamePrompt;
extern float otherHermiteTime;
void setNewGridPosition__Fii(int x, int y);

void switchToNamePad__Fv() {
    for (int i = 25; i >= 0; --i) fontGrid[i] = static_cast<unsigned char>('a' + i);
    nameGridPos = -3;
    setNewGridPosition__Fii(0, 0);
    showNoNamePrompt = 0;
    otherHermiteTime = 1.0f;
}
}
