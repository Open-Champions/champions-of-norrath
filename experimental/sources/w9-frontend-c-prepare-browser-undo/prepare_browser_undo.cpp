// Observed prepareBrowserUndo__Fv at 0x0025bbf0, 148 bytes (pinned SLUS_205.65).
// Copies the 64 bytes at quickLoadStamp to undoQuickLoadStamp using unaligned
// doubleword moves (eight 8-byte pieces), then returns. undoBrowser__Fv at
// 0x0025bc88 is the same copy in the other direction.
// Inferred: the pair saves and restores the quick-load stamp around the file
// browser; the name comes from the two function names, not from the data.
// Unknown: the layout of the 64 bytes (callers treat +32 as a name for a
// memory-card command and the rest is unexamined here).
struct QuickLoadStamp {
    unsigned char bytes[64];
};
extern "C" {
extern QuickLoadStamp quickLoadStamp;
extern QuickLoadStamp undoQuickLoadStamp;

void prepareBrowserUndo__Fv() {
    undoQuickLoadStamp = quickLoadStamp;
}
}
