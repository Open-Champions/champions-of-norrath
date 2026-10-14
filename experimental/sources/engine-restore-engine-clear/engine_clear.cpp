// Observed engineClear at 0x001911c8, 48 bytes. It returns.
// Cause 2 is disabled, then the handler id in vblankHID is removed, then
// drawRemoveInterrupts runs. vblankHID is the local word at 0x004843ec.
extern "C" {
void DisableIntc(int cause);
int RemoveIntcHandler(int cause, int handler);
void drawRemoveInterrupts__Fv();
extern int vblankHID;

void engineClear__Fv() {
    DisableIntc(2);
    RemoveIntcHandler(2, vblankHID);
    drawRemoveInterrupts__Fv();
}
}
